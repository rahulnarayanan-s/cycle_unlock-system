#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>  // Firebase ESP8266 library

// Wi-Fi credentials
const char* ssid = "12";
const char* password = "0987654321";

// Firebase credentials
#define FIREBASE_HOST "https://cycle-project-2c901-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define FIREBASE_AUTH "9jJ2iYWNM9DRiaHzRnD4NRL2m2DhtolLvx7tcYrW"

// Initialize Firebase objects
FirebaseData firebaseData;
FirebaseConfig firebaseConfig;
FirebaseAuth firebaseAuth;  // FirebaseAuth object

String userInput;

void setup() {
  Serial.begin(9600);  // Start serial communication for debugging

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  // Firebase configuration
  firebaseConfig.host = FIREBASE_HOST;
  firebaseConfig.signer.tokens.legacy_token = FIREBASE_AUTH;

  // Initialize Firebase with authentication
  Firebase.begin(&firebaseConfig, &firebaseAuth);  
  Firebase.reconnectWiFi(true);
  
  Serial.println("Enter data to send to Firebase:");
}

void loop() {
  // Check if there is user input from Serial Monitor
  if (Serial.available() > 0) {
    userInput = Serial.readStringUntil('\n');  // Read user input until newline
    userInput.trim();  // Remove any extra spaces or newline characters
    
    if (userInput.length() > 0) {
      Serial.println("Sending to Firebase: " + userInput);
      uploadDataToFirebase(userInput);  // Upload user input to Firebase
    }
  }

  delay(1000);  // Small delay to avoid rapid looping
}

// Function to upload data to Firebase
void uploadDataToFirebase(String data) {
  if (Firebase.setString(firebaseData, "/rfid_data", data)) {  // Upload data to Firebase
    Serial.println("Data uploaded successfully: " + data);
  } else {
    Serial.println("Failed to upload data: " + firebaseData.errorReason());
  }
}
