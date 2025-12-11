/*
 * AT_RobotArmExp_V11 - ML Failure Prediction Simulator
 * Demonstrates findings from AT_RobotFailures_V9.ipynb
 * 
 * This demo physically shows ML predictions by:
 * - Performing high-risk movements identified in ML analysis
 * - Displaying predicted failure types on LCD
 * - Demonstrating failure patterns (stall, obstruction, collision)
 * 
 * NOTE ABOUT ML MAPPING:
 * The Arduino obviously can’t run my Python models, so I exported
 * a simplified list of predictions from AT_RobotFailures_V9.ipynb
 * and stored them here.
 *
 * Each entry represents:
 *  - the servo involved in a real failure case
 *  - an angle associated with abnormal behaviour
 *  - the predicted failure label
 *  - the risk level (derived from model confidence)
 *
 * The robot arm physically demonstrates the same patterns by using
 * simplified "failure motions" that match the dataset definitions.

 * Hardware: Arduino Uno + LCD1602 + PCA9685 + 4 Servos
 * LCD Pins: RS=7, E=8, D4=9, D5=10, D6=11, D7=12
 */

#include <Wire.h>
#include <LiquidCrystal.h>
#include <Adafruit_PWMServoDriver.h>

// LCD Configuration.
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

// PCA9685 Servo Driver.
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

// Channel assignments.
#define BASE_CH     0
#define SHOULDER_CH 1
#define ELBOW_CH    2
#define GRIP_CH     3

// Servo Parameters.
const int SERVO_MIN_PULSE = 150;
const int SERVO_MAX_PULSE = 600;

// ML failure predictions taken directly from my dataset work.
struct Prediction {
  int channel;
  int angle;
  const char* failureType;
  const char* riskLevel;
};

// These movements match the problem cases I found in my notebook:
// extreme angles, fast changes, and the two servos that failed the most.
Prediction predictions[] = {
  {SHOULDER_CH, 135, "SHOULDER_STALL", "HIGH RISK"},
  {ELBOW_CH, 20, "ELBOW_JITTER", "MEDIUM RISK"},
  {BASE_CH, 160, "OVER_TRAVEL", "HIGH RISK"},
  {SHOULDER_CH, 15, "OBSTRUCTION", "MEDIUM RISK"},
  {ELBOW_CH, 165, "COLLISION", "HIGH RISK"},
  {GRIP_CH, 180, "TOOL_FAILURE", "LOW RISK"},
  {SHOULDER_CH, 90, "NORMAL", "SAFE"},
  {ELBOW_CH, 90, "NORMAL", "SAFE"}
};

const int NUM_PREDICTIONS = sizeof(predictions) / sizeof(predictions[0]);

// Degree → Pulse mapping.
int angleToPulse(int ang) {
  return map(ang, 0, 180, SERVO_MIN_PULSE, SERVO_MAX_PULSE);
}

// Release torque (cut power) on a servo.
void powerOff(int channel) {
  pwm.setPWM(channel, 0, 4096);
}

// Release all servos except the one being moved
void isolateServo(int activeChannel) {
  int channels[4] = {BASE_CH, SHOULDER_CH, ELBOW_CH, GRIP_CH};
  for (int i = 0; i < 4; i++) {
    if (channels[i] != activeChannel) {
      powerOff(channels[i]);
    }
  }
}

// Move servo with isolation.
void moveServo(int channel, int angle, int pause=800) {
  isolateServo(channel);
  pwm.setPWM(channel, 0, angleToPulse(angle));
  delay(pause);
}

// Demonstrate failure pattern based on ML prediction.
void demonstrateFailure(int channel, const char* failureType) {
  if (strcmp(failureType, "SHOULDER_STALL") == 0 || strcmp(failureType, "ELBOW_STALL") == 0) {
    // Stall: Move partway then stop unnaturally.
    int currentAngle = 90;
    int targetAngle = (channel == SHOULDER_CH) ? 135 : 20;
    
    // Move slowly toward target.
    for (int a = currentAngle; a < targetAngle - 10; a += 5) {
      moveServo(channel, a, 200);
    }
    // Stall: stop before reaching target.
    delay(1000);
  }
  else if (strcmp(failureType, "ELBOW_JITTER") == 0) {
    // Jitter: Rapid small movements.
    for (int i = 0; i < 5; i++) {
      moveServo(channel, 20, 100);
      moveServo(channel, 23, 100);
      moveServo(channel, 20, 100);
    }
  }
  else if (strcmp(failureType, "OVER_TRAVEL") == 0) {
    // Over-travel: Move to extreme angle.
    moveServo(channel, 160, 1000);
    delay(500);
  }
  else if (strcmp(failureType, "OBSTRUCTION") == 0) {
    // Obstruction: Try to move but can't reach target.
    moveServo(channel, 15, 500);
    delay(500);
    // Simulate hitting obstruction.
    moveServo(channel, 25, 300);
    moveServo(channel, 15, 300);
  }
  else if (strcmp(failureType, "COLLISION") == 0) {
    // Collision: Quick movement then sudden stop.
    moveServo(channel, 165, 300);
    delay(200);
    powerOff(channel);
  }
  else {
    // Normal: Smooth movement to target.
    moveServo(channel, 90, 800);
  }
}

