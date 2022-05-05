//nRF24L01 communication 2 ways Arduino A

#include <SPI.h>
#include <printf.h>
#include <nRF24L01.h>
#include <RF24.h>

//RF24 radio(3, 2); // Mega
RF24 radio(6, 7); // Uno
const byte addresses [][6] = {"00001", "00002"};  //Setting the two addresses. One for transmitting and one for receiving

int ackData[2] = { -1, -1};

void setup() {
  Serial.begin(115200);
  printf_begin();
  while (!Serial);

  radio.begin();                           //Starting the radio communication
  radio.enableAckPayload();
  radio.setRetries(5, 5); // delay, count
  radio.openWritingPipe(addresses[0]);     //Setting the address at which we will send the data
  // radio.openReadingPipe(1, addresses[1]);  //Setting the address at which we will receive the data
  radio.setPALevel(RF24_PA_MIN); //You can set it as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.printPrettyDetails();
}

void loop()
{
  const char buf[] = "Hello ";
  char buf2[80];

  delay(5);
  radio.stopListening();
  if (radio.write(&buf, sizeof(buf)))
  {
    Serial.print("Tx sent: ");
    Serial.println(buf);
    if (radio.isAckPayloadAvailable()) {
      radio.read(&ackData, sizeof(ackData));
      Serial.print("Ack int1: ");
      Serial.print(ackData[0]);
      Serial.print(", int2: ");
      Serial.println(ackData[1]);      
    }
    else {
      Serial.println("Acknowledge but no data ");
    }
  }
  else
  {
    Serial.println("Tx transmit failed");
  }

  //  delay(5);
  //
  //  radio.startListening();
  //  if (radio.available())
  //  {
  //    while (radio.available())
  //    {
  //      radio.read(&buf2, sizeof(buf2));
  //      Serial.print("Tx read: ");
  //      Serial.println(buf2);
  //    }
  //  }

  delay(2000);
}
