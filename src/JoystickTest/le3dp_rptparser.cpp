#include "le3dp_rptparser.h"

GamePadEventData JoystickEvents::mostRecentEvent;

JoystickReportParser::JoystickReportParser(JoystickEvents *evt) :
  joyEvents(evt)
{}

void JoystickReportParser::Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf)
{
  static uint32_t id = 1;
  bool match = true;

  // Checking if there are changes in report since the method was last called
  ((GamePadEventData*)buf)->id = 0;
  ((GamePadEventData*)oldPad)->id = 0;
  for (uint8_t i = 0; i < RPT_GAMEPAD_LEN; i++) {
    if ( buf[i] != oldPad[i] ) {
      match = false;
      break;
    }
  }
  
  // Calling Game Pad event handler
  if (!match && joyEvents) {
    ((GamePadEventData*)buf)->id = id++;
    joyEvents->OnGamePadChanged((const GamePadEventData*)buf);
        
    for (uint8_t i = 0; i < RPT_GAMEPAD_LEN; i++)
    {
      oldPad[i] = buf[i];
    }
  }
}

void JoystickEvents::OnGamePadChanged(const GamePadEventData *evt)
{
  mostRecentEvent = *evt;
  uint16_t inputX = evt->x;
  uint16_t inputY = evt->y;
  uint16_t inputT = evt->twist;
  uint16_t inputS = evt->slider;
//
//  Serial.print("ID: ");
//  Serial.print(evt->id);
//  Serial.print(" X: ");
//  PrintHex<uint16_t>(evt->x, 0x80);
//  Serial.print(" Y: ");
//  PrintHex<uint16_t>(evt->y, 0x80);
//  Serial.print(" Hat Switch: ");
//  PrintHex<uint8_t>(evt->hat, 0x80);
//  Serial.print(" Twist: ");
//  PrintHex<uint8_t>(evt->twist, 0x80);
//  Serial.print(" Slider: ");
//  PrintHex<uint8_t>(evt->slider, 0x80);
//  Serial.print(" Buttons A: ");
//  PrintHex<uint8_t>(evt->buttons_a, 0x80);
//  Serial.print(" Buttons B: ");
//  PrintHex<uint8_t>(evt->buttons_b, 0x80);
//  Serial.println("");
}
