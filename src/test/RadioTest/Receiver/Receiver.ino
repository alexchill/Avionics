#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//RF24 radio(3, 2); // Mega
RF24 radio(6, 7); // Uno

const byte address[6] = "00001";

void setup()
{
    Serial.begin(115200);
    while (!Serial) {
        // some boards need to wait to ensure access to serial over USB
    }

    if (!radio.begin()) {
        Serial.println(F("radio hardware is not responding!!"));
        while (1) {} // hold in infinite loop
    }

    Serial.println(F("radio hardware is responding!!"));

    radio.openReadingPipe(0, address);
    radio.setPALevel(RF24_PA_LOW);
    // radio.setDataRate(RF24_250KBPS);
    radio.startListening();
}

void loop()
{
    if (radio.available())
    {
        char text[32] = "";
        radio.read(&text, sizeof(text));
        Serial.println(text);
    }

    delay(100);
}
