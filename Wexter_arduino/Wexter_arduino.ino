#include <Wire.h>
#include <AM2320.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <Servo.h>

// Sensor
AM2320 am2320_sensor;
Servo shutterServo;

// Pin definitions
const int motorControlPin = D1;  // PWM pin connected to the motor controller (blue contacts on base card)

// Wi-Fi credentials
#define WIFI_SSID "Hitachigymnasiet_2.4"
#define WIFI_PASSWORD "mittwifiarsabra"

// Firebase credentials
#define FIREBASE_HOST "wexteras-g8-default-rtdb.europe-west1.firebasedatabase.app"
#define FIREBASE_AUTH "AIzaSyAv-utnzbanDe6FrsPF4e5E7X4V3IlGr2o"

// Firebase objects
FirebaseData firebaseData;
FirebaseConfig firebaseConfig;
FirebaseAuth firebaseAuth;

bool wasConnected = false;
int currentServoAngle = 0; // Track the current position of the servo

void setup() {
  Serial.begin(9600);
  Wire.begin(14, 12);

  pinMode(motorControlPin, OUTPUT);

  shutterServo.attach(D2); // Attach micro servo SG90 to pin D2

  // Configure Firebase
  firebaseConfig.host = FIREBASE_HOST;
  firebaseConfig.signer.tokens.legacy_token = FIREBASE_AUTH;
  Firebase.begin(&firebaseConfig, &firebaseAuth);
  Firebase.reconnectWiFi(true);

  connectToWiFi(); // Initial Wi-Fi connection
}

void loop() {
  // Check Wi-Fi reconnection
  if (WiFi.status() == WL_CONNECTED && !wasConnected) {
    Serial.println("Reconnected to Wi-Fi. Resetting Firebase data...");
    wasConnected = true;
  }

  // Handle Wi-Fi disconnection
  if (WiFi.status() != WL_CONNECTED) {
    wasConnected = false;
    connectToWiFi(); // Attempt reconnection
  }

  // Read Firebase paths for fan and shutter
  if (Firebase.getBool(firebaseData, "/fan/status")) {
    bool fanStatus = firebaseData.boolData();
    if (fanStatus) {
      if (Firebase.getInt(firebaseData, "/fan/power")) {
        int fanPower = firebaseData.intData();
        analogWrite(motorControlPin, map(fanPower, 0, 100, 0, 255));
        Serial.print("Fan power set to: ");
        Serial.println(fanPower);
      } else {
        Serial.println("Failed to read fan power: " + firebaseData.errorReason());
      }
    } else {
      analogWrite(motorControlPin, 0); // Turn off the motor
      Serial.println("Fan is off.");
    }
  } else {
    Serial.println("Failed to read fan status: " + firebaseData.errorReason());
  }

  if (Firebase.getBool(firebaseData, "/hatch/status")) {
    bool shutterStatus = firebaseData.boolData();
    if (shutterStatus) {
      if (Firebase.getInt(firebaseData, "/hatch/power")) {
        int shutterPower = constrain(firebaseData.intData(), 0, 100); // Ensure power is within 0-100 range
        int targetServoAngle = constrain(map(shutterPower, 0, 100, 0, 180), 0, 180); // Map 0% to 0° and 100% to 180°
        moveServoToPosition(targetServoAngle, 5); // Faster movement
        Serial.print("Hatch moving to position: ");
        Serial.println(targetServoAngle);
      } else {
        Serial.println("Failed to read hatch power: " + firebaseData.errorReason());
      }
    } else {
      moveServoToPosition(0, 5); // Gradually return to 0° when hatch is off
      Serial.println("Hatch is off. Returning to 0°.");
    }
  } else {
    Serial.println("Failed to read hatch status: " + firebaseData.errorReason());
  }

  delay(100); // Small delay to avoid rapid polling
}

void moveServoToPosition(int targetAngle, int speed) {
  while (currentServoAngle != targetAngle) {
    if (currentServoAngle < targetAngle) {
      currentServoAngle++;
    } else if (currentServoAngle > targetAngle) {
      currentServoAngle--;
    }
    shutterServo.write(currentServoAngle);
    delay(speed); // Faster movement with reduced delay
  }
}

void connectToWiFi() {
  Serial.print("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  unsigned long startTime = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - startTime < 60000) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to Wi-Fi");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFailed to connect to Wi-Fi within timeout.");
  }
}
