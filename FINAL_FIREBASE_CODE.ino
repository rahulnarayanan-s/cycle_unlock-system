#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <SoftwareSerial.h> 

// Wi-Fi credentials
const char* WIFI_SSID = "oppoa17";
const char* WIFI_PASSWORD = "77777778";

const char* FIREBASE_HOST = "cycle-project-2c901-default-rtdb.asia-southeast1.firebasedatabase.app";
const char* FIREBASE_AUTH = "9jJ2iYWNM9DRiaHzRnD4NRL2m2DhtolLvx7tcYrW";

// Define RX pins for each RFID reader
#define SLOT1_RX D1
#define SLOT2_RX D2
#define SLOT3_RX D5
#define SLOT4_RX D6

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
  // Read and upload RFID data from each slot
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

    // Check the RFID ID and print the corresponding cycle
    if (id == "4C003D07F781") {
      // Serial.println("CYCLE 1");
      CYCLE_ID = "cycle1";  // Update CYCLE_ID based on the card ID
    } 
    else if (id == "4C003D13A2C0") {
      // Serial.println("CYCLE 2");
      CYCLE_ID = "cycle2";  // Update CYCLE_ID based on the card ID
    }
    else if (id == "4C003D055226") {
      // Serial.println("CYCLE 3");
      CYCLE_ID = "cycle3";  // Update CYCLE_ID based on the card ID
    }
    else if (id == "4C003D22095A") {
      // Serial.println("CYCLE 4");
      CYCLE_ID = "cycle4";  // Update CYCLE_ID based on the card ID
    }
    else if (id == "43002DFB8217") {
      // Serial.println("CYCLE 5");
      CYCLE_ID = "cycle5";  // Update CYCLE_ID based on the card ID
    }
    else if (id == "42003C1F6302") {
      // Serial.println("CYCLE 6");
      CYCLE_ID = "cycle6";  // Update CYCLE_ID based on the card ID
    }
    else if (id == "4B005ACD36EA") {
      // Serial.println("CYCLE 7");
       CYCLE_ID = "cycle7";  // Update CYCLE_ID based on the card ID
    }
    else if (id == "43002DFD198A") {
      // Serial.println("CYCLE 8");
      CYCLE_ID = "cycle8";  // Update CYCLE_ID based on the card ID
    }
    else if (id == "4C003817FE9D") {
      // Serial.println("CYCLE 9");
      CYCLE_ID = "cycle9";  // Update CYCLE_ID based on the card ID
    }
    else if (id == "4900F775DB10") {
      // Serial.println("CYCLE 10");
      CYCLE_ID = "cycle10";  // Update CYCLE_ID based on the card ID
    }
    else if (id == "4B005A763E59") {
      // Serial.println("CYCLE 11");
      CYCLE_ID = "cycle11";  // Update CYCLE_ID based on the card ID
    }
    else if (id == "41008FA692FA") {
      // Serial.println("CYCLE 12");
      CYCLE_ID = "cycle12";  // Update CYCLE_ID based on the card ID
    }
    else if (id == "4900B6A590CA") {
      // Serial.println("CYCLE 13");
      CYCLE_ID = "cycle13";  // Update CYCLE_ID based on the card ID
    }
    else {
      Serial.println("Unknown card");
      return;  // Exit the function if the card is not recognized
    }

    // Update Firebase only if a new ID is detected
    uploadDataToFirebase(id, STAND_ID, SLOT_ID, CYCLE_ID);

    // Store the last processed ID
    lastId = id;
  }
}

// Function to upload data to Firebase under a specific station, slot, and cycle format
void uploadDataToFirebase(String id, String station, String SLOT_ID, String CYCLE_ID) {
  // Define the Firebase path dynamically based on the station, slot, and cycle
  String path = "/stations/" + station + "/" + SLOT_ID + "/" + CYCLE_ID;

  // Create a FirebaseJson object to upload structured data
  FirebaseJson jsonData;

  // Set the structured data in the FirebaseJson object
  jsonData.set("CYCLE_STATUS", "UNLOCKED");
  jsonData.set("SLOT_ID", SLOT_ID);  // Use the SLOT_ID as a string (no need to convert to int)
  jsonData.set("STAND_ID", station);
  jsonData.set("CYCLE", CYCLE_ID);  // Add cycle info
  jsonData.set("ID", id);  // Add the RFID ID

  if (Firebase.setString(firebaseData, "/CYCLE_STATUS", CYCLE_STATUS)) {
            //Serial.println("Data sent successfully to Firebase!");
        } else {
            Serial.println("Failed to send data");
            Serial.println("Reason: " + firebaseData.errorReason());
        }
  if (Firebase.setString(firebaseData, "/CYCLE_ID", CYCLE_ID)) {
            // Serial.println("Data sent successfully to Firebase!");
        } else {
             Serial.println("Failed to send data");
            Serial.println("Reason: " + firebaseData.errorReason());
        }
        
  if (Firebase.setString(firebaseData, "/SLOT_ID", SLOT_ID)) {
            // Serial.println("Data sent successfully to Firebase!");
        } else {
            Serial.println("Failed to send data");
            Serial.println("Reason: " + firebaseData.errorReason());
        }

  if (Firebase.setString(firebaseData, "/STAND_ID",STAND_ID  )) {
            // Serial.println("Data sent successfully to Firebase!");
        } else {
            Serial.println("Failed to send data");
            Serial.println("Reason: " + firebaseData.errorReason());
        }
  if (Firebase.setString(firebaseData, "/STAND_STATUS", STAND_ID + "_" + SLOT_ID +"_" + CYCLE_ID )) {
            //Serial.println("Data sent successfully to Firebase!");
        } else {
            Serial.println("Failed to send data");
            Serial.println("Reason: " + firebaseData.errorReason());
        }

  // Send data to Firebase
  if (Firebase.setJSON(firebaseData, path, jsonData)) {
    //Serial.println("Uploaded data to: " + path);
  } else {
    Serial.println("Failed to upload data: " + firebaseData.errorReason());
  }
}
