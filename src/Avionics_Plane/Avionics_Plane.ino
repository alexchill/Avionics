#include <SPI.h>
#include <printf.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

#include "Joystickcommands.h"

const unsigned long LastRadioMessageReceivedTimeoutMS = 10000;

RF24 radio(3, 2);
const uint8_t RadioAddress[6] = "00001";

struct telemetry
{
    bool stale = true;
    bool goodSignal = false;
    float batteryVoltage = 0;
};

typedef struct telemetry Telemetry;
Telemetry telemetryData;

// Digital pin
const int ServoRollPin = 12;
const int ServoPitchPin = 11;
const int ServoYawPin = 10;
const int ServoNoseGearPin = 8;
const int ServoLeftGearPin = 7;
const int ServoRightGearPin = 6;
const int MotorPin = 5;
const int ModeSwitchPin = 0;

const int MotorInitThrottle = 0;

// Analog pin
const int BATT_MEAS_PIN = 0;

const int NavLedsPin = 13;

int ServoRollCenter = 90;
int ServoPitchCenter = 90;
int ServoYawCenter = 90;

const int servoGearUp = 180;
const int servoGearDown = 90;

Servo servoRoll;
Servo servoPitch;
Servo servoYaw;
Servo servoNoseGear;
Servo servoLeftGear;
Servo servoRightGear;
Servo motor1;

const float MAX_MEAS_VOLT = 5; // max voltage arduino analog input will handle
const int AIN_ADC_BITS = 10;   // analog number analog/digital convert number of bits
const float AIN_VOLT_PER_UNIT = MAX_MEAS_VOLT / pow(2, AIN_ADC_BITS);

const float BATT_VOLT_MAX = 12;

const float BATT_MEAS_R1 = 4.7e6;
const float BATT_MEAS_R2 = 1.55e6;
const float BATT_MEAS_RA = 100e6;

// total resistance of resistor 2 and analog input impedance in parallel
const float BATT_MEAS_R2T = (BATT_MEAS_RA * BATT_MEAS_R2) / (BATT_MEAS_RA - BATT_MEAS_R2);
const float BATT_MEAS_DIV = (BATT_MEAS_R1 / BATT_MEAS_R2T) + 1;
const float BATT_MEAS_MAX_CURRENT = BATT_VOLT_MAX / (BATT_MEAS_R1 + BATT_MEAS_R2T);

const int telemetryDataInterval = 10000; // 10 seconds

static unsigned long landingGearToggle;

void setup()
{
    Serial.begin(115200);
    printf_begin();

    // some boards need to wait to ensure access to serial over USB
    while (!Serial)
        ;
    Serial.println("Serial port connected, starting PLANE avionics setup...");

    pinMode(ModeSwitchPin, INPUT_PULLUP);
    int mode = digitalRead(ModeSwitchPin);
    Serial.print("Mode switch set to: ");
    Serial.println(mode);

    servoRoll.attach(ServoRollPin, 500, 2500);
    servoRoll.write(ServoRollCenter);

    servoPitch.attach(ServoPitchPin, 500, 2500);
    servoPitch.write(ServoPitchCenter);

    servoYaw.attach(ServoYawPin, 500, 2500);
    servoYaw.write(ServoYawCenter);

    servoNoseGear.attach(ServoNoseGearPin, 500, 2500);
    servoNoseGear.write(servoGearDown);

    servoLeftGear.attach(ServoLeftGearPin, 500, 2500);
    servoLeftGear.write(servoGearDown);

    servoRightGear.attach(ServoRightGearPin, 500, 2500);
    servoRightGear.write(servoGearDown);

    motor1.attach(MotorPin, 1000, 2000);
    motor1.write(MotorInitThrottle);

    // Turn on by default, use buttons to turn on and off
    digitalWrite(NavLedsPin, HIGH);

    Serial.println("Setting up radio");
    if (!radio.begin())
    {
        Serial.println(F("Radio hardware is not responding!!"));

        // hold in infinite loop if radio is not responding
        while (1)
            ;
    }

    Serial.println(F("Radio hardware is responding!!"));

    // Configure radio
    radio.enableAckPayload();
    radio.setDataRate(RF24_250KBPS);
    radio.setPALevel(RF24_PA_MAX);
    radio.openReadingPipe(1, RadioAddress);
    radio.startListening();

    Serial.println("Radio setup complete, configuration is:");
    radio.printPrettyDetails();

    checkBattMeasAnalogInVoltage();
}

