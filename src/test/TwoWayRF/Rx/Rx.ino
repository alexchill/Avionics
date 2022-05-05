// nRF24L01 communication 2 ways Arduino B

#include <SPI.h>
#include <printf.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(3, 2); // Mega
//RF24 radio(6, 7); // Uno
const byte addresses[][6] = {"00001", "00002"}; // Setting the two addresses. One for transmitting and one for receiving

int ackData[2] = { -1, -1};

void setup()
{
  Serial.begin(115200);
  printf_begin();
  while (!Serial);

  radio.begin();                          // Starting the radio communication
  // radio.openWritingPipe(addresses[1]);    // Setting the address at which we will send the data
  radio.openReadingPipe(1, addresses[0]); // Setting the address at which we will receive the data
  radio.enableAckPayload();
  radio.setPALevel(RF24_PA_MIN);          // You can set it as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.writeAckPayload(1, &ackData, sizeof(ackData)); // pre-load data

  Serial.println(sizeof(float));

  radio.printPrettyDetails();
}

void loop()
{
  static unsigned long LoopCount = 0;
  char buf[80];
  buf[0] = '\0';

  delay(5);
  radio.startListening();
  if (radio.available())
  {
    while (radio.available())
    {
      radio.read(&buf, sizeof(buf));
      Serial.print("Rx read " );
      Serial.println(buf);
      ackData[0] = LoopCount++;
      ackData[1] = LoopCount;
      radio.writeAckPayload(1, &ackData, sizeof(ackData)); // load the payload for the next time      
    }
  }

//  if (LoopCount++ % 40 == 0)
//  {
//    strcat(buf, "from the receiver");
//
//    delay(5);
//    radio.stopListening();
//    if (radio.write(&buf, sizeof(buf)))
//    {
//      Serial.print("Rx sent: ");
//      Serial.println(buf);
//    }
//    else
//    {
//      Serial.print("Rx transmit FAILED to send: ");
//      Serial.println(buf);
//    }
//  }

  delay(2000);
}
