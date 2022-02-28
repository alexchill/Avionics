#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(6, 7);

const byte address[6] = "00001";

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    // some boards need to wait to ensure access to serial over USB
  }

  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {} // hold in infinite loop
  }

  Serial.println(F("radio hardware is responding!!"));

  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  //radio.setDataRate(RF24_250KBPS);
  radio.stopListening();
}

void loop() {
  const char text[] = "Hi";
  if (radio.write(&text, sizeof(text))) {
    Serial.println("transmitted");
  }
  else
  {
    Serial.println("fail");
  }

  delay(5000);
}
