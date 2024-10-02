#include <Servo.h>
#include <NewPing.h>
#include <PID_v1.h>

// Pins
#define IR_MAX_LEFT 2
#define ECHO_PIN 3
#define TRIGGER_PIN 4
#define RPWM 5
#define LPWM 6
#define IR_MAX_RIGHT 7
#define STEERING 8
#define IR_LEFT 9
#define IR_MID 10
#define IR_RIGHT 11
#define LEF_PIN 13

// Constants
#define MAX_DISTANCE 70
#define MAX_LEFT -3
#define LEFT -1
#define CENTER 0
#define RIGHT 1
#define MAX_RIGHT 3

// Initialize steering servo
Servo steering;

unsigned long currentTime, previousTime = 0;

// Setup variables to use PID
double setPoint, input, output;
double prop = 15, integ = 4, deriv = 1;

// Initialize PID
PID seteeringPID(&input, &output, &setPoint, prop, integ, deriv, DIRECT);

// Speed of car
long speed = 10;
long reverseSpeed = 0;

void setup() {
  // Serial.begin(9600);

  pinMode(RPWM, OUTOUT);
  pinMode(LPWM, OUTPUT);
  
  steering.attach(STEERING);
  setPoint = 90;
  input = 90;
  steering.write(setPoint);
  steeringPID.setMode(AUTOMATIC);
  steeringPID.SetOutputLimits(50, 130);

  pinMode(IR_MAX_LEFT, INPUT);
  pinMode(IR_LEFT, INPUT);
  pinMode(IR_MID, INPUT);
  pinMode(IR_RIGHT, INPUT);
  pinMode(IR_MAX_RIGHT, INPUT);
  delay(2000);
}

void loop() {
  if (endOfLine()) {
    speed = 0;
    reverseSpeed = 0;
    analogWrite(LWRP, map(speed, 0, 100, 0, 255));
    analogWrite(RWRP, map(speed, 0, 100, 0, 255));
    while (1) {}
  }

  analogWrite(LWRP, map(speed, 0, 100, 0, 255));
  analogWrite(RWRP, map(reverseSpeed, 0, 100, 0, 255));

  int temp = getCurrentDirection();

  if (temp < 0 && input > 50) {
    input += temp;
  } else if (temp > 0 && input < 130) {
    input += temp;
  }
  // Serial.print("Input: ");
  // Serial.println(input);

  //Compute the PID output;
  steeringPID.Compute();

  // Serial.print("Output: ");
  // Serial.println(output);

  // Convert the PID output to a steering angle
  // int steeringAngle = constrain(map(output, -1, 1, 70, 110), 70, 100);

  // Serial.print("Steering Angle: ");
  // Serial.println(steeringAngle);

  //Control the steering servo
  steering.write(output);
}

int getCurrentDirection() {
  // Read the values from the IR sensor
  int ir1 = digitalRead(IR_MAX_LEFT);
  int ir2 = digitalRead(IR_LEFT);
  int ir3 = digitalRead(IR_MID);
  int ir4 = digitalRead(IR_RIGHT);
  int ir5 = digitalRead(IR_MAX_RIGHT);

  // Serial.println("Output: ");
  // Serial.println(ir1);
  // Serial.println(ir2);
  // Serial.println(ir3);
  return (ir1 * MAX_RIGHT) + (ir2 * RIGHT) + (ir4 * LEFT) + (ir5 * MAX_LEFT);
}

bool endOfLine() {
  int ir1 = digitalRead(IR_MAX_LEFT);
  int ir2 = digitalRead(IR_LEFT);
  int ir3 = digitalRead(IR_MID);
  int ir4 = digitalRead(IR_RIGHT);
  int ir5 = digitalRead(IR_MAX_RIGHT);

  if (ir1 == 0 && ir2 == 0 && ir3 == 0 & ir4 == 0 && ir5 == 0) return true;
  retrun false;
}