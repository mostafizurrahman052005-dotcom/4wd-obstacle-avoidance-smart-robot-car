#include <ESP32Servo.h>

const int ENA = 14;
const int IN1 = 27;
const int IN2 = 26;

const int IN3 = 25;
const int IN4 = 33;
const int ENB = 32;

const int TRIG_PIN = 5;
const int ECHO_PIN = 18;

const int SERVO_PIN = 13;

const int OBSTACLE_DIST = 25;
const int MOTOR_SPEED = 115;
const int TURN_SPEED = 215;

Servo myServo;

void setup() {
  Serial.begin(115200);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  myServo.attach(SERVO_PIN);
  myServo.write(90);
  delay(500);
}

void loop() {
  int frontDistance = getDistance();

  if (frontDistance > OBSTACLE_DIST || frontDistance == 0) {
    moveForward();
  } 
  else {
    stopCar();
    delay(200);

    myServo.write(20);
    delay(500);
    int rightDistance = getDistance();

    myServo.write(160);
    delay(500);
    int leftDistance = getDistance();

    myServo.write(90);
    delay(300);

    if (rightDistance > leftDistance && rightDistance > OBSTACLE_DIST) {
      turnRight();
      delay(450);
      stopCar();
    } 
    else if (leftDistance >= rightDistance && leftDistance > OBSTACLE_DIST) {
      turnLeft();
      delay(450);
      stopCar();
    } 
    else {
      moveBackward();
      delay(400);
      turnRight();
      delay(600);
      stopCar();
    }
  }
  delay(50);
}

int getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) return 250;
  int distance = duration * 0.034 / 2;
  return distance;
}

void moveForward() {
  analogWrite(ENA, MOTOR_SPEED);
  analogWrite(ENB, MOTOR_SPEED);
  
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveBackward() {
  analogWrite(ENA, MOTOR_SPEED);
  analogWrite(ENB, MOTOR_SPEED);
  
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnRight() {
  analogWrite(ENA, TURN_SPEED);
  analogWrite(ENB, TURN_SPEED);
  
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnLeft() {
  analogWrite(ENA, TURN_SPEED);
  analogWrite(ENB, TURN_SPEED);
  
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void stopCar() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}