#include <Vex5.h>

Vex5_Motor motor1;
Vex5_Motor motor6;

void setup() {
  Vex5.begin();
  motor1.begin(VEX5_PORT_1);
  motor6.begin(VEX5_PORT_6);
}

void loop() {
	motor1.setSpeed(2000);
  motor6.setSpeed(-2000);
}