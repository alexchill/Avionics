#include <Servo.h>


Servo servo1;       //wrist

//define initial positions of servos
const int init1 = 30;

//define the increment in degrees. This is what we will use to move the servos
int inc = 2;

//define the delay between each iteration of movement. I will call it speed_ for simplicity
int speed_ = 0;

//define the initial postions of the servos
int pos1 = init1;

//power on/off (optional)
int powerPin = 13;


void setup() {
  Serial.begin(9600);

  //attaching servos to pins of arduino
  servo1.attach(powerPin, 1000, 2000);

  //begin at initial positions
  servo1.write(pos1);

  //(optional) power again
  pinMode(powerPin, OUTPUT);
}

void loop() {

  //possibly not needed?
  Serial.flush();

  //val is the variable we store the received data in
  int val = Serial.read();
  bool input;

  if (val == -1) {
    input = false;
    delay(50);
    return;
    }

  //condtional needed for loops to work properly
  int i = 1;
  
  //--------------------------------MOVE servo5 METHOD-----------------------------------------
  while (val == 'w' && i == 1) {
    val = Serial.read();
    Serial.println(val);

    input = true;
    //limitations that I put in so I can't break the servo cords
    if (pos1 + inc <= 170) {

      //increment position to move servo
      pos1 = pos1 + inc;
      servo1.write(pos1);

      //used to break out of the loop and move different servo or do nothing
      if (val != 'w') {
        i = 0;
      }
      delay(speed_);
    }
  }

  if (input == false) {
    Serial.println("fail");
  }

  while (val == 's' && i == 1) {
    val = Serial.read();

    //limitations that I put in so I can't break the servo cords
    if (pos1 - inc >= 10) {

      //increment position to move servo
      pos1 = pos1 - inc;
      servo1.write(pos1);

      //used to break out of the loop and move different servo or do nothing
      if (val != 's') {
        i = 0;
      }
      delay(speed_);
    }
  }
}
