#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

#include "Joystickcommands.h"

const unsigned long LastRadioMessageReceivedTimeoutMS = 10000;

RF24 radio(9, 10);
const uint8_t RadioAddress[6] = "00001";

const int ServoRollPin = 3;
const int ServoPitchPin = 5;
const int ServoYawPin = 8;
const int MotorPin = 11;
const int MotorInitThrottle = 0;

const int NavLedsPin = 13;

int ServoRollCenter = 90;
int ServoPitchCenter = 90;
int ServoYawCenter = 90;

Servo servoRoll;
Servo servoPitch;
Servo servoYaw;
Servo motor1;

void setup()
{
  Serial.begin(115200);

  // some boards need to wait to ensure access to serial over USB
  while (!Serial);
  Serial.println("Serial port connected, starting PLANE avionics setup...");
  
  servoRoll.attach(ServoRollPin, 500, 2500);
  servoRoll.write(ServoRollCenter);

  servoPitch.attach(ServoPitchPin, 500, 2500);
  servoPitch.write(ServoPitchCenter);

  servoYaw.attach(ServoYawPin, 500, 2500);
  servoYaw.write(ServoYawCenter);

  motor1.attach(MotorPin, 1000, 2000);
  motor1.write(MotorInitThrottle);

  // Turn on by default, use buttons to turn on and off
  digitalWrite(NavLedsPin, HIGH);

  if (!radio.begin())
  {
    Serial.println(F("Radio hardware is not responding!!"));
    radio.printPrettyDetails();

    // hold in infinite loop if radio is not responding
    while (1);
  }

  Serial.println(F("Radio hardware is responding!!"));

  // Configure radio
  radio.setPALevel(RF24_PA_LOW);
  //radio.setDataRate(RF24_250KBPS);

  Serial.print("Radio channel is set to: ");
  Serial.println(radio.getChannel());
  Serial.print("Radio data rate is set to: ");
  Serial.println(radio.getDataRate());

  radio.openReadingPipe(0, RadioAddress);
  radio.startListening();
}

void loop()
{
  static unsigned long loopStart = millis();
  static unsigned long lastSignalTestTime = millis();
  static unsigned long lastRadioMsgReceivedMS = millis();
  static bool inSafetyShutdownMode = false;

  unsigned long nowMs = millis();
  if (nowMs > lastSignalTestTime + 30000)
  {
    lastSignalTestTime = nowMs;
    bool goodSignal = radio.testRPD();
    Serial.println(goodSignal ? "Strong signal > 64dBm" : "Weak signal < 64dBm" );
  }

  if (radio.available())
  {
    Commands cmd;
    radio.read(&cmd, sizeof(Commands));

    lastRadioMsgReceivedMS = millis();
    inSafetyShutdownMode = false;
    
    Serial.print("Roll: ");
    Serial.println(cmd.roll);
    servoRoll.write(cmd.roll);

    Serial.print("Pitch: ");
    Serial.println(cmd.pitch);
    servoPitch.write(cmd.pitch);

    Serial.print("Yaw: ");
    Serial.println(cmd.yaw);
    servoYaw.write(cmd.yaw);

    Serial.print("Throttle: ");
    Serial.println(cmd.throttle);
    motor1.write(cmd.throttle);
  }

  // Safety shutdown of motor if lost connectiviy to ground
  if (!inSafetyShutdownMode && (millis() > lastRadioMsgReceivedMS + LastRadioMessageReceivedTimeoutMS))
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

  delay(25);
}
