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

const byte address[6] = "00001";

void setup()
{
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial)
    ; // some boards need to wait to ensure access to serial over USB
#endif
  Serial.println("Serial port connected, starting setup...");

  Serial.println("Initiaing USB...");
  if (Usb.Init() == -1)
    Serial.println("OSC did not start.");

  Serial.println("Setting up HID report parser...");
  if (!Hid.SetReportParser(0, &Joy))
    ErrorMessage<uint8_t>(PSTR("SetReportParser"), 1);

  Serial.println("Setting up radio");
  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {} // hold in infinite loop
  }
  Serial.println(F("radio hardware is responding!!"));

  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  //radio.setDataRate(RF24_250KBPS);
  radio.stopListening();
  //defaultValue();
  Serial.println("Radio setup complete");
}

void loop()
{
  static int prevId = -1;

  Usb.Task();

  GamePadEventData mostRecentEvent = JoystickEvents::mostRecentEvent;

  if (mostRecentEvent.id != prevId) {
    Commands cmd;

    cmd.roll = map(JoystickEvents::mostRecentEvent.x, 0, 1023, 0, 180);
    cmd.pitch = map(JoystickEvents::mostRecentEvent.y, 0, 1023, 0, 180);
    cmd.yaw = map(JoystickEvents::mostRecentEvent.twist, 0, 255, 0, 180);
    cmd.throttle = map(JoystickEvents::mostRecentEvent.slider, 0, 255, 100, 0);

    Serial.println(toString(&cmd));
    
    const char text[] = "HiDude";
    if (radio.write(&cmd, sizeof(Commands))) {
      Serial.println("transmitted");
    }
    else
    {
      Serial.println("fail");
    }

    prevId = mostRecentEvent.id;
  }

  delay(100);
}