void setup() {
  Serial.begin(9600);
  
  // Initialise LCD.
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("AT_RobotArmExp");
  lcd.setCursor(0, 1);
  lcd.print("ML Demo V11");
  delay(2000);
  
  // Initialise PCA9685.
  pwm.begin();
  pwm.setPWMFreq(60);
  delay(10);
  
  // Ensuring everything starts unpowered.
  powerOff(BASE_CH);
  powerOff(SHOULDER_CH);
  powerOff(ELBOW_CH);
  powerOff(GRIP_CH);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ML Prediction");
  lcd.setCursor(0, 1);
  lcd.print("Simulator Ready");
  
  Serial.println("========================================");
  Serial.println("AT_RobotArmExp_V11 - ML Failure Prediction");
  Serial.println("Demonstrating AT_RobotFailures_V9 findings");
  Serial.println("========================================");
  Serial.println("Commands:");
  Serial.println("  DEMO - Run full ML prediction demo");
  Serial.println("  NEXT - Show next prediction");
  Serial.println("  MOVE <s> <a> - Move servo s to angle a");
  Serial.println("  STATUS - Show current state");
  Serial.println("========================================");
}

int currentPrediction = 0;

void loop() {
  // Check serial commands.
  if (Serial.available() > 0) {
    String cmd = "";
    while (Serial.available() > 0) {
      char c = Serial.read();
      if (c == '\n' || c == '\r') break;
      if (c >= 32) cmd += c;
    }
    cmd.trim();
    cmd.toUpperCase();
    
    if (cmd.length() > 0) {
      Serial.print("> ");
      Serial.println(cmd);
      
      if (cmd == "DEMO" || cmd == "demo") {
        runFullDemo();
      }
      else if (cmd == "NEXT" || cmd == "next") {
        showNextPrediction();
      }
      else if (cmd.startsWith("MOVE")) {
        int firstSpace = cmd.indexOf(' ');
        int secondSpace = cmd.indexOf(' ', firstSpace + 1);
        if (firstSpace > 0 && secondSpace > 0) {
          int servo = cmd.substring(firstSpace + 1, secondSpace).toInt();
          int angle = cmd.substring(secondSpace + 1).toInt();
          if (servo >= 0 && servo < 4 && angle >= 0 && angle <= 180) {
            Serial.print("Moving servo ");
            Serial.print(servo);
            Serial.print(" to ");
            Serial.println(angle);
            moveServo(servo, angle);
            powerOff(servo);
          }
        }
      }
      else if (cmd == "STATUS" || cmd == "status") {
        Serial.println("Current prediction index: ");
        Serial.println(currentPrediction);
      }
    }
  }
  
  delay(10);
}

void showNextPrediction() {
  if (currentPrediction >= NUM_PREDICTIONS) {
    currentPrediction = 0;
  }
  
  Prediction pred = predictions[currentPrediction];
  
  // Display on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(pred.riskLevel);
  lcd.setCursor(0, 1);
  
  // Truncate if too long.
  String failureMsg = String(pred.failureType);
  if (failureMsg.length() > 16) {
    failureMsg = failureMsg.substring(0, 16);
  }
  lcd.print(failureMsg);
  
  // Serial output.
  Serial.println("\n========================================");
  Serial.print("ML Prediction #");
  Serial.println(currentPrediction + 1);
  Serial.print("Servo: ");
  Serial.println(pred.channel);
  Serial.print("Angle: ");
  Serial.println(pred.angle);
  Serial.print("Failure Type: ");
  Serial.println(pred.failureType);
  Serial.print("Risk Level: ");
  Serial.println(pred.riskLevel);
  Serial.println("========================================");
  
  // Demonstrate the failure.
  Serial.println("Demonstrating failure pattern...");
  demonstrateFailure(pred.channel, pred.failureType);
  
  // Power off.
  powerOff(pred.channel);
  
  Serial.println("Demonstration complete");
  Serial.println("Type NEXT for next prediction or DEMO for full cycle\n");
  
  currentPrediction++;
}

void runFullDemo() {
  Serial.println("\n========================================");
  Serial.println("RUNNING FULL ML PREDICTION DEMO");
  Serial.println("Demonstrating all failure types from dataset");
  Serial.println("========================================");
  
  currentPrediction = 0;
  
  for (int i = 0; i < NUM_PREDICTIONS; i++) {
    Serial.print("\n--- Prediction ");
    Serial.print(i + 1);
    Serial.print(" of ");
    Serial.println(NUM_PREDICTIONS);
    
    showNextPrediction();
    
    delay(2000);  // Pausing between demonstrations.
  }
  
  Serial.println("\n========================================");
  Serial.println("FULL DEMO COMPLETE");
  Serial.println("All ML predictions demonstrated");
  Serial.println("========================================");
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Demo Complete");
  lcd.setCursor(0, 1);
  lcd.print("Type DEMO again");
}

