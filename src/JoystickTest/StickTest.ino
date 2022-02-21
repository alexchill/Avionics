/* Simplified Logitech Extreme 3D Pro Joystick Report Parser */

#include <usbhid.h>
#include <hiduniversal.h>
#include <usbhub.h>
#include <Servo.h>

#include "le3dp_rptparser.h"

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB                                             Usb;
USBHub                                          Hub(&Usb);
HIDUniversal                                    Hid(&Usb);
JoystickEvents                                  JoyEvents;
JoystickReportParser                            Joy(&JoyEvents);

Servo servoX;
Servo servoY;

void setup()
{
  Serial.begin( 115200 );
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif

  int xPin = 13;
  int yPin = 12;
  
  Serial.println("Serial port connected, starting setup...");

  servoX.attach(xPin, 1000, 2000);
  servoY.attach(yPin, 500, 2500);


  Serial.println("Initing USB...");
  if (Usb.Init() == -1)
    Serial.println("OSC did not start.");

  Serial.println("Setting up HID report parser...");
  if (!Hid.SetReportParser(0, &Joy))
    ErrorMessage<uint8_t>(PSTR("SetReportParser"), 1  );

  delay( 2000 );
}

void loop()
{
  Usb.Task();

  uint16_t valX = map(JoystickEvents::mostRecentEvent.x, 0, 1023, 0, 180);
  uint16_t valY = map(JoystickEvents::mostRecentEvent.y, 0, 1023, 0, 180);
  uint16_t valS = map(JoystickEvents::mostRecentEvent.slider, 0, 255, 100, 0);
  Serial.println(valY);
  
  servoX.write(valX);
  servoY.write(valY);
  delay(15);
}
