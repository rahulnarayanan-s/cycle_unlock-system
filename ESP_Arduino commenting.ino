void setup() {
  Serial.begin(9600); // Initialize Serial communication
  Serial.println("ESP8266 ready. Type messages to send to Arduino...");
}

void loop() {
  // Check if there is data available to read from the Serial Monitor
  if (Serial.available()) {
    String userInput = Serial.readStringUntil('\n'); // Read the incoming message until a newline character

    // Send the user input to the Arduino
    // Print what is being sent for debugging
    Serial.print(userInput); // Send the input to Arduino
  }
}
