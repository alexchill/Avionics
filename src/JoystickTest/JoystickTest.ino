/* Simplified Logitech Extreme 3D Pro Joystick Report Parser */

#include <usbhid.h>
#include <hiduniversal.h>
#include <usbhub.h>

#include "le3dp_rptparser.h"

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

void setup()
{
    Serial.begin(115200);
#if !defined(__MIPSEL__)
    while (!Serial)
        ; // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif

    Serial.println("Serial port connected, starting setup...");

    Serial.println("Initiaing USB...");
    if (Usb.Init() == -1)
        Serial.println("OSC did not start.");

    Serial.println("Setting up HID report parser...");
    if (!Hid.SetReportParser(0, &Joy))
        ErrorMessage<uint8_t>(PSTR("SetReportParser"), 1);

    delay(2000);
}

void loop()
{
    Usb.Task();

    uint16_t valX = map(JoystickEvents::mostRecentEvent.x, 0, 1023, 0, 180);
    uint16_t valY = map(JoystickEvents::mostRecentEvent.y, 0, 1023, 0, 180);
    uint16_t valS = map(JoystickEvents::mostRecentEvent.slider, 0, 255, 100, 0);
    uint16_t valT = map(JoystickEvents::mostRecentEvent.twist, 0, 255, 0, 180);

    Serial.println(valT);

    delay(15);
}
