// Pin for receiving data from ESP8266
#define rx 0 // RX pin for Arduino (connected to TX of ESP8266)

// Initialize the message variable
String message;

void setup() {
  Serial.begin(9600);        // Start serial communication with the Serial Monitor
  pinMode(LED_BUILTIN, OUTPUT); // Set the built-in LED pin as OUTPUT
}

void loop() {
  if (Serial.available()) {   // Check if data is available to read
    message = Serial.readStringUntil('\n'); // Read the incoming message until a newline character

    // Print the received message for debugging
    Serial.println("Received: " + message);

    // Trim any leading or trailing whitespace from the message
    message.trim();

    // Check if the received message is "1"
    if (message == "1") { // Compare the trimmed message with "1"
      blinkLED(); // Call the function to blink the LED
    }
     // Check if the received message is "1"
    if (message == "0") { // Compare the trimmed message with "1"
      blinkLEDF(); // Call the function to blink the LED
    }
     else {
      Serial.println("Unrecognized command."); // Inform about unrecognized command
    }
  }
}

// Function to blink the built-in LED
void blinkLED() {
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED on (HIGH is the voltage level)
                        // Wait for a second
}
void blinkLEDF() {
    digitalWrite(LED_BUILTIN, LOW); 
}
