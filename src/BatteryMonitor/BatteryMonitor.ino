const int BATT_MEAS_PIN = 0;

const float MAX_MEAS_VOLT = 5.0; // max voltage arduino analog input will handle
const int AIN_ADC_BITS = 10;     // analog number analog/digital convert number of bits
const float AIN_VOLT_PER_UNIT = MAX_MEAS_VOLT / pow(2, AIN_ADC_BITS);

const float BATT_VOLT_MAX = 7;

const float BATT_MEAS_R1 = 9.89e3;
const float BATT_MEAS_R2 = 9.83e3;
const float BATT_MEAS_RA = 100e6;

// total resistance of resistor 2 and analog input impedance in parallel
const float BATT_MEAS_R2T = (BATT_MEAS_RA * BATT_MEAS_R2) / (BATT_MEAS_RA - BATT_MEAS_R2);
const float BATT_MEAS_DIV = (BATT_MEAS_R1 / BATT_MEAS_R2T) + 1;
const float BATT_MEAS_MAX_CURRENT = BATT_VOLT_MAX / (BATT_MEAS_R1 + BATT_MEAS_R2T);

void setup()
{
  Serial.begin(9600);
  while (!Serial)
    ;

  checkBattMeasAnalogInVoltage();

  //  Serial.print("volts/unit: ");
  //  Serial.println(AIN_VOLT_PER_UNIT, 5);
  //
  //  Serial.print("BATT_MEAS_R2T: ");
  //  Serial.println(BATT_MEAS_R2T, 5);
  //
  //  Serial.print("BATT_MEAS_DIV: ");
  //  Serial.println(BATT_MEAS_DIV, 5);
}

void loop()
{
  float battVolt = measureBattVolt();
  Serial.print("Battery Voltage: ");
  Serial.println(battVolt);

  delay(5000);
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