#include <WiFi.h>
#include <FirebaseESP32.h>
#include <SoftwareSerial.h>

// Wi-Fi credentials
const char* WIFI_SSID = "12";
const char* WIFI_PASSWORD = "0987654321";

const char* FIREBASE_HOST = "https://cycle-rental-system-default-rtdb.firebaseio.com/";
const char* FIREBASE_AUTH = "SB2F7IhaKvvswyMhS5vqusZYZyspKf1V5nk9ioWB";

// Define RX pins for each RFID reader
#define SLOT1_RX 16
#define SLOT2_RX 17
#define SLOT3_RX 19
#define SLOT4_RX 18

// Define the station identifier
const String STAND_ID = "A";

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
SoftwareSerial slot1Reader(SLOT1_RX, -1);
SoftwareSerial slot2Reader(SLOT2_RX, -1);
SoftwareSerial slot3Reader(SLOT3_RX, -1);
SoftwareSerial slot4Reader(SLOT4_RX, -1);

// Variables to store the latest retrieved data
String prevCYCLE_STATUS = "";
String prevCYCLE_ID = "";
String prevSTAND_ID = "";
String prevSLOT_ID = "";

String CYCLE_STATUS;
String STAND_ID_FROM_FIREBASE;
String SLOT_ID;
String CYCLE_ID;
String standStatus;

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
  retrieveDataFromFirebase();
}

void loop() {
  // Retrieve and process data from Firebase
  retrieveDataFromFirebase();

  // Read and upload RFID data from each slot
  readAndUploadRFID(slot1Reader, lastIdSLOT1, "1", "cycle1");
  readAndUploadRFID(slot2Reader, lastIdSLOT2, "2", "cycle2");
  readAndUploadRFID(slot3Reader, lastIdSLOT3, "3", "cycle3");
  readAndUploadRFID(slot4Reader, lastIdSLOT4, "4", "cycle4");

  delay(1000);
}

void retrieveDataFromFirebase() {
  if (Firebase.getString(firebaseData, "/CYCLES/CYCLE_STATUS")) {
    CYCLE_STATUS = firebaseData.stringData();
  } else {
    Serial.println("Failed to retrieve CYCLE_STATUS: " + firebaseData.errorReason());
  }

  if (Firebase.getString(firebaseData, "/CYCLES/STAND_ID")) {
    STAND_ID_FROM_FIREBASE = firebaseData.stringData();
  } else {
    Serial.println("Failed to retrieve STAND_ID: " + firebaseData.errorReason());
  }

  if (Firebase.getInt(firebaseData, "/CYCLES/SLOT_ID")) {
    SLOT_ID = String(firebaseData.intData());
  } else {
    Serial.println("Failed to retrieve SLOT_ID: " + firebaseData.errorReason());
  }

  if (Firebase.getString(firebaseData, "/CYCLES/CYCLE_ID")) {
    CYCLE_ID = firebaseData.stringData();
  } else {
    Serial.println("Failed to retrieve CYCLE_ID: " + firebaseData.errorReason());
  }

  if (CYCLE_STATUS != prevCYCLE_STATUS || CYCLE_ID != prevCYCLE_ID || STAND_ID_FROM_FIREBASE != prevSTAND_ID || SLOT_ID != prevSLOT_ID) {
    prevCYCLE_STATUS = CYCLE_STATUS;
    prevCYCLE_ID = CYCLE_ID;
    prevSTAND_ID = STAND_ID_FROM_FIREBASE;
    prevSLOT_ID = SLOT_ID;

    if (CYCLE_STATUS == "LOCKED") {
      standStatus = STAND_ID_FROM_FIREBASE + "_" + SLOT_ID + "_" + CYCLE_ID;
      Serial.print(standStatus);
    } else if (CYCLE_STATUS == "UNLOCKED") {
      standStatus = STAND_ID_FROM_FIREBASE + "_" + SLOT_ID + "_0";
      Serial.print( standStatus);
    }
  }
}

