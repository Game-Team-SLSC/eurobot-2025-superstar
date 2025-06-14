#include <Arduino.h>
#include <AccelStepper.h>
#include <MultiStepper.h>

#define EN 8

// EG X-Y position bed driven by 2 steppers
// Alas its not possible to build an array of these with different pins for each :-(
AccelStepper stepper1(AccelStepper::DRIVER, 2, 5);
AccelStepper stepper2(AccelStepper::DRIVER, 4, 7);

void goTo(long left, long right) {
  stepper1.moveTo(left);
  Serial.println(left);
  stepper2.moveTo(-right);
  Serial.println(-right);

  while (stepper1.isRunning() & stepper2.isRunning()) {
    stepper1.run();
    stepper2.run();
  }

  stepper1.setCurrentPosition(0);
  stepper2.setCurrentPosition(0);
  
  //steppers.runSpeedToPosition(); // Blocks until all are in position
}

void run() {
  digitalWrite(EN, LOW);
  goTo(1000, 1000);
  goTo(-1000, -1000);
}

void setup() {
  pinMode(EN, OUTPUT);
  Serial.begin(9600);

  // Configure each stepper
  stepper1.setMaxSpeed(500);
  stepper2.setMaxSpeed(500);
  stepper1.setAcceleration(1000);
  stepper2.setAcceleration(1000);

  run();
  delay(3000);
}

void loop() {
  
}
