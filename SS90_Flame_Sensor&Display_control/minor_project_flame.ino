#include <Servo.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Create the OLED display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int flameSensor1 = 12;
const int flameSensor2 = 11; // Assuming you meant 11 for the second sensor
const int flameSensor3 = 8;
const int flameSensor4 = 7;
const int water = 13;

Servo servo1;
Servo servo2;

void setup() {
  // Initialize flame sensors
  pinMode(flameSensor1, INPUT);
  pinMode(flameSensor2, INPUT);
  pinMode(flameSensor3, INPUT);
  pinMode(flameSensor4, INPUT);
  pinMode(water , OUTPUT);
  // Attach servos to PWM pins
  servo1.attach(9);
  servo2.attach(10);
  
  // Initialize servos to 0 position
  servo1.write(0);
  servo2.write(0);
  Serial.begin(9600); // For debugging

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  
  // Draw the eyes
  drawEyes();
  
  // Display the result
  display.display();
}

void loop() {
  // Read the flame sensors
  digitalWrite(water , LOW);
  
  int sensor1Value = digitalRead(flameSensor1);
  int sensor2Value = digitalRead(flameSensor2);
  int sensor3Value = digitalRead(flameSensor3);
  int sensor4Value = digitalRead(flameSensor4);
  Serial.println(sensor1Value);

  
  // Check if any sensor detects flame
  if (sensor1Value == 0) {
    // Rotate servo1 from 0 to 180
    digitalWrite(water,HIGH);
    for (int pos = 10; pos <= 120; pos += 1) {
      servo1.write(pos);
      servo2.write(130-pos);
      delay(10); // Adjust speed of rotation
    }
    
    // Rotate servo1 from 180 to 0
    for (int pos = 120; pos >= 10; pos -= 1) {
      servo1.write(pos);
      servo2.write(130-pos);
      delay(10); // Adjust speed of rotation
    }
    delay(500);
    
    // Additional action for servo2 can be added here if needed
  }
  delay(500);
}



void drawEyes() {
  int eyeX = 40;
  int eyeY = 32;
  int pupilX = eyeX;
  int pupilY = eyeY;
  
  // Left Eye
  display.fillCircle(eyeX, eyeY, 12, WHITE); // Big eye
  display.fillCircle(pupilX, pupilY, 6, BLACK);  // Small pupil
  display.fillCircle(pupilX + 3, pupilY + 3, 3, WHITE); // Small white circle touching the pupil

  // Right Eye
  eyeX = 88;
  display.fillCircle(eyeX, eyeY, 12, WHITE); // Big eye
  display.fillCircle(pupilX + 48, pupilY, 6, BLACK);  // Small pupil
  display.fillCircle(pupilX + 48 + 3, pupilY + 3, 3, WHITE); // Small white circle touching the pupil
}
