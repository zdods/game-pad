#include <ArduinoJson.h>

StaticJsonDocument<200> doc;
char json[100];

//
// PINS
//
const int switchPin = 2; // switch pin
const int xPin = 0; // x-axis pin
const int yPin = 1; // y-axis pin

const int onePin = 7;
const int twoPin = 8;

void setup() {
  // Initialize serial port
  Serial.begin(9600);
  while (!Serial) continue;
  pinMode(switchPin, INPUT);
  pinMode(onePin, INPUT);
  pinMode(twoPin, INPUT);
  digitalWrite(switchPin, HIGH);
}

void loop() {
  if(Serial.available() > 0) {
    Serial.readStringUntil('\n').toCharArray(json, 100); 
    Serial.println(json);

    DeserializationError error = deserializeJson(doc, json);
  
    // Test if parsing succeeds.
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }
  }


  // JOYSTICK
  if(digitalRead(switchPin) == 0) {
    Serial.println((const char*)doc["data"]["press"]);
  }
  if(analogRead(xPin) > 600) {
    Serial.println((const char*)doc["data"]["right"]);
  }
  if(analogRead(xPin) < 500) {
    Serial.println((const char*)doc["data"]["left"]);
  }
  if(analogRead(yPin) > 600) {
    Serial.println((const char*)doc["data"]["down"]);
  }
  if(analogRead(yPin) < 500) {
    Serial.println((const char*)doc["data"]["up"]);
  }

  // BUTTONS
  if(digitalRead(onePin) == 1) {
    Serial.println((const char*)doc["data"]["one"]);
  }
  if(digitalRead(twoPin) == 1) {
    Serial.println((const char*)doc["data"]["two"]);
  }
}
