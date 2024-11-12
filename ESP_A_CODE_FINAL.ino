#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <SoftwareSerial.h>  // Include SoftwareSerial for multiple RFID readers

// Wi-Fi credentials
const char* WIFI_SSID = "Link";
const char* WIFI_PASSWORD = "jan12345";

const char* FIREBASE_HOST = "cycle-project-2c901-default-rtdb.asia-southeast1.firebasedatabase.app";
const char* FIREBASE_AUTH = "9jJ2iYWNM9DRiaHzRnD4NRL2m2DhtolLvx7tcYrW";

// Define RX pins for each RFID reader
#define SLOT1_RX D1
#define SLOT2_RX D2
#define SLOT3_RX D3
#define SLOT4_RX D4

// Define the station identifier
const String STAND_ID = "A"; 
const String CYCLE_STATUS = "LOCKED";  // Constant for station identifier

// Initialize Firebase objects
FirebaseData firebaseData;
FirebaseConfig firebaseConfig;
FirebaseAuth firebaseAuth;

// RFID variables
String lastIdSLOT1 = "";
String lastIdSLOT2 = "";
String lastIdSLOT3 = "";
String lastIdSLOT4 = "";

// Variables to store the latest retrieved data
String prevCYCLE_STATUS = "";
String prevSTAND_ID = "";
String prevSLOT_ID = "";

// Software serial ports for RFID readers
SoftwareSerial slot1Reader(SLOT1_RX, -1);  // Only RX pin is defined
SoftwareSerial slot2Reader(SLOT2_RX, -1);
SoftwareSerial slot3Reader(SLOT3_RX, -1);
SoftwareSerial slot4Reader(SLOT4_RX, -1);

void setup() {
  Serial.begin(9600);

  // Initialize software serial for each RFID reader
  slot1Reader.begin(9600);
  slot2Reader.begin(9600);
  slot3Reader.begin(9600);
  slot4Reader.begin(9600);

  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

  // Firebase configuration
  firebaseConfig.host = FIREBASE_HOST;
  firebaseConfig.signer.tokens.legacy_token = FIREBASE_AUTH;

  // Initialize Firebase
  Firebase.begin(&firebaseConfig, &firebaseAuth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  // Retrieve and print Firebase data if changed
  retrieveAndPrintFirebaseData();

  // Read and upload RFID data for each slot
  readAndUploadRFID(slot1Reader, lastIdSLOT1, "1", "cycle1");
  readAndUploadRFID(slot2Reader, lastIdSLOT2, "2", "cycle2");
  readAndUploadRFID(slot3Reader, lastIdSLOT3, "3", "cycle3");
  readAndUploadRFID(slot4Reader, lastIdSLOT4, "4", "cycle4");

  delay(1000);  // Avoid rapid polling
}

// Function to read RFID and upload data to Firebase for a specific slot
void readAndUploadRFID(SoftwareSerial &reader, String &lastId, String SLOT_ID, String CYCLE_ID) {
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
    Serial.print(STAND_ID + "_" + SLOT_ID + "_" + CYCLE_STATUS);

    // Map RFID IDs to corresponding cycles
    if (id == "4C003D07F781") CYCLE_ID = "1";
    else if (id == "4C003D13A2C0") CYCLE_ID = "2";
    else if (id == "4C003D055226") CYCLE_ID = "3";
    else if (id == "4C003D22095A") CYCLE_ID = "4";
    else if (id == "43002DFB8217") CYCLE_ID = "5";
    else if (id == "42003C1F6302") CYCLE_ID = "6";
    else if (id == "4B005ACD36EA") CYCLE_ID = "7";
    else if (id == "43002DFD198A") CYCLE_ID = "8";
    else if (id == "4C003817FE9D") CYCLE_ID = "9";
    else if (id == "4900F775DB10") CYCLE_ID = "10";
    else if (id == "4B005A763E59") CYCLE_ID = "11";
    else if (id == "41008FA692FA") CYCLE_ID = "12";
    else if (id == "4900B6A590CA") CYCLE_ID = "13";
    else {
      Serial.println("Unknown card");
      return;
    }

    // Update Firebase only if a new ID is detected
    uploadDataToFirebase(id, STAND_ID, SLOT_ID, CYCLE_ID);

    // Store the last processed ID
    lastId = id;
  }
}

/// Declare global variables at the top of the file
String prevSTATUS = "";


void retrieveAndPrintFirebaseData() {
  String STATUS, STAND_ID, SLOT_ID;

  // Retrieve STATUS (previously CYCLE_STATUS)
  if (Firebase.getString(firebaseData, "/CYCLE_UNLOCK/STATUS")) {
    STATUS = firebaseData.stringData();
  } else {
    Serial.println("Failed to retrieve STATUS: " + firebaseData.errorReason());
  }

  // Retrieve STAND_ID
  if (Firebase.getString(firebaseData, "/CYCLE_UNLOCK/STAND_ID")) {
    STAND_ID = firebaseData.stringData();
  } else {
    Serial.println("Failed to retrieve STAND_ID: " + firebaseData.errorReason());
  }

  // Retrieve SLOT_ID as integer and convert to string
  if (Firebase.getInt(firebaseData, "/CYCLE_UNLOCK/SLOT_ID")) {
    SLOT_ID = String(firebaseData.intData());
  } else {
    Serial.println("Failed to retrieve SLOT_ID: " + firebaseData.errorReason());
  }

  // Print only if there is a change
  if (STATUS != prevSTATUS || STAND_ID != prevSTAND_ID || SLOT_ID != prevSLOT_ID) {
    prevSTATUS = STATUS;
    prevSTAND_ID = STAND_ID;
    prevSLOT_ID = SLOT_ID;

    if (!STATUS.isEmpty() && !STAND_ID.isEmpty() && !SLOT_ID.isEmpty()) {
      Serial.print(STAND_ID + "_" + SLOT_ID + "_" + STATUS);
    }
  }
}

// Function to upload data to Firebase under a specific station, slot, and cycle format
void uploadDataToFirebase(String id, String station, String SLOT_ID, String CYCLE_ID) {
  // Define the base path under "CYCLE_LOCKED"
  String path = String("/CYCLE_LOCKED") ;

  // Create FirebaseJson object to store structured data
  FirebaseJson jsonData;

  // Convert SLOT_ID to an integer
  int slotIdInt = SLOT_ID.toInt();

  // Set structured data in the FirebaseJson object
  jsonData.set("STATUS", "LOCKED");      // Set CYCLE_STATUS as "UNLOCKED"
  jsonData.set("SLOT_ID", slotIdInt);            // Store SLOT_ID as an integer
  jsonData.set("STAND_ID", station);             // Set the station ID
  jsonData.set("CYCLE", CYCLE_ID);               // Add cycle information
  // jsonData.set("ID", id);                        // Add the RFID ID

  // Optional additional fields
  // jsonData.set("CYCLE_ID", CYCLE_ID);            // Store CYCLE_ID separately if needed
  // jsonData.set("STAND_STATUS", station + "_" + SLOT_ID + "_" + CYCLE_ID);  // Concatenated status

  // Upload the JSON object to Firebase at the specified "CYCLE_LOCKED" path
  if (Firebase.setJSON(firebaseData, path, jsonData)) {
    // Serial.println("Data successfully uploaded to: " + path);
  } else {
    Serial.println("Failed to upload data to path: " + path);
    Serial.println("Reason: " + firebaseData.errorReason());
  }
}
