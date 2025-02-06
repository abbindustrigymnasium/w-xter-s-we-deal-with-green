#include <Servo.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <Wire.h>
#include <AM2320.h>

#define FIREBASE_HOST "wexteras-g8-default-rtdb.europe-west1.firebasedatabase.app"
#define FIREBASE_AUTH "AIzaSyAv-utnzbanDe6FrsPF4e5E7X4V3IlGr2o"
#define WIFI_SSID "Hitachigymnasiet_2.4"
#define WIFI_PASSWORD "mittwifiarsabra"

const byte MotorSpeed = D1;
const byte MotorDir = D3;
const byte PumpSpeed = D2;
const byte PumpDir = D4;
const byte servoPin = D0;

Servo continuousServo;
FirebaseData firebaseData;
AM2320 am2320_sensor;

unsigned long previousMillisFirebase = 0;
unsigned long LongTermpreviousMillisFirebase = 0;
const unsigned long firebaseInterval = 4000;
const unsigned long firebaseLongTermInterval = 60000;

void setup() {
  Serial.begin(9600);
  Wire.begin(D6, D5);

  // Initialize pins once
  pinMode(MotorSpeed, OUTPUT);
  pinMode(MotorDir, OUTPUT);
  pinMode(PumpSpeed, OUTPUT);
  pinMode(PumpDir, OUTPUT);

  Wire.beginTransmission(0x5C); // AM2320 address
  if (Wire.endTransmission() == 0) {
    Serial.println("AM2320 detected.");
  } else {
    Serial.println("AM2320 not found.");
  }

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println("\nConnected: " + WiFi.localIP().toString());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  continuousServo.attach(servoPin);
}

int fetchMotorPower() {
  if (Firebase.getInt(firebaseData, "/fan/power")) {
    return firebaseData.intData();
  }
  Serial.println("Error fetching fan power.");
  return 0;
}

bool fetchMotorDirection() {
  if (Firebase.getBool(firebaseData, "/fan/direction")) {
    return firebaseData.boolData();
  }
  Serial.println("Error fetching fan direction.");
  return false;
}

int fetchPumpPower() {
  if (Firebase.getInt(firebaseData, "/pump/power")) {
    return firebaseData.intData();
  }
  Serial.println("Error fetching pump power.");
  return 0;
}

bool fetchPumpDirection() {
  if (Firebase.getBool(firebaseData, "/pump/direction")) {
    return firebaseData.boolData();
  }
  Serial.println("Error fetching pump direction.");
  return false;
}

void controlMotor(bool direction, int power) {
  bool status;
  if (!Firebase.getBool(firebaseData, "/fan/status")) {
    Serial.println("Error fetching fan status.");
    status = false;
  } else {
    status = firebaseData.boolData();
  }

  int pwm = 0;
  if (status && power >= 40) {
    pwm = map(power, 40, 100, 150, 1023); // Minimum PWM to start motor
    pwm = constrain(pwm, 0, 1023);
  }

  digitalWrite(MotorDir, direction ? HIGH : LOW);
  analogWrite(MotorSpeed, pwm);
}

void controlPump(bool direction, int power) {
  bool status;
  if (!Firebase.getBool(firebaseData, "/pump/status")) {
    Serial.println("Error fetching pump status.");
    status = false;
  } else {
    status = firebaseData.boolData();
  }

  int pwm = 0;
  if (status) {
    pwm = map(power, 0, 100, 0, 1023);
    pwm = constrain(pwm, 0, 1023);
  }

  digitalWrite(PumpDir, direction ? HIGH : LOW);
  analogWrite(PumpSpeed, pwm);
}

// Servo control variables
bool lastHatchStatus = false;
bool isServoActive = false;
unsigned long servoStartTime = 0;
const unsigned long servoDuration = 900;

void controlServo() {
  bool newHatchStatus;
  if (Firebase.getBool(firebaseData, "/hatch/status")) {
    newHatchStatus = firebaseData.boolData();
  } else {
    Serial.println("Failed to read hatch status.");
    return;
  }

  if (newHatchStatus != lastHatchStatus) {
    lastHatchStatus = newHatchStatus;
    isServoActive = true;
    servoStartTime = millis();
    continuousServo.write(newHatchStatus ? 180 : 0);
  }

  if (isServoActive && (millis() - servoStartTime >= servoDuration)) {
    continuousServo.write(90);
    isServoActive = false;
  }
}

void readAndSendSensorData() {
  static unsigned long lastRead = 0;
  if (millis() - lastRead >= 5000) {
    if (am2320_sensor.measure()) {
      float temp = am2320_sensor.getTemperature();
      float hum = am2320_sensor.getHumidity();
      Firebase.setFloat(firebaseData, "/current/temperature", temp);
      Firebase.setFloat(firebaseData, "/current/humidity", hum);
    }
    lastRead = millis();
  }
}

unsigned long pumpStartTime = 0;
bool pumpRunning = false;

void checkAutomation() {
  // Fan automation
  if (Firebase.getBool(firebaseData, "/automation/fan/enabled") && firebaseData.boolData()) {
    float tempThreshold;
    if (Firebase.getFloat(firebaseData, "/automation/fan/tempThreshold")) {
      tempThreshold = firebaseData.floatData();
      
      if (am2320_sensor.getTemperature() > tempThreshold) {
        if (!Firebase.getBool(firebaseData, "/fan/status") || !firebaseData.boolData()) {
          Firebase.setBool(firebaseData, "/fan/status", true);
        }
        // Add similar logic for automatic power adjustment
      } else {
        Firebase.setBool(firebaseData, "/fan/status", false);
      }
    }
  }

  // Hatch automation
  if (Firebase.getBool(firebaseData, "/automation/hatch/enabled") && firebaseData.boolData()) {
    float humThreshold;
    if (Firebase.getFloat(firebaseData, "/automation/hatch/humThreshold")) {
      humThreshold = firebaseData.floatData();
      
      bool newHatchStatus = am2320_sensor.getHumidity() > humThreshold;
      if (newHatchStatus != Firebase.getBool(firebaseData, "/hatch/status")) {
        Firebase.setBool(firebaseData, "/hatch/status", newHatchStatus);
      }
    }
  }

  // Pump automation
  if (Firebase.getBool(firebaseData, "/automation/pump/enabled") && firebaseData.boolData()) {
    unsigned long currentMillis = millis();
    int pumpDuration, pumpInterval;
    
    if (Firebase.getInt(firebaseData, "/automation/pump/duration")) {
      pumpDuration = firebaseData.intData();
    }
    if (Firebase.getInt(firebaseData, "/automation/pump/interval")) {
      pumpInterval = firebaseData.intData();
    }

    if (!pumpRunning && (currentMillis - pumpStartTime >= pumpInterval)) {
      pumpStartTime = currentMillis;
      pumpRunning = true;
      Firebase.setBool(firebaseData, "/pump/status", true);
    }
    
    if (pumpRunning && (currentMillis - pumpStartTime >= pumpDuration)) {
      pumpRunning = false;
      Firebase.setBool(firebaseData, "/pump/status", false);
    }
  }
}

void loop() {
  readAndSendSensorData();
  checkAutomation();  // Add this line
  controlServo();
  controlPump(fetchPumpDirection(), fetchPumpPower());
  controlMotor(fetchMotorDirection(), fetchMotorPower());
  delay(300);
}
