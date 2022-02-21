#include "commands.h"

void setup()
{
  Serial.begin(9600);
  while (!Serial)
    ;

    Command cmd = Command(CMD_NAVLED, 1);
    Serial.println(cmd.toString());
    
    cmd = Command(CMD_PITCH, 150);
    Serial.println(cmd.toString());
}

void loop()
{
    delay(5000);
}
