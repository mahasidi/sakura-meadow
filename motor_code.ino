#include <Servo.h>

// === PIN DEFINITIONS ===
#define BIRD_SERVO_PIN 9
#define FLOWER_SERVO_PIN 10

// === SERVO ANGLES ===
// Bird positions
#define OPEN_POSITION 0
#define BLOCK_POSITION 170

// Flower positions
#define FLOWER_OPEN_ANGLE 0
#define FLOWER_CLOSED_ANGLE 180

// === TIMING SETTINGS ===
#define MIN_WAIT_TIME_MS 300
#define MAX_WAIT_TIME_MS 600

unsigned long timeToMoveBird = 0;
unsigned long timeToMoveFlower = 0;
unsigned long birdInterval = 1000;
unsigned long flowerInterval = 1000;

// === STATE TRACKING ===
bool birdOut = true;
bool flowerOpen = false;

Servo birdServo;
Servo flowerServo;

void setup() {
  Serial.begin(9600);

  birdServo.attach(BIRD_SERVO_PIN);
  flowerServo.attach(FLOWER_SERVO_PIN);

  birdServo.write(OPEN_POSITION);         // Start with bird open
  flowerServo.write(FLOWER_CLOSED_ANGLE); // Start with flower closed

  timeToMoveBird = millis() + birdInterval;
  timeToMoveFlower = millis() + flowerInterval;
}

void loop() {
  if (millis() >= timeToMoveBird) {
    moveBird();
    timeToMoveBird = millis() + birdInterval;
  }

  if (millis() >= timeToMoveFlower) {
    moveFlower();
    timeToMoveFlower = millis() + flowerInterval;
  }

  delay(random(MIN_WAIT_TIME_MS, MAX_WAIT_TIME_MS + 1));
}

void moveBird() {
  int blockChance = random(0, 100);
  int start = birdOut ? OPEN_POSITION : BLOCK_POSITION;
  int end = (blockChance < 40) ? OPEN_POSITION : BLOCK_POSITION;

  Serial.println(end == OPEN_POSITION ? "Bird moves to BLOCK the marble"
                                      : "Bird moves to OPEN the path");

  for (int angle = start; angle != end; angle += (end > start ? 1 : -1)) {
    birdServo.write(angle);
    delay(5);
  }

  birdServo.write(end);
  birdOut = (end == OPEN_POSITION);
}

void moveFlower() {
  int openChance = random(0, 100);
  int start = flowerOpen ? FLOWER_OPEN_ANGLE : FLOWER_CLOSED_ANGLE;
  int end = (openChance < 80) ? FLOWER_CLOSED_ANGLE : FLOWER_OPEN_ANGLE;

  Serial.println(end == FLOWER_OPEN_ANGLE ? "Flower randomly opens"
                                          : "Flower randomly closes");

  for (int angle = start; angle != end; angle += (end > start ? 1 : -1)) {
    flowerServo.write(angle);
    delay(5);
  }

  flowerServo.write(end);
  flowerOpen = (end == FLOWER_OPEN_ANGLE);
}
