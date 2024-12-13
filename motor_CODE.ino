#include <SoftwareSerial.h>

// CNC Shield Pin Definitions
const int StepX = 2; // Step pin for Motor X
const int DirX = 5;  // Direction pin for Motor X
const int StepY = 3; // Step pin for Motor Y
const int DirY = 6;  // Direction pin for Motor Y
const int EnablePin = 8; // Enable pin for CNC shield

// Serial communication pins for ESP8266
#define rx 5
#define tx 4

// State variables to track lock/unlock status for slots
bool slot1Locked = true; // Start as locked
bool slot2Locked = true; // Start as locked
unsigned long slot1UnlockTime = 0; // Store the unlock time for Slot 1
unsigned long slot2UnlockTime = 0; // Store the unlock time for Slot 2
const unsigned long lockDelay = 10000; // 10-second delay in milliseconds

void setup() {
    // Initialize pins as outputs
    pinMode(StepX, OUTPUT);
    pinMode(DirX, OUTPUT);
    pinMode(StepY, OUTPUT);
    pinMode(DirY, OUTPUT);
    pinMode(EnablePin, OUTPUT);

    // Enable the CNC shield
    digitalWrite(EnablePin, LOW); // Enable motors

    // Start serial communication
    Serial.begin(9600);
}

void loop() {
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n'); // Read the incoming command
        Serial.println(command);
        

        unsigned long currentTime = millis();
    

        // Parse the command for Slot 1
        if (command.startsWith("A_1_CYCLE_")&& slot1Locked) { // Command to unlock Slot 1
            int cycleNum = command.substring(10).toInt(); // Extract the cycle number
            if (cycleNum >= 1 && cycleNum <= 13) { // Check valid cycle range
                 if (currentTime - slot1UnlockTime >= lockDelay) {
                  rotateMotor1Clockwise(360); // Unlock Slot 1
                Serial.println("locked Slot 1");
                slot1Locked = false; // Update state to unlocked
                } 
                else {
                Serial.println("Cannot lock Slot 1 yet. Wait for 10 seconds.");
               }// Record unlock time
            }
        } else if (command == "A_1_0" && !slot1Locked) { // Command to lock Slot 1
            if (currentTime - slot1UnlockTime >= lockDelay) { // Check if 10 seconds have passed
                rotateMotor1CounterClockwise(360); // Lock Slot 1
                Serial.println("Unlocked Slot 1");
                slot1Locked = true; // Update state to locked
                slot1UnlockTime = currentTime; 
            } else {
                Serial.println("Cannot lock Slot 1 yet. Wait for 10 seconds.");
            }
        }

        // Parse the command for Slot 2
        if (command.startsWith("A_2_CYCLE_" )&& slot2Locked) { // Command to unlock Slot 2
            int cycleNum = command.substring(10).toInt(); // Extract the cycle number
            if (cycleNum >= 1 && cycleNum <= 13) { // Check valid cycle range
               if (currentTime - slot2UnlockTime >= lockDelay) { // Check if 10 seconds have passed
                rotateMotor2Clockwise(360); // Unlock Slot 2
                Serial.println("locked Slot 2");
                slot2Locked = false; // Update state to unlocked
                 } else {
                Serial.println("Cannot lock Slot 2 yet. Wait for 10 seconds.");
            }
            }
        } else if (command == "A_2_0" && !slot2Locked) { // Command to lock Slot 2
            if (currentTime - slot2UnlockTime >= lockDelay) { // Check if 10 seconds have passed
                rotateMotor2CounterClockwise(360); // Lock Slot 2
                Serial.println("Unlocked Slot 2");
                slot2Locked = true; // Update state to locked
                slot2UnlockTime = currentTime; // Record unlock time
            } else {
                Serial.println("Cannot lock Slot 2 yet. Wait for 10 seconds.");
            }
        }
    }
}


// Function to rotate motor
// Function to rotate the motor 1 clockwise
void rotateMotor1Clockwise(int steps) {
    digitalWrite(DirX, HIGH);  // Set direction to clockwise
    for (int x = 0; x < steps; x++) {
        digitalWrite(StepX, HIGH);
        delayMicroseconds(500);  // Adjust speed as needed
        digitalWrite(StepX, LOW);
        delayMicroseconds(500);  // Adjust speed as needed
    }
    delay(500);  // Delay before the next operation
    for(int x = 0; x<200; x++) { // loop for 200 steps
      digitalWrite(StepY,LOW);
      delayMicroseconds(500);
      digitalWrite(StepY,LOW); 
      delayMicroseconds(500);
    }
    delay(500); // delay for 1 second

    // delay for 1 second

}    
// Function to rotate the motor 2 clockwise
void rotateMotor2Clockwise(int steps) {
    digitalWrite(DirY, HIGH);  // Set direction to clockwise
    for (int x = 0; x < steps; x++) {
        digitalWrite(StepX, LOW);
        delayMicroseconds(500);  // Adjust speed as needed
        digitalWrite(StepX, LOW);
        delayMicroseconds(500);  // Adjust speed as needed
    }
    delay(500);  // Delay before the next operation
    for(int x = 0; x<200; x++) { // loop for 200 steps
      digitalWrite(StepY,HIGH);
      delayMicroseconds(500);
      digitalWrite(StepY,LOW); 
      delayMicroseconds(500);
    }
    delay(500); // delay for 1 second
  
}  
void rotateMotor1CounterClockwise(int steps) {
    digitalWrite(DirX, LOW);  // Set direction to clockwise
    for (int x = 0; x < steps; x++) {
        digitalWrite(StepX, HIGH);
        delayMicroseconds(500);  // Adjust speed as needed
        digitalWrite(StepX, LOW);
        delayMicroseconds(500);  // Adjust speed as needed
    }
    delay(500);  // Delay before the next operation
    for(int x = 0; x<200; x++) { // loop for 200 steps
      digitalWrite(StepY,LOW);
      delayMicroseconds(500);
      digitalWrite(StepY,LOW); 
      delayMicroseconds(500);
    }
    delay(500); // delay for 1 second

    // delay for 1 second

}    
// Function to rotate the motor 2 clockwise
void rotateMotor2CounterClockwise(int steps) {
    digitalWrite(DirY, LOW);  // Set direction to clockwise
    for (int x = 0; x < steps; x++) {
        digitalWrite(StepX, LOW);
        delayMicroseconds(500);  // Adjust speed as needed
        digitalWrite(StepX, LOW);
        delayMicroseconds(500);  // Adjust speed as needed
    }
    delay(500);  // Delay before the next operation
    for(int x = 0; x<200; x++) { // loop for 200 steps
      digitalWrite(StepY,HIGH);
      delayMicroseconds(500);
      digitalWrite(StepY,LOW); 
      delayMicroseconds(500);
    }
    delay(500); // delay for 1 second
 
}  