#include <Servo.h>

// ---------- PINS ----------
const int IR1 = 2;   // Child front (low height)
const int IR2 = 3;   // Adult front (high height)
const int IR3 = 4;   // Child back (low height)
const int IR4 = 5;   // Adult back (high height)

const int servoPin1 = 9;
const int servoPin2 = 10;
const int espNotify = 8;
const int buzzerPin = 7;  // Buzzer pin

// ---------- SERVO POSITIONS ----------
const int CLOSED_POS_SERVO1 = 180;
const int CLOSED_POS_SERVO2 = 0;
const int OPEN_POS_SERVO1   = 90;
const int OPEN_POS_SERVO2   = 90
;

// ---------- TIMING ----------
const unsigned long SENSOR_SETTLE_DELAY = 350;  // Wait 3000ms to check adult sensor
const unsigned long DEBOUNCE = 500;             // Prevent repeated triggers
const unsigned long BUZZER_DURATION = 1000;     // Buzzer on for 1 seconds

// ---------- STATE ----------
Servo servo1;
Servo servo2;

enum DoorState { CLOSED, OPEN };
DoorState doorState = CLOSED;

bool prevIR1 = HIGH, prevIR2 = HIGH, prevIR3 = HIGH, prevIR4 = HIGH;
unsigned long lastActionTime = 0;

