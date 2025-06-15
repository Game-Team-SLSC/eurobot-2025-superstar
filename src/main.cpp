#include <Arduino.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <Servo.h>

#define IS_TEST
#define COLOR_SW 6
#define EN 8
#define STEPS_PER_REV 200.f
#define DIAMETER 53.25f

const float STEPS_PER_MM = STEPS_PER_REV / (3.14159265358979323846f * DIAMETER); // 200 steps per revolution, 53.25 mm diameter wheel

// EG X-Y position bed driven by 2 steppers
// Alas its not possible to build an array of these with different pins for each :-(
AccelStepper stepper1(AccelStepper::DRIVER, 2, 5);
AccelStepper stepper2(AccelStepper::DRIVER, 4, 7);

Servo servo;

void runServo() {
  for(;;) {
    servo.write((sin(millis() / 300.f) + 1.f) * 90.f);
    delay(20);
  }
}

void goTo(float left, float right) {
  stepper1.moveTo(left * STEPS_PER_MM);
  stepper2.moveTo(-right * STEPS_PER_MM);

  while (stepper1.isRunning() | stepper2.isRunning()) {
    stepper1.run();
    stepper2.run();
  }

  stepper1.setCurrentPosition(0);
  stepper2.setCurrentPosition(0);
  
  //steppers.runSpeedToPosition(); // Blocks until all are in position
}

void turnLeft() {
  goTo(170, 0);
}

void turnRight() {
  goTo(0, 170);
}

void runBlue() {
  digitalWrite(EN, LOW);
  goTo(1300, 1300); // go up to the ramp
  turnRight();
  goTo(200, 200);
  goTo(-408, -408);
  runServo();
}

void runYellow() {
  digitalWrite(EN, LOW);
  goTo(1300, 1300); // go up to the ramp
  turnLeft();
  goTo(200, 200);
  goTo(-408, -408);
  runServo();
}

void setup() {
  pinMode(COLOR_SW, INPUT_PULLUP);
  pinMode(EN, OUTPUT);
  Serial.begin(9600);

  // Configure each stepper
  stepper1.setMaxSpeed(600);
  stepper2.setMaxSpeed(600);

  servo.attach(3);
  
  servo.write(90);

  #ifdef IS_TEST
  delay(2000);
  #else
  delay(87000);
  #endif

  if (digitalRead(COLOR_SW)) {
    stepper1.setAcceleration(600);
    stepper2.setAcceleration(600);
    runYellow();
  } else {
    stepper1.setAcceleration(900);
    stepper2.setAcceleration(900);
    runBlue();
  }
}

void loop() {
  
}
