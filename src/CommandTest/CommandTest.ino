#include "commands.h"

void setup()
{
    Serial.begin(115200);
    while (!Serial)
        ;

    Serial.print("sizeof Command: ");
    Serial.println((byte)sizeof(Command));
}

void loop()
{
    Command cmd = getCommand();
    executeCommand(cmd);

    delay(2000);
}

Command getCommand()
{
    static int ndx = 0;
    Command cmd;

    if (ndx++ % 2 == 0)
    {
        cmd = Command(CMD_NAVLED, 1);
        Serial.println(cmd.toString());
    }
    else
    {
        cmd = Command(CMD_PITCH, 150);
        Serial.println(cmd.toString());
    }

    return cmd;
}

void executeCommand(Command cmd)
{
    Serial.print("Executing: ");
    Serial.println(cmd.toString());
}
