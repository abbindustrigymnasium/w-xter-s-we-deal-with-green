#include <Wire.h>
#include <AM2320.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <Servo.h>

// Sensor
AM2320 am2320_sensor;
Servo continuousServo; // Continuous rotation servo

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
bool hatchStatus = false; // Tracks the current status of the hatch
unsigned long hatchStartTime = 0; // Tracks when the hatch was turned on/off
const unsigned long hatchDuration = 5000; // Duration for hatch rotation (5 seconds)

void connectToWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");
}

void setup() {
  Serial.begin(9600);
  Wire.begin(D6, D5); 

  pinMode(motorControlPin, OUTPUT);

  continuousServo.attach(D2); // Attach continuous rotation servo to pin D2

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

  // Read Firebase path for hatch status
  if (Firebase.getBool(firebaseData, "/hatch/status")) {
    bool newHatchStatus = firebaseData.boolData();
    if (newHatchStatus != hatchStatus) {
      hatchStatus = newHatchStatus;
      hatchStartTime = millis(); // Record the start time
      if (hatchStatus) {
        // Turn hatch on (rotate full power clockwise for 5 seconds)
        continuousServo.write(180); // Rotate servo clockwise at full speed
        Serial.println("Hatch turned on. Rotating clockwise for 5 seconds...");
      } else {
        // Turn hatch off (rotate full power counterclockwise for 5 seconds)
        continuousServo.write(0); // Rotate servo counterclockwise at full speed
        Serial.println("Hatch turned off. Rotating counterclockwise for 5 seconds...");
      }
    }
  } else {
    Serial.println("Failed to read hatch status: " + firebaseData.errorReason());
  }

  // Stop the servo after 5 seconds if the hatch is on or off
  if (hatchStartTime > 0 && millis() - hatchStartTime >= hatchDuration) {
    continuousServo.write(90); // Stop the servo (neutral position)
    hatchStartTime = 0; // Reset the timer
    Serial.println("Hatch rotation complete. Stopping servo.");
  }

  // Read Firebase path for motor status and power
  if (Firebase.getBool(firebaseData, "/fan/status")) {
    bool motorStatus = firebaseData.boolData();
    if (Firebase.getInt(firebaseData, "/fan/power")) {
      int motorPower = firebaseData.intData();
      if (motorStatus) {
        // Ensure motor power is at least 40 (except when 0)
        int adjustedPower = (motorPower > 0 && motorPower < 40) ? 40 : motorPower;
        analogWrite(motorControlPin, map(adjustedPower, 0, 100, 0, 255)); // Map 0-100% to 0-255 PWM
        Serial.print("Motor turned on. Power level: ");
        Serial.println(adjustedPower);
      } else {
        // Turn motor off
        analogWrite(motorControlPin, 0); // Set PWM to 0 (off)
        Serial.println("Motor turned off.");
      }
    } else {
      Serial.println("Failed to read motor power: " + firebaseData.errorReason());
    }
  } else {
    Serial.println("Failed to read motor status: " + firebaseData.errorReason());
  }

  // Read and send sensor data
  readAndSendSensorData();

  delay(100); // Small delay to avoid rapid polling
}

void readAndSendSensorData() {
  if (am2320_sensor.measure()) {
    float temperature = am2320_sensor.getTemperature();
    float humidity = am2320_sensor.getHumidity();

    if (Firebase.setFloat(firebaseData, "/current/temperature", temperature)) {
      Serial.print("Temperature sent: ");
      Serial.println(temperature);
    } else {
      Serial.println("Failed to send temperature: " + firebaseData.errorReason());
    }

    if (Firebase.setFloat(firebaseData, "/current/humidity", humidity)) {
      Serial.print("Humidity sent: ");
      Serial.println(humidity);
    } else {
      Serial.println("Failed to send humidity: " + firebaseData.errorReason());
    }
  } else {
    Serial.println("Failed to read from AM2320 sensor.");
  }
  delay(5000); // Delay to avoid rapid sensor polling
}
