// Define stepper motor control pins
const int StepPin = 2; // Step pin for CNC Shield
const int DirPin = 5;  // Direction pin for CNC Shield

// Pin for receiving data from ESP8266
#define rx 0 // RX pin for Arduino (connected to TX of ESP8266)

String message;
char input[50]; // Increased size for larger messages

void setup() {
  Serial.begin(9600);  // Start serial communication with Serial Monitor
  pinMode(StepPin, OUTPUT);
  pinMode(DirPin, OUTPUT);
}

void loop() {
  if (Serial.available()) {
    int count = 0;
    while (Serial.available() && count < sizeof(input) - 1) {
      input[count] = Serial.read();
      count++;
      delay(5); // Allow time for incoming data
    }

    input[count] = '\0'; // Null-terminate the string
    message = String(input); // Convert input array to string

    Serial.println( message); // Print received message

    // Process the message
    
  }

  // Reduce delay for faster reading
}
