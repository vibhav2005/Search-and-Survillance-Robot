#define encoder0PinA 2    // Encoder signal A pin
#define encoder0PinB 3    // Encoder signal B pin

#define countsPerRevolution 90  // Number of counts per revolution of the encoder
#define rpmUpdateInterval 1000  // Interval in milliseconds to update RPM

volatile long encoder0Pos = 0; // Encoder position
long encoder0PosPrev = 0; // Previous encoder position
unsigned long lastUpdateTime = 0; // Last time RPM was calculated

void setup() {
  Serial.begin(9600); // Initialize serial communication
  pinMode(encoder0PinA, INPUT_PULLUP); // Use internal pull-up resistor
  pinMode(encoder0PinB, INPUT_PULLUP); // Use internal pull-up resistor
  
  lastUpdateTime = millis(); // Initialize last update time
  Serial.println("Start");
}

void loop() {
  unsigned long currentTime = millis();

  // Read the encoder count and update position
  readEncoder();

  // Check if it's time to update RPM
  if (currentTime - lastUpdateTime >= rpmUpdateInterval) {
    long encoderCountDiff = encoder0Pos - encoder0PosPrev; // Counts in the last interval

    // Calculate RPM
    float rpm = (encoderCountDiff * 60000.0) / (countsPerRevolution * rpmUpdateInterval);

    // Print RPM
    Serial.print("Counts: ");
    Serial.print(encoderCountDiff);
    Serial.print(" | RPM: ");
    Serial.println(rpm);
    
    // Update previous encoder position and time
    encoder0PosPrev = encoder0Pos;
    lastUpdateTime = currentTime;
  }
}

// Function to read the encoder count by polling the pins
void readEncoder() {
  static int lastStateA = LOW;
  static int lastStateB = LOW;

  // Read the current state of the encoder signals
  int stateA = digitalRead(encoder0PinA);
  int stateB = digitalRead(encoder0PinB);

  // Debug prints to observe encoder states
  Serial.print("State A: "); Serial.print(stateA);
  Serial.print(" | State B: "); Serial.println(stateB);

  // Determine the direction and update the position accordingly
  if (lastStateA == LOW && stateA == HIGH) {
    if (stateB == LOW) {
      encoder0Pos--; // Counter-clockwise (CCW)
    } else {
      encoder0Pos++; // Clockwise (CW)
    }
  } else if (lastStateA == HIGH && stateA == LOW) {
    if (stateB == HIGH) {
      encoder0Pos--; // Counter-clockwise (CCW)
    } else {
      encoder0Pos++; // Clockwise (CW)
    }
  }

  // Update last state variables
  lastStateA = stateA;
  lastStateB = stateB;
}
