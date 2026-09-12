#include <ESP32Servo.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT11.h>

#define soilPin       27  
#define mq2Pin        14   
#define waterLevelPin 4
#define ldr           35

#define pumb          13 
#define buzz          32

#define flame1Pin     22
#define flame2Pin     21
#define flame3Pin     19
#define flame4Pin     18
#define flame5Pin     5

#define left1         12
#define left2         16
#define right1        2
#define right2        0
#define en1           17
#define en2           15

const char* ssid = "iPhone";
const char* password = "sma2006@@";

const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;

Servo seed, soil, dig, pumbServo;

int soilMin = 1000; 
int soilMax = 3000;
int gasLimit = 2000;
int waterLevelMin = 1500;
int flame1, flame2, flame3, flame4, flame5;
int humVal;
int lightVal;

int fixedR = 2200;

DHT11 dht(34);

void setup() {
  Serial.begin(115200);

  pinMode(flame1Pin, INPUT);
  pinMode(flame2Pin, INPUT);
  pinMode(flame3Pin, INPUT);
  pinMode(flame4Pin, INPUT);
  pinMode(flame5Pin, INPUT);

  pinMode(soilPin, INPUT);
  pinMode(mq2Pin, INPUT);
  pinMode(waterLevelPin, INPUT);
  pinMode(ldr, INPUT);

  pinMode(buzz, OUTPUT);
  pinMode(pumb, OUTPUT);

  pinMode(left1, OUTPUT);
  pinMode(left2, OUTPUT);
  pinMode(en1, OUTPUT);
  pinMode(right1, OUTPUT);
  pinMode(right2, OUTPUT);
  pinMode(en2, OUTPUT);

  seed.attach(25);
  pumbServo.attach(23);
  soil.attach(33);
  soil.write(90);
  dig.attach(26);

  Serial.println("System Started");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  int soilValue = analogRead(soilPin);
  Serial.print("Soil Moisture: ");
  Serial.println(soilValue);

  if (soilValue >= soilMin && soilValue <= soilMax) {
    Serial.println("Soil isn't Dry");
    client.publish("EcoRover/Soil", String(soilValue).c_str());
  } else {
    Serial.println("Soil is Dry");
    client.publish("EcoRover/Soil", String(soilValue).c_str());
  }

  int gasValue = analogRead(mq2Pin);
  Serial.print("Gas Value: ");
  Serial.println(gasValue);

  if (gasValue > gasLimit) {
    Serial.println("WARNING! Harmful Gas Detected!");
    client.publish("EcoRover/Gas", "Gas Detected");
  } else {
    Serial.println("Gas Level is Normal");
    client.publish("EcoRover/Gas", "Normal");
  }

  flame1 = digitalRead(flame1Pin);
  flame2 = digitalRead(flame2Pin);
  flame3 = digitalRead(flame3Pin);
  flame4 = digitalRead(flame4Pin);
  flame5 = digitalRead(flame5Pin);
  flames();

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  int humVal = dht.readHumidity();

  float V_LDR = analogRead(ldr) * (5.0 / 1023.0);
  float V_fixedR = 5.0 - V_LDR;
  float Common_I = V_fixedR / fixedR;
  float R_LDR = V_LDR / Common_I;
  lightVal = 1 / R_LDR;

  unsigned long now = millis();
  if (now - lastMsg > 3000) {
    lastMsg = now;
    if (humVal != DHT11::ERROR_CHECKSUM && humVal != DHT11::ERROR_TIMEOUT) {
      client.publish("EcoRover/Hum", String(humVal).c_str());    
    }   
    client.publish("EcoRover/Light", String(lightVal).c_str());
    Serial.println("Updated sensor readings published.");
  }

  delay(150);
}

// --- دوال الحركة المعدلة ---

void forward() {
  
  // digitalWrite(right1, LOW);
  // digitalWrite(right2, HIGH);
  // digitalWrite(en1, HIGH);

  //  digitalWrite(left1, LOW);
  // digitalWrite(left2, HIGH);
  //  digitalWrite(en2, HIGH);

  digitalWrite(right1,LOW);
  digitalWrite(right2, HIGH);
  digitalWrite(en1, HIGH);

  digitalWrite(left1,HIGH);
  digitalWrite(left2,LOW);
  digitalWrite(en2, HIGH);

}




