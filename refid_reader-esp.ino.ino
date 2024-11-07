#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

// Define the RX pins for each RFID reader
#define RFID1_RX D1   // EM-18 Reader 1 RX pin connected to D1 on ESP8266
#define RFID2_RX D2   // EM-18 Reader 2 RX pin connected to D2 on ESP8266
#define RFID3_RX D5   // EM-18 Reader 3 RX pin connected to D5 on ESP8266
#define RFID4_RX D6   // EM-18 Reader 4 RX pin connected to D6 on ESP8266

// Define the buzzer pin (if controlling via ESP)
#define BUZZER_PIN D7  // Change if needed, if using an external transistor/relay for the buzzer

// Initialize SoftwareSerial for each reader
SoftwareSerial rfidReader1(RFID1_RX, -1);  // Reader 1 on D1 (TX unused)
SoftwareSerial rfidReader2(RFID2_RX, -1);  // Reader 2 on D2 (TX unused)
SoftwareSerial rfidReader3(RFID3_RX, -1);  // Reader 3 on D5 (TX unused)
SoftwareSerial rfidReader4(RFID4_RX, -1);  // Reader 4 on D6 (TX unused)

String lastId[4] = {"", "", "", ""};  // Array to store the last processed ID for each reader

void setup() {
  Serial.begin(9600);
  
  // Set the buzzer pin as output (if controlling via ESP)
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);  // Ensure the buzzer is off initially

  // Begin communication with each RFID reader
  rfidReader1.begin(9600);
  rfidReader2.begin(9600);
  rfidReader3.begin(9600);
  rfidReader4.begin(9600);

  Serial.println("Four RFID Readers Ready.");
}

void loop() {
  // Process each RFID reader sequentially
  checkReader(rfidReader1, 0, "Reader 1");
  checkReader(rfidReader2, 1, "Reader 2");
  checkReader(rfidReader3, 2, "Reader 3");
  checkReader(rfidReader4, 3, "Reader 4");

  delay(100);  // Small delay to avoid excessive looping
}

// Function to read RFID data and return ID as a string
String readRFID(SoftwareSerial &rfidReader) {
  String id = "";
  char incomingChar;
  
  while (rfidReader.available()) {
    incomingChar = rfidReader.read();
    id += String(incomingChar);
    delay(5);  // Small delay for each byte
  }

  id.trim();  // Clean the ID string by removing any unwanted characters
  return id;
}

// Function to check each reader, process the ID, and continuously print if present
void checkReader(SoftwareSerial &rfidReader, int readerIndex, String readerName) {
  if (rfidReader.available()) {
    String id = readRFID(rfidReader);
    if (id.length() > 0 && id != lastId[readerIndex]) {  // Process only if ID changes
      processID(id, readerName);
      lastId[readerIndex] = id;  // Update the last processed ID for this reader
    }
  } else {
    // Clear lastId if card is removed so it can detect again when re-entered
    lastId[readerIndex] = "";
  }
}

// Function to process the ID and print a message if it matches specific values
void processID(String id, String readerName) {
  Serial.print(readerName + " detected ID: ");
  
  // Here you would process the ID and avoid triggering the buzzer
  if (id == "4C003D07F781") {
    Serial.println("CYCLE 1");
    // Trigger buzzer if needed, or leave it off
    // digitalWrite(BUZZER_PIN, HIGH);  // Uncomment if you want buzzer sound on certain condition
  } 
  else if (id == "4C003D13A2C0") {
    Serial.println("CYCLE 2");
  } 
  else if (id == "4C003D055226") {
    Serial.println("CYCLE 3");
  } 
  else if (id == "4C003D22095A") {
    Serial.println("CYCLE 4");
  } 
  else {
    Serial.println("Unknown ID: " + id);
  }

  // Keep the buzzer OFF after detection to avoid unwanted sound
  digitalWrite(BUZZER_PIN, LOW);  // Buzzer OFF
}
