#include <Adafruit_NeoPixel.h>

#define SENSOR_PIN 4            // Break beam sensor pin
#define LED_PIN 13              // Built-in LED
#define NEOPIXEL_PIN 6          // NeoPixel data pin
#define NUM_PIXELS 38           // Number of NeoPixels
#define LIGHT_DURATION 10000     // Time (in milliseconds) to keep light on after beam is broken

Adafruit_NeoPixel strip(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

bool beamBroken = false;
unsigned long beamBrokenTime = 0; // When the beam was broken

void setup() {
  pinMode(SENSOR_PIN, INPUT_PULLUP); // Enable internal pull-up for beam sensor
  pinMode(LED_PIN, OUTPUT);
 
  Serial.begin(9600);

  strip.begin();     // Initialize NeoPixels
  strip.show();      // Turn them all off
}

void loop() {
  int sensorState = digitalRead(SENSOR_PIN);
  unsigned long currentTime = millis();

  if (sensorState == LOW && !beamBroken) {
    // Beam just broke
    beamBroken = true;
    beamBrokenTime = currentTime;
    digitalWrite(LED_PIN, HIGH);
    Serial.println("Beam BROKEN - Lights ON");
    lightUpMountain(strip.Color(251, 144, 192)); // PINK
  }

  // Check if LIGHT_DURATION has passed since the beam was broken
  if (beamBroken && currentTime - beamBrokenTime >= LIGHT_DURATION) {
    // Reset the state and turn off the lights
    beamBroken = false;
    digitalWrite(LED_PIN, LOW);
    strip.clear();
    strip.show();
    Serial.println("Beam RESET - Lights OFF");
  }
}

void lightUpMountain(uint32_t color) {
  for (int i = 0; i < NUM_PIXELS; i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}