void back() {
  digitalWrite(right1, HIGH);
  digitalWrite(right2, LOW);
  digitalWrite(en1, HIGH);

  digitalWrite(left1, LOW);
  digitalWrite(left2, HIGH);
  digitalWrite(en2, HIGH);

  
}

void left() {
  digitalWrite(right1, LOW);
  digitalWrite(right2, HIGH);
  digitalWrite(en1, HIGH);

  digitalWrite(left1, LOW);
  digitalWrite(left2, HIGH);
   digitalWrite(en2, HIGH);
}

void right() {
     digitalWrite(right1,HIGH);
    digitalWrite(right2,LOW);
    digitalWrite(en1,HIGH);
    digitalWrite(left1,HIGH);
    digitalWrite(left2,LOW);
    digitalWrite(en2,HIGH);

}

void stop() {
  digitalWrite(right1, LOW);
  digitalWrite(right2, LOW);
  digitalWrite(en1, LOW);

  digitalWrite(left1, LOW);
  digitalWrite(left2, LOW);
  digitalWrite(en2, LOW);
}

// ----------------------------

void flames() {
  if (flame1 == 1 && flame2 == 1 && flame3 == 1 && flame4 == 1 && flame5 == 1) {
    for (int i = 110; i < 180; i++) {
      pumbServo.write(i);
      delay(50);
    }
    for (int i = 180; i >= 50; i--) {
      pumbServo.write(i);
      delay(50);
    }
    for (int i = 50; i < 110; i++) {
      pumbServo.write(i);
      delay(50);
    }
    client.publish("EcoRover/Flame", "FIRE");
    //digitalWrite(buzz, HIGH);
    digitalWrite(pumb, HIGH);
  }
  else if (flame1 == 1) {
    pumbServo.write(150);
    client.publish("EcoRover/Flame", "FIRE");
   // digitalWrite(buzz, HIGH);
    digitalWrite(pumb, HIGH);
  }
  else if (flame2 == 1) {
    pumbServo.write(120);
    client.publish("EcoRover/Flame", "FIRE");
   // digitalWrite(buzz, HIGH);
    digitalWrite(pumb, HIGH);
  }
  else if (flame3 == 1) {
    pumbServo.write(90);
    client.publish("EcoRover/Flame", "FIRE");
   // digitalWrite(buzz, HIGH);
    digitalWrite(pumb, HIGH);
  }
  else if (flame4 == 1) {
    pumbServo.write(60);
    client.publish("EcoRover/Flame", "FIRE");
   // digitalWrite(buzz, HIGH);
    digitalWrite(pumb, HIGH);
  }
  else if (flame5 == 1) {
    pumbServo.write(30);
    client.publish("EcoRover/Flame", "FIRE");
   // digitalWrite(buzz, HIGH);
    digitalWrite(pumb, HIGH);
  }
  else {
    client.publish("EcoRover/Flame", "Safe");
    digitalWrite(buzz, LOW);
    digitalWrite(pumb, LOW);
    pumbServo.write(90);
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(message);

  if (String(topic) == "EcoRover/Motion") {
    if (message == "F") {
      forward();
    } else if (message == "B") {
      back();
    } else if (message == "L") {
      left();
    } else if (message == "R") {
      right();
    } else if (message == "S") {
      stop();
    } else if (message == "SOIL") {
      soil.write(0);
      delay(2000);
      soil.write(90);
    } else if (message == "PLANT") {
      stop();
      dig.write(90);
      delay(100);
      dig.write(0);

      seed.write(90);
      delay(1000);
      seed.write(0);
    }
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to HiveMQ...");
    String clientId = "ESP32Client-" + String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      Serial.println("CONNECTED!");
      client.subscribe("EcoRover/Motion");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds...");
      delay(5000);
    }
  }
}
