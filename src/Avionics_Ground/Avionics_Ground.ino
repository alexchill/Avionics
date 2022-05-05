/* Simplified Logitech Extreme 3D Pro Joystick Report Parser */

#include <usbhid.h>
#include <hiduniversal.h>
#include <usbhub.h>
#include <printf.h>
#include <nRF24L01.h>
#include <RF24.h>

#include "le3dp_rptparser.h"
#include "Joystickcommands.h"

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
USBHub Hub(&Usb);
HIDUniversal Hid(&Usb);
JoystickEvents JoyEvents;
JoystickReportParser Joy(&JoyEvents);

RF24 radio(6, 7);
const uint8_t RadioAddress[6] = "00001";

struct telemetry
{
    bool stale = true;
    bool goodSignal = false;
    float batteryVoltage = 0;
};

typedef struct telemetry Telemetry;
Telemetry telemetryData;

void setup()
{
    Serial.begin(115200);
    printf_begin();

    // some boards need to wait to ensure access to serial over USB
    while (!Serial)
        ;
    Serial.println("Serial port connected, starting GROUND avionics setup...");

    Serial.println("Initiaing USB...");
    if (Usb.Init() == -1)
        Serial.println("OSC did not start.");

    Serial.println("Setting up HID report parser...");
    if (!Hid.SetReportParser(0, &Joy))
        ErrorMessage<uint8_t>(PSTR("SetReportParser"), 1);

    Serial.println("Setting up radio");
    if (!radio.begin())
    {
        Serial.println(F("Radio hardware is not responding!!"));

        // hold in infinite loop
        while (1)
            ;
    }

    Serial.println(F("Radio hardware is responding!!"));

    // Configure radio
    radio.enableAckPayload();
    radio.setRetries(5, 5); // delay, count
    radio.setDataRate(RF24_250KBPS);
    radio.setPALevel(RF24_PA_MAX);
    radio.openWritingPipe(RadioAddress);
    radio.stopListening();

    Serial.println("Radio setup complete, configuration is:");
    radio.printPrettyDetails();

    Serial.println("Waiting to arm joystick");
}

void loop()
{
    static unsigned long loopStartTime = millis();
    static unsigned long lastSignalTestTime = loopStartTime;
    static unsigned long lastRadioMessageSentTime = loopStartTime;
    static int prevId = -1;
    static bool joystickArmed = false;

    unsigned long nowMs = millis();
    if (nowMs > lastSignalTestTime + 30000)
    {
        lastSignalTestTime = nowMs;
        bool goodSignal = radio.testRPD();
        Serial.println(goodSignal ? "Strong signal > 64dBm" : "Weak signal < 64dBm");
    }

    Usb.Task();

    // Don't send joystick data for the first 10 seconds after USB initializes
    // due to spuriuos data from the joystick.
    if (!joystickArmed && (millis() > loopStartTime + 10000))
    {
        joystickArmed = true;
        Serial.println("Joystick is armed");
    }

    // Don't send joystick data unless joystick is armed and it has changed
    // (or has been 4 seconds since last message sent).
    GamePadEventData mostRecentEvent = JoystickEvents::mostRecentEvent;
    if (joystickArmed && ((mostRecentEvent.id != prevId) || (millis() > lastRadioMessageSentTime + 4000)))
    {
        prevId = mostRecentEvent.id;

        Commands cmd;
        cmd.roll = map(JoystickEvents::mostRecentEvent.x, 0, 1023, 0, 180);
        cmd.pitch = map(JoystickEvents::mostRecentEvent.y, 0, 1023, 0, 180);
        cmd.yaw = map(JoystickEvents::mostRecentEvent.twist, 0, 255, 0, 180);
        cmd.throttle = map(JoystickEvents::mostRecentEvent.slider, 0, 255, 180, 13);
        cmd.gear = JoystickEvents::mostRecentEvent.buttons_a;

        // print command
        Serial.println(toString(&cmd));

        if (radio.write(&cmd, sizeof(Commands)))
        {
            if (radio.isAckPayloadAvailable())
            {
                radio.read(&telemetryData, sizeof(telemetryData));
                if (!telemetryData.stale)
                {
                    Serial.print("Plane battery voltage: ");
                    Serial.println(telemetryData.batteryVoltage);
                    Serial.print("Plane signal strength: ");
                    Serial.println(telemetryData.goodSignal ? "strong > 64dBm" : "weak < 64dBm");
                }
            }
        }
        else
        {
            Serial.println("Transmit failed");
        }

        lastRadioMessageSentTime = millis();
    }

    delay(50);
}