void setup() {
  Serial.begin(115200); 
  pinMode(IR1, INPUT_PULLUP);
  pinMode(IR2, INPUT_PULLUP);
  pinMode(IR3, INPUT_PULLUP);
  pinMode(IR4, INPUT_PULLUP);

  pinMode(espNotify, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  
  digitalWrite(espNotify, LOW);
  digitalWrite(buzzerPin, LOW);

  servo1.attach(servoPin1);
  servo2.attach(servoPin2);

  servo1.write(CLOSED_POS_SERVO1);
  servo2.write(CLOSED_POS_SERVO2);

  Serial.println("✅ Arduino Ready - Smart Child Safety Door");
}

void loop() {
  unsigned long now = millis();

  int s1 = digitalRead(IR1);
  int s2 = digitalRead(IR2);
  int s3 = digitalRead(IR3);
  int s4 = digitalRead(IR4);

  // Debounce - stay in current state
  if (now - lastActionTime < DEBOUNCE) {
    prevIR1 = s1; prevIR2 = s2; prevIR3 = s3; prevIR4 = s4;
    delay(10);
    return;
  }

  // ========== FRONT SENSORS ==========
  // ✅ Check if BOTH IR1 and IR2 trigger simultaneously
  if (s1 == LOW && prevIR1 == HIGH && s2 == LOW) {
    Serial.println("👨‍👦 Both sensors triggered simultaneously (IR1 + IR2) → Door OPEN");
    openDoor();
    triggerCamera();
    lastActionTime = now;
    prevIR1 = s1; prevIR2 = s2; prevIR3 = s3; prevIR4 = s4;
    return;
  }

  // Trigger ONLY when child sensor (IR1) goes from HIGH to LOW
  if (s1 == LOW && prevIR1 == HIGH) {
    
    Serial.println("🔍 IR1 (child front) triggered - checking for adult...");
    
    // WAIT 250ms to see if adult sensor also triggers
    delay(SENSOR_SETTLE_DELAY);
    
    // Check if IR2 (adult sensor) is NOW triggered
    s2 = digitalRead(IR2);
    
    if (s2 == LOW) {
      // IR2 triggered during the 250ms window = ADULT
      Serial.println("👤 Adult detected (IR2 triggered within 350ms) → Door OPEN");
      openDoor();
    } else {
      // IR2 NOT triggered = CHILD ALONE
      Serial.println("🧒 Child alone (only IR1, no IR2) → Door CLOSED");
      closeDoor();
      triggerBuzzer();  // ✅ Buzzer rings AFTER 250ms check
    }
    
    triggerCamera();
    lastActionTime = now;
    prevIR1 = s1; prevIR2 = s2; prevIR3 = s3; prevIR4 = s4;
    return;
  }

  // IR2 (adult front) triggered ALONE - no child sensor first
  if (s2 == LOW && prevIR2 == HIGH && s1 == HIGH) {
    Serial.println("👤 Adult only detected (IR2 front, no IR1) → Door OPEN");
    openDoor();
    triggerCamera();
    lastActionTime = now;
    prevIR1 = s1; prevIR2 = s2; prevIR3 = s3; prevIR4 = s4;
    return;
  }

  // ========== BACK SENSORS ==========
  // ✅ Check if BOTH IR3 and IR4 trigger simultaneously
  if (s3 == LOW && prevIR3 == HIGH && s4 == LOW) {
    Serial.println("👨‍👦 Both sensors triggered simultaneously (IR3 + IR4) → Door OPEN");
    openDoor();
    triggerCamera();
    lastActionTime = now;
    prevIR1 = s1; prevIR2 = s2; prevIR3 = s3; prevIR4 = s4;
    return;
  }

  // Trigger ONLY when child sensor (IR3) goes from HIGH to LOW
  if (s3 == LOW && prevIR3 == HIGH) {
    
    Serial.println("🔍 IR3 (child back) triggered - checking for adult...");
    
    // WAIT 250ms to see if adult sensor also triggers
    delay(SENSOR_SETTLE_DELAY);
    
    // Check if IR4 (adult sensor) is NOW triggered
    s4 = digitalRead(IR4);
    
    if (s4 == LOW) {
      // IR4 triggered during the 250ms window = ADULT
      Serial.println("👤 Adult detected (IR4 triggered within 350ms) → Door OPEN");
      openDoor();
    } else {
      // IR4 NOT triggered = CHILD ALONE
      Serial.println("🧒 Child alone (only IR3, no IR4) → Door CLOSED");
      closeDoor();
      triggerBuzzer();  // ✅ Buzzer rings AFTER 250ms check
    }
    
    triggerCamera();
    lastActionTime = now;
    prevIR1 = s1; prevIR2 = s2; prevIR3 = s3; prevIR4 = s4;
    return;
  }

  // IR4 (adult back) triggered ALONE - no child sensor first
  if (s4 == LOW && prevIR4 == HIGH && s3 == HIGH) {
    Serial.println("👤 Adult only detected (IR4 back, no IR3) → Door OPEN");
    openDoor();
    triggerCamera();
    lastActionTime = now;
    prevIR1 = s1; prevIR2 = s2; prevIR3 = s3; prevIR4 = s4;
    return;
  }

  prevIR1 = s1; prevIR2 = s2; prevIR3 = s3; prevIR4 = s4;
  delay(10);
}

// ---------- DOOR CONTROL ----------
void openDoor() {
  if (doorState == OPEN) {
    Serial.println("🚪 Already OPEN - staying open");
    return;
  }
  servo1.write(OPEN_POS_SERVO1);
  servo2.write(OPEN_POS_SERVO2);
  doorState = OPEN;
  Serial.println("🚪✅ Door OPENED");
}

void closeDoor() {
  if (doorState == CLOSED) {
    Serial.println("🚪 Already CLOSED - staying closed");
    return;
  }
  servo1.write(CLOSED_POS_SERVO1);
  servo2.write(CLOSED_POS_SERVO2);
  doorState = CLOSED;
  Serial.println("🚪🔒 Door CLOSED");
}

void triggerCamera() {
  Serial.println("📸 Triggering ESP32 camera");
  digitalWrite(espNotify, HIGH);
  delay(100);
  digitalWrite(espNotify, LOW);
}

void triggerBuzzer() {
  Serial.println("🔔 BUZZER ON - Child detected alone!");
  digitalWrite(buzzerPin, HIGH);
  delay(BUZZER_DURATION);
  digitalWrite(buzzerPin, LOW);
  Serial.println("🔕 BUZZER OFF");
}
