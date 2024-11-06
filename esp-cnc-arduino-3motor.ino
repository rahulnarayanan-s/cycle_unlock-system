#include <SoftwareSerial.h>
// #define rx 5                              
// #define tx 4       

const int StepX = 2;
const int DirX = 5;
const int StepY = 3;
const int DirY = 6;
const int StepZ = 4;
const int DirZ = 7;


// String cycle1 = "CYCLE_1";
// String cycle2 = "CYCLE_2";
// String cycle3 = "CYCLE_3";



void setup() {
   
    // Start serial communication
    Serial.begin(9600);  // For communication with ESP8266
    pinMode(StepX,OUTPUT);
    pinMode(DirX,OUTPUT);
    pinMode(StepY,OUTPUT);
    pinMode(DirY,OUTPUT);
    pinMode(StepZ,OUTPUT);
    pinMode( DirZ,OUTPUT);
}

void loop() {
  delay(500);
    // Check if there is data available from the ESP8266
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n'); // Read the incoming command
        Serial.println(command);
        if (command == "CYCLE_1") {
            rotateMotor1Clockwise(200);  // Rotate motor 180 degrees clockwise
            Serial.println("Hello1");
        } else if (command == "CYCLE_2") {
            rotateMotor2Clockwise(200);  // Rotate motor 180 degrees clockwise
            Serial.println("CYCLE 2");
        } else if (command == "CYCLE_3") {
            rotateMotor3Clockwise(200);  // Rotate motor 180 degrees counterclockwise
            Serial.println("CYCLE 3");
        }
    }
}

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

    for(int x = 0; x<200; x++) { // loop for 200 steps
      digitalWrite(StepZ,LOW);
      delayMicroseconds(500);
      digitalWrite(StepZ,LOW); 
      delayMicroseconds(500);
    }
  delay(500); // delay for 1 second

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

    for(int x = 0; x<200; x++) { // loop for 200 steps
      digitalWrite(StepZ,LOW);
      delayMicroseconds(500);
      digitalWrite(StepZ,LOW); 
      delayMicroseconds(500);
    }
  delay(500);
}  
// Function to rotate the motor 3 clockwise
void rotateMotor3Clockwise(int steps) {
    digitalWrite(DirZ, HIGH);  // Set direction to clockwise
    for (int x = 0; x < steps; x++) {
        digitalWrite(StepX, LOW);
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

    for(int x = 0; x<200; x++) { // loop for 200 steps
      digitalWrite(StepZ,HIGH);
      delayMicroseconds(500);
      digitalWrite(StepZ,LOW); 
      delayMicroseconds(500);
    }
  delay(500);
}