void loop()
{
    static unsigned long loopStartTime = millis();
    static unsigned long lastSignalTestTime = loopStartTime;
    static unsigned long lastRadioMessageReceivedTime = loopStartTime;
    static unsigned long lastBatteryMeasureTime = loopStartTime;
    static unsigned long lastgearToggleTime = loopStartTime;

    static bool inSafetyShutdownMode = false;

    static unsigned long movingTime = 5000;
    static unsigned long moveStartTime;

    unsigned long nowMs = millis();
    if (nowMs > lastSignalTestTime + telemetryDataInterval)
    {
        lastSignalTestTime = nowMs;
        telemetryData.goodSignal = radio.testRPD();
        telemetryData.stale = false;
        Serial.println(telemetryData.goodSignal ? "Strong signal > 64dBm" : "Weak signal < 64dBm");
    }

    if (radio.available())
    {
        Commands cmd;
        radio.read(&cmd, sizeof(Commands));

        // load the payload for the next time
        radio.writeAckPayload(1, &telemetryData, sizeof(telemetryData));
        Serial.print("Telemetry stale: ");
        Serial.println(telemetryData.stale);
        Serial.print("Plane battery voltage: ");
        Serial.println(telemetryData.batteryVoltage);        
        telemetryData.stale = true;

        lastRadioMessageReceivedTime = millis();
        inSafetyShutdownMode = false;

        servoRoll.write(cmd.roll);
        servoPitch.write(cmd.pitch);
        servoYaw.write(cmd.yaw);
        motor1.write(cmd.throttle);

        if (cmd.gear == 2 && millis() > lastgearToggleTime + 500)
        {
            landingGearToggle = !landingGearToggle;
            Serial.println(landingGearToggle);
            lastgearToggleTime = millis();
        }

        printCommand(cmd);
    }

    // Safety shutdown of motor if lost connectiviy to ground
    if (!inSafetyShutdownMode && (millis() > lastRadioMessageReceivedTime + LastRadioMessageReceivedTimeoutMS))
    {
        inSafetyShutdownMode = true;

        servoRoll.write(ServoRollCenter);
        servoPitch.write(ServoPitchCenter);
        servoYaw.write(ServoYawCenter);
        motor1.write(MotorInitThrottle);

        Serial.print("Radio message not received for >= ");
        Serial.print(LastRadioMessageReceivedTimeoutMS);
        Serial.println(" ms, going into safety shutdown mode");
    }

    if (millis() > lastBatteryMeasureTime + telemetryDataInterval)
    {
        telemetryData.batteryVoltage = measureBattVolt();
        telemetryData.stale = false;
        Serial.print("Battery Voltage: ");
        Serial.println(telemetryData.batteryVoltage);
        lastBatteryMeasureTime = millis();
    }

    // handle landing gear up/down
    if (landingGearToggle == 1)
    {
        int startAngle = servoGearDown;
        int stopAngle = servoGearUp;
        unsigned long progress = millis() - lastgearToggleTime;
        if (progress <= movingTime)
        {
            long angle = map(progress, 0, movingTime, stopAngle, startAngle);
            servoNoseGear.write(angle);
            servoLeftGear.write(angle);
            servoRightGear.write(angle);
        }
    }
    else
    {
        int startAngle = servoGearUp;
        int stopAngle = servoGearDown;
        unsigned long progress = millis() - lastgearToggleTime;
        if (progress <= movingTime)
        {
            long angle = map(progress, 0, movingTime, stopAngle, startAngle);
            servoNoseGear.write(angle);
            servoLeftGear.write(angle);
            servoRightGear.write(angle);
        }
    }

    delay(25);
}

void printCommand(Commands cmd)
{
    Serial.print("Roll: ");
    Serial.print(cmd.roll);

    Serial.print(", Pitch: ");
    Serial.print(cmd.pitch);

    Serial.print(", Yaw: ");
    Serial.print(cmd.yaw);

    Serial.print(", Throttle: ");
    Serial.print(cmd.throttle);

    Serial.print(", Gear: ");
    Serial.println(cmd.gear);
}

float measureBattVolt()
{
    int val = analogRead(BATT_MEAS_PIN);
    return computeBattVolt(val);
}

float computeBattVolt(int adValue)
{
    float measVolt = adValue * AIN_VOLT_PER_UNIT;
    float battVolt = measVolt * BATT_MEAS_DIV;
    return battVolt;
}

void checkBattMeasAnalogInVoltage()
{
    float maxVoltMeasured = BATT_MEAS_MAX_CURRENT * BATT_MEAS_R2T;
    Serial.print("Max voltage measured: ");
    Serial.println(maxVoltMeasured);
    if (maxVoltMeasured > MAX_MEAS_VOLT)
    {
        Serial.print("Battery monitor resistor values allow max analog input voltage to be exceeded: ");
        Serial.println(maxVoltMeasured);
    }
}
