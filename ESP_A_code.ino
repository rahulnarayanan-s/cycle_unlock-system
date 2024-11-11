#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <SoftwareSerial.h>  // Include SoftwareSerial for multiple RFID readers

// Wi-Fi credentials
const char* WIFI_SSID = "Deiva";
const char* WIFI_PASSWORD = "123123.dp";
const char* FIREBASE_HOST = "fmtc-cycle-proj-be-default-rtdb.firebaseio.com/";
const char* FIREBASE_AUTH = "5TGcuMjCb2GhrFqQoG0z9l3IO6T8KPuK4jn2yqIh";

// Define RX pins for each RFID reader
#define SLOT1_RX D1
#define SLOT2_RX D2
#define SLOT3_RX D5
#define SLOT4_RX D6

// Initialize Firebase objects
FirebaseData firebaseData;
FirebaseConfig firebaseConfig;
FirebaseAuth firebaseAuth;

// RFID variables
String lastIdSLOT1 = "";
String lastIdSLOT2 = "";
String lastIdSLOT3 = "";
String lastIdSLOT4 = "";

// Software serial ports for RFID readers
SoftwareSerial slot1Reader(SLOT1_RX, -1);  // Only RX pin is defined
SoftwareSerial slot2Reader(SLOT2_RX, -1);
SoftwareSerial slot3Reader(SLOT3_RX, -1);
SoftwareSerial slot4Reader(SLOT4_RX, -1);

// Variables to store current and previous data values
String prevCYCLE_STATUS = "";
String prevSTAND_ID = "";
String prevSLOT_ID = "";

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);

  // Initialize software serial for each RFID reader
  slot1Reader.begin(9600);
  slot2Reader.begin(9600);
  slot3Reader.begin(9600);
  slot4Reader.begin(9600);

  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Connected to WiFi");

  // Firebase configuration
  firebaseConfig.host = FIREBASE_HOST;
  firebaseConfig.signer.tokens.legacy_token = FIREBASE_AUTH;

  // Initialize Firebase
  Firebase.begin(&firebaseConfig, &firebaseAuth);
  Firebase.reconnectWiFi(true);

  // Check if connected to Firebase
  if (Firebase.ready()) {
    Serial.println("Connected to Firebase!");
  } else {
    Serial.println("Failed to connect to Firebase");
  }
}

void loop() {
  // Variables to store the latest retrieved data from Firebase
  String CYCLE_STATUS;
  String STAND_ID;
  String SLOT_ID;

  // Retrieve CYCLE_STATUS
  if (Firebase.getString(firebaseData, "/CYCLE_STATUS")) {
    if (firebaseData.dataType() == "string") {
      CYCLE_STATUS = firebaseData.stringData();
    }
  } else {
    Serial.println("Failed to retrieve CYCLE_STATUS data");
    Serial.println("Reason: " + firebaseData.errorReason());
  }

  // Retrieve STAND_ID
  if (Firebase.getString(firebaseData, "/STAND_ID")) {
    if (firebaseData.dataType() == "string") {
      STAND_ID = firebaseData.stringData();
    }
  } else {
    Serial.println("Failed to retrieve STAND_ID data");
    Serial.println("Reason: " + firebaseData.errorReason());
  }

  // Retrieve SLOT_ID
  if (Firebase.getInt(firebaseData, "/SLOT_ID")) {  // Assuming SLOT_ID is an integer in Firebase
    SLOT_ID = String(firebaseData.intData());     // Convert integer to string for concatenation
  } else {
    Serial.println("Failed to retrieve SLOT_ID data");
    Serial.println("Reason: " + firebaseData.errorReason());
  }

  // Check if any of the values have changed
  if (CYCLE_STATUS != prevCYCLE_STATUS || STAND_ID != prevSTAND_ID || SLOT_ID != prevSLOT_ID) {
    // If there is a change, update the previous values and print the combined status
    prevCYCLE_STATUS = CYCLE_STATUS;
    prevSTAND_ID = STAND_ID;
    prevSLOT_ID = SLOT_ID;

    if (CYCLE_STATUS.length() > 0 && STAND_ID.length() > 0 && SLOT_ID.length() > 0) {
      String combinedStatus = STAND_ID + "_" + SLOT_ID + "_" + CYCLE_STATUS;
      Serial.println(combinedStatus);
    } else {
      Serial.println("Failed to retrieve all required data (CYCLE_STATUS, STAND_ID, SLOT_ID)");
    }
  }

  // Read and upload RFID data for each slot
  readAndUploadRFID(slot1Reader, lastIdSLOT1, "A_1", 1);
  readAndUploadRFID(slot2Reader, lastIdSLOT2, "A_2", 2);
  readAndUploadRFID(slot3Reader, lastIdSLOT3, "A_3", 3);
  readAndUploadRFID(slot4Reader, lastIdSLOT4, "A_4", 4);

  // Small delay to control the polling rate
  delay(1000);
}

// Function to read RFID and upload data to Firebase for a specific slot
void readAndUploadRFID(SoftwareSerial &reader, String &lastId, String slot, int cycle) {
  String id = "";
  int count = 0;
  char input[13];

  // Check if data is available in the RFID reader
  if (reader.available()) {
    while (reader.available() && count < 12) {  // Read 12 characters from RFID
      input[count] = reader.read();
      count++;
      delay(5);
    }
    input[count] = '\0';  // Null-terminate the string
    id = String(input);   // Convert the input array to a string
  }

  // Only process if the current ID is different from the last processed one
  if (id != "" && id != lastId) {
    Serial.print(slot + "_" + "LOCKED" );
    delay(200);
    // Update Firebase only if a new ID is detected
    uploadDataToFirebase(id, slot, cycle);

    // Store the last processed ID
    lastId = id;
  }
}

/// Function to upload data to Firebase in a flat structure with CYCLE_STATUS, SLOT_ID, and STAND_ID
void uploadDataToFirebase(String id, String slot, int cycle) {
  // Define the base path where data will be uploaded
  String path = "/rfid_data";  // You can adjust this base path as needed

  // Create a JSON object with the flat structure
  FirebaseJson json;
  json.set("CYCLE_STATUS", "UNLOCKED");  // Set cycle status to "UNLOCKED"
  json.set("SLOT_ID", String(cycle));    // Use cycle number as SLOT_ID (converted to string)
  json.set("STAND_ID", "A");             // Assuming "A" as a constant value for STAND_ID

  // Upload the JSON object to Firebase
  if (Firebase.set(firebaseData, path, json)) {
    // Success message
    Serial.println("Successfully uploaded to: " + path + " with ID: " + id);
  } else {
    // Error handling with more details
    String errorMessage = firebaseData.errorReason();
    Serial.println("Failed to upload data to " + path + ": " + errorMessage);
  }
}
