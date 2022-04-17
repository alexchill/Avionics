#include <Servo.h>

const int MotorPin = 11;

Servo Motor;

void setup()
{
    Serial.begin(115200);
    while (!Serial)
        ;

    Serial.println("Serial port connected, starting setup...");

    Motor.attach(MotorPin, 1000, 2000);
    Motor.write(0);

    // 10 hour timeout
    Serial.setTimeout(36000000);

    Serial.println("Enter 'y' BEGIN Motor/ESC calibration, press Enter to skip");
    String resp = Serial.readStringUntil('\n');
    if (resp == "y")
    {
        Motor.write(180);

        Serial.println("Attach battery and press Enter when 4 beeps for desired setting is reached");
        Serial.readStringUntil('\n');

        Motor.write(0);

        Serial.println("END Motor/ESC calibration - detach battery and restart program");
        while (1)
            delay(1000);
    }
    else
    {
        Motor.write(0);

        Serial.println("Attach battery then press Enter");
        Serial.readStringUntil('\n');

        Serial.println("Wait for motor to arm then press Enter to begin motor test loop");
        Serial.readStringUntil('\n');
    }

    Serial.flush();
}

void loop()
{
    static int step = 5;
    static int throttle = 0;

    Motor.write(throttle);

    throttle += step;

    if (throttle >= 90)
    {
        step = -step;
    }
    else if (throttle <= 0)
    {
        // Done with test - do nothing until a reset
        while (1)
            delay(5000);
    }

    Serial.print("Throttle: ");
    Serial.print(throttle);
    Serial.print(", Step: ");
    Serial.println(step);

    delay(250);
}
