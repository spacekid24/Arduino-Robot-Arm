#include <Servo.h>
#include <Stepper.h>

const int steps_per_rev = 2048;
Stepper base{steps_per_rev, 7, 12, 8, 13};
Servo servo_shoulder;
Servo servo_shoulder2;
Servo servo_elbow;
Servo servo_claw;
const int x_pin1 = A0;
const int y_pin1 = A1;
const int sw_pin1 = 2;
const int x_pin2 = A2;
const int y_pin2 = A3;
const int sw_pin2 = 4;
int x_neutral1;
int y_neutral1;
int x_neutral2;
int y_neutral2;
int degree1;
int degree2;
int degree3;
int degree4;

void setup() {
  Serial.begin(9600);
  pinMode(sw_pin1, INPUT);
  digitalWrite(sw_pin1, HIGH);
  pinMode(sw_pin2, INPUT);
  digitalWrite(sw_pin2, HIGH);
  
  x_neutral1 = analogRead(x_pin1);
  y_neutral1 = analogRead(y_pin1);

  x_neutral2 = analogRead(x_pin2);
  y_neutral2 = analogRead(y_pin2);

  servo_elbow.attach(5);
  servo_shoulder.attach(3);
  servo_shoulder2.attach(6);
  servo_claw.attach(9);

  degree1 = 0;
  degree2 = 90;
  degree3 = 0;
  degree4 = 0;
}

void loop() {
  int x_data1 = analogRead(x_pin1);
  int y_data1 = analogRead(y_pin1);
  int sw_data1 = digitalRead(sw_pin1);

  int x_data2 = analogRead(x_pin2);
  int y_data2 = analogRead(y_pin2);
  int sw_data2 = digitalRead(sw_pin2);

  turnBase(base, x_data1, x_neutral1);
  turnShoulder(servo_shoulder, servo_shoulder2, degree2, y_data1, y_neutral1);
  turnServo(servo_elbow, degree1, y_data2, y_neutral2);
  turnServo(servo_claw, degree3, x_data2, x_neutral2);
  // turnServo(servo_elbow, degree2, y_data2, y_neutral2);
  // turnServo(servo_wrist, degree3, x_data2, x_neutral2);
  // controlClaw(servo_claw, degree4, sw_data2);

  delay(20);
}

void turnBase(Stepper &stepper, int data, int neutral) {
  stepper.setSpeed(10);

  if (data > 1000) {
    stepper.step(1);
  }

  if (data < 50) {
    stepper.step(-1);
  }
}

void turnShoulder(Servo &servo1, Servo &servo2, int &degree, int data, int neutral) { //control two servos for the shoulder joint (one servo does opposite degree of the other)
  if (data > 1000) {
    if (degree <= 179) {                                                                            //if both motors do not have same range of motion, try mapping them to the same range of values
      servo1.write(degree + 1);
      servo2.write(180 - degree + 1);
      degree = degree + 1;
    }
    else {
      servo1.write(degree);
      servo2.write(180 - degree);
    }
  }

  else if (data < 50) {
    if (degree >= 1) {
      servo1.write(degree - 1);
      servo2.write(180 - degree - 1);
      degree = degree - 1;
    }
    
    else {
      servo1.write(degree);
      servo2.write(180 - degree);
    }
  }
}

void turnServo(Servo &servo, int &degree, int data, int neutral) {
  if (data > 1000) {
    if (degree <= 179) {
      servo.write(degree+1);
      degree = degree + 1;
    }
    else{
      servo.write(degree);
    }
  }

  else if (data < 50) {
    if (degree >= 1) {
      servo.write(degree-1);
      degree = degree - 1;
    }
    else{
      servo.write(degree);
    }
  }

  else {
    servo.write(degree);
  } 
}

// void controlClaw(Servo &servo, int &degree, int switch_data) {
//   if ((switch_data == 0) && (degree == 0)) {
//     degree = 180;
//     servo.write(degree);
//   }

//   else if ((switch_data == 0) && (degree == 180)) {
//     degree = 0;
//     servo.write(degree);
//   }
// }