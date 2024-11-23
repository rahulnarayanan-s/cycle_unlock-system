#include <WiFi.h>
#include <FirebaseESP32.h>

// Define Firebase objects
FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;

// Default Wi-Fi and Firebase credentials
const char* DEFAULT_WIFI_SSID = "12";
const char* DEFAULT_WIFI_PASSWORD = "0987654321";
const char* FIREBASE_HOST = "cycle-project-2c901-default-rtdb.asia-southeast1.firebasedatabase.app";
const char* FIREBASE_AUTH = "9jJ2iYWNM9DRiaHzRnD4NRL2m2DhtolLvx7tcYrW";

// Variables to store current Wi-Fi credentials
String currentSSID = DEFAULT_WIFI_SSID;
String currentPassword = DEFAULT_WIFI_PASSWORD;

void connectToWiFi(const char* ssid, const char* password, int timeout = 10000) {
    WiFi.begin(ssid, password);
    Serial.printf("Connecting to Wi-Fi: %s\n", ssid);

    unsigned long startAttemptTime = millis();
    while (WiFi.status() != WL_CONNECTED && (millis() - startAttemptTime) < timeout) {
        Serial.print(".");
        delay(500);
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.printf("\nConnected to Wi-Fi: %s\n", ssid);
    } else {
        Serial.println("\nFailed to connect to Wi-Fi within timeout!");
    }
}

void setup() {
    // Initialize Serial Monitor
    Serial.begin(9600);

    // Connect to default Wi-Fi
    connectToWiFi(DEFAULT_WIFI_SSID, DEFAULT_WIFI_PASSWORD);

    // Set Firebase project host and authentication
    config.host = FIREBASE_HOST;
    config.signer.tokens.legacy_token = FIREBASE_AUTH;

    // Initialize Firebase
    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);

    // Check if connected to Firebase
    if (Firebase.ready()) {
        Serial.println("Connected to Firebase!");
    } else {
        Serial.println("Failed to connect to Firebase");
    }
}

void loop() {
    // Temporary variables to hold fetched Wi-Fi credentials
    String fetchedSSID, fetchedPassword;

    // Retrieve updated Wi-Fi credentials from Firebase
    if (Firebase.getString(firebaseData, "/SSID")) {
        fetchedSSID = firebaseData.stringData();
    } else {
        Serial.println("Failed to retrieve new Wi-Fi SSID");
    }

    if (Firebase.getString(firebaseData, "/password")) {
        fetchedPassword = firebaseData.stringData();
    } else {
        Serial.println("Failed to retrieve new Wi-Fi password");
    }

    // If new credentials are found, attempt to switch Wi-Fi
    if (fetchedSSID.length() > 0 && fetchedPassword.length() > 0) {
        if (fetchedSSID != currentSSID || fetchedPassword != currentPassword) {
            Serial.println("Wi-Fi credentials updated. Attempting to switch...");

            // Save the new credentials
            String newSSID = fetchedSSID;
            String newPassword = fetchedPassword;

            // Disconnect and connect to the new network
            WiFi.disconnect();
            connectToWiFi(newSSID.c_str(), newPassword.c_str(), 10000);

            // Check if the connection to the new network was successful
            if (WiFi.status() == WL_CONNECTED) {
                Serial.println("Switched to new Wi-Fi!");
                currentSSID = newSSID;
                currentPassword = newPassword;
            } else {
                Serial.println("Failed to connect to new Wi-Fi. Reconnecting to default Wi-Fi...");

                // Reconnect to the default Wi-Fi
                connectToWiFi(DEFAULT_WIFI_SSID, DEFAULT_WIFI_PASSWORD);
                currentSSID = DEFAULT_WIFI_SSID;
                currentPassword = DEFAULT_WIFI_PASSWORD;
            }

            // Reinitialize Firebase after switching Wi-Fi
            Firebase.begin(&config, &auth);
            Firebase.reconnectWiFi(true);

            if (Firebase.ready()) {
                Serial.println("Reconnected to Firebase!");
            } else {
                Serial.println("Failed to reconnect to Firebase");
            }
        } else {
            Serial.println("No changes in Wi-Fi credentials. Holding default Wi-Fi.");
        }
    } else {
        Serial.println("No updates in Firebase. Staying connected to default Wi-Fi.");
    }

    delay(10000);  // Delay before the next check
}