void readAndUploadRFID(SoftwareSerial &reader, String &lastId, String SLOT_ID, String CYCLE_ID) {
  String id = "";
  int count = 0;
  char input[13];

  if (reader.available()) {
    while (reader.available() && count < 12) {
      input[count] = reader.read();
      count++;
      delay(5);
    }
    input[count] = '\0';
    id = String(input);
  }

  if (id != "" ) {
    
    // Check the RFID ID and print the corresponding cycle
    if (id == "4C003D07F781") {
      // Serial.println("CYCLE 1");
      CYCLE_ID = "CYCLE_1";  // Update CYCLE_ID based on the card ID
    } 
    else if (id == "4C003D13A2C0") {
      // Serial.println("CYCLE 2");
      CYCLE_ID = "CYCLE_2";  // Update CYCLE_ID based on the card ID
    }
    else if (id == "4C003D055226") {
      // Serial.println("CYCLE 3");
      CYCLE_ID = "CYCLE_3";  // Update CYCLE_ID based on the card ID
    }
    else if (id == "4C003D22095A") {
      // Serial.println("CYCLE 4");
      CYCLE_ID = "CYCLE_4";  // Update CYCLE_ID based on the card ID
    }
    else if (id == "43002DFB8217") {
      // Serial.println("CYCLE 5");
      CYCLE_ID = "CYCLE_5";  // Update CYCLE_ID based on the card ID
    }
    else if (id == "42003C1F6302") {
      // Serial.println("CYCLE 6");
      CYCLE_ID = "CYCLE_7";  // Update CYCLE_ID based on the card ID
    }
    else if (id == "4B005ACD36EA") {
      // Serial.println("CYCLE 7");
       CYCLE_ID = "CYCLE_8";  // Update CYCLE_ID based on the card ID
    }
    else if (id == "43002DFD198A") {
      // Serial.println("CYCLE 8");
      CYCLE_ID = "CYCLE_9";  // Update CYCLE_ID based on the card ID
    }
    else if (id == "4C003817FE9D") {
      // Serial.println("CYCLE 9");
      CYCLE_ID = "CYCLE_6";  // Update CYCLE_ID based on the card ID
    }
    else if (id == "4900F775DB10") {
      // Serial.println("CYCLE 10");
      CYCLE_ID = "CYCLE_10";  // Update CYCLE_ID based on the card ID
    }
    else if (id == "4B005A763E59") {
      // Serial.println("CYCLE 11");
      CYCLE_ID = "CYCLE_11";  // Update CYCLE_ID based on the card ID
    }
    else if (id == "41008FA692FA") {
      // Serial.println("CYCLE 12");
      CYCLE_ID = "cycle12";  // Update CYCLE_ID based on the card ID
    }
    else if (id == "4900B6A590CA") {
      // Serial.println("CYCLE 13");
      CYCLE_ID = "CYCLE_13";  // Update CYCLE_ID based on the card ID
    }
    else {
      Serial.println("Unknown card");
      return;  // Exit the function if the card is not recognized
    }
    uploadDataToFirebase(id, STAND_ID, SLOT_ID, CYCLE_ID);
    lastId = id;
  }
}

void uploadDataToFirebase(String id, String station, String SLOT_ID, String CYCLE_ID) {
  String path = "/CYCLES";
  FirebaseJson jsonData;
 int slot=SLOT_ID.toInt();
  jsonData.set("STAND_STATUS", station + "_" + SLOT_ID + "_" + CYCLE_ID);
  jsonData.set("SLOT_ID", slot);
  jsonData.set("STAND_ID", station);
  jsonData.set("CYCLE_STATUS", "LOCKED");
  jsonData.set("CYCLE_ID", CYCLE_ID);

  if (Firebase.setJSON(firebaseData, path, jsonData)) {
    // Serial.println("Data successfully uploaded to Firebase");
  } else {
    Serial.println("Failed to upload data: " + firebaseData.errorReason());
  }
}
