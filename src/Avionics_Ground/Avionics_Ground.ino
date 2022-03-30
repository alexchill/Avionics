/* Simplified Logitech Extreme 3D Pro Joystick Report Parser */

#include <usbhid.h>
#include <hiduniversal.h>
#include <usbhub.h>
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

void setup()
{
  Serial.begin(115200);
  // some boards need to wait to ensure access to serial over USB
  while (!Serial);
  Serial.println("Serial port connected, starting GROUND avionics setup...");

  Serial.println("Initiaing USB...");
  if (Usb.Init() == -1)
    Serial.println("OSC did not start.");

  Serial.println("Setting up HID report parser...");
  if (!Hid.SetReportParser(0, &Joy))
    ErrorMessage<uint8_t>(PSTR("SetReportParser"), 1);

  Serial.println("Setting up radio");
  if (!radio.begin()) {
    Serial.println(F("Radio hardware is not responding!!"));

    // hold in infinite loop
    while (1);
  }
  Serial.println(F("Radio hardware is responding!!"));

  radio.setPALevel(RF24_PA_MAX);
  //radio.setDataRate(RF24_250KBPS);

  Serial.print("Radio channel is set to: ");
  Serial.println(radio.getChannel());
  Serial.print("Radio data rate is set to: ");
  Serial.println(radio.getDataRate());

  radio.stopListening();
  radio.openWritingPipe(RadioAddress);

  Serial.println("Radio setup complete");
  Serial.println("Waiting to arm joystick");
}

void loop()
{
  static unsigned long loopStartTimeMS = millis();
  static unsigned long lastSignalTestTime = millis();
  static int prevId = -1;
  static bool joystickArmed = false;

  unsigned long nowMs = millis();
  if (nowMs > lastSignalTestTime + 30000)
  {
    lastSignalTestTime = nowMs;
    bool goodSignal = radio.testRPD();
    Serial.println(goodSignal ? "Strong signal > 64dBm" : "Weak signal < 64dBm" );
  }

  Usb.Task();

  // Don't send joystick data for the first 10 seconds after USB initializes
  // due to spuriuos data from the joystick.
  if (!joystickArmed && (millis() > loopStartTimeMS + 10000))
  {
    joystickArmed = true;
    Serial.println("Joystick is armed");
  }

  // Don't send joystick data unless it has changed and is armed
  GamePadEventData mostRecentEvent = JoystickEvents::mostRecentEvent;
  if ((mostRecentEvent.id != prevId) && joystickArmed) {
    prevId = mostRecentEvent.id;

    Commands cmd;
    cmd.roll = map(JoystickEvents::mostRecentEvent.x, 0, 1023, 0, 180);
    cmd.pitch = map(JoystickEvents::mostRecentEvent.y, 0, 1023, 0, 180);
    cmd.yaw = map(JoystickEvents::mostRecentEvent.twist, 0, 255, 0, 180);
    cmd.throttle = map(JoystickEvents::mostRecentEvent.slider, 0, 255, 180, 0);

    Serial.println(toString(&cmd));

    if (radio.write(&cmd, sizeof(Commands))) {
      Serial.println("transmitted");
    }
    else
    {
      Serial.println("fail");
    }
  }

  delay(50);
}
