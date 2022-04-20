#include <Servo.h>

const int rollPin = 12;
const int pitchPin = 11;
const int yawPin = 10;

Servo servoRoll;
Servo servoPitch;
Servo servoYaw;

void setup()
{
    Serial.begin(115200);
    while (!Serial)
        ;

    Serial.println("Serial port connected, starting setup...");

    servoRoll.attach(rollPin, 500, 2500);
    servoPitch.attach(pitchPin, 500, 2500);
    servoYaw.attach(yawPin, 500, 2500);

    delay(500);

    servoRoll.write(0);
    servoPitch.write(0);
    servoYaw.write(0);

    delay(2000);

    Serial.println("Setup is done!");
    Serial.flush();
}

void loop()
{
    static int step = 1;
    static int roll = 0;
    static int pitch = 0;
    static int yaw = 0;

    servoRoll.write(roll);
    delay(10);
    servoPitch.write(pitch);
    delay(10);
    servoYaw.write(yaw);

    roll += step;
    pitch += step;
    yaw += step;

    if (roll >= 180)
    {
        step = -step;
    }
    else if (roll <= 0)
    {
        // Done with test - do nothing until a reset
        while (1)
            delay(5000);
    }

    Serial.print("Roll: ");
    Serial.print(roll);
    Serial.print(", Pitch: ");
    Serial.print(pitch);
    Serial.print(", Yaw: ");
    Serial.print(yaw);
    Serial.print(", Step: ");
    Serial.println(step);

    delay(30);
}
