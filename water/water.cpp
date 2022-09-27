#include <Arduino.h>
#include <SoftwareSerial.h>
#include <WiFiEsp.h>

#define SENSOR_1_POWER 2
#define SENSOR_2_POWER 3
#define PUMP_1_POWER 4
#define PUMP_2_POWER 5
#define SENSOR_1_SIGNAL A1
#define SENSOR_2_SIGNAL A2
#define ONBOARD_LED 13
#define WIFI_RX 7
#define WIFI_TX 6

#define OFF LOW
#define ON HIGH

#define DISABLED_PUMPS true

#define SENSOR_1_CUTOFF 250
#define SENSOR_2_CUTOFF 250

#define SLEEP_TIME 3600000

SoftwareSerial WifiSerial(WIFI_RX, WIFI_TX);

char ssid[] = "bongas";
char pass[] = "linjerat";
int wlanStatus = WL_IDLE_STATUS;

void setup() {
  pinMode(ONBOARD_LED, OUTPUT);
  digitalWrite(ONBOARD_LED, OFF);

  pinMode(SENSOR_1_POWER, OUTPUT);
  digitalWrite(SENSOR_1_POWER, OFF);
  pinMode(SENSOR_2_POWER, OUTPUT);
  digitalWrite(SENSOR_2_POWER, OFF);

  pinMode(SENSOR_1_SIGNAL, INPUT);
  pinMode(SENSOR_2_SIGNAL, INPUT);

  pinMode(PUMP_1_POWER, OUTPUT);
  digitalWrite(PUMP_1_POWER, OFF);
  
  pinMode(PUMP_2_POWER, OUTPUT);
  digitalWrite(PUMP_2_POWER, OFF);

  Serial.begin(115200);
  WifiSerial.begin(9600);
  WiFi.init(&WifiSerial);
  if(WiFi.status()  == WL_NO_SHIELD){
    Serial.println("Wifi not present");
    return;
  }
  while(wlanStatus != WL_CONNECTED){
    Serial.print("Connecting");
    wlanStatus = WiFi.begin(ssid, pass);
  }

  printCurrentNet();
}


void printCurrentNet()
{
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  byte bssid[6];
  WiFi.BSSID(bssid);
  char buf[20];
  sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X", bssid[5], bssid[4], bssid[3], bssid[2], bssid[1], bssid[0]);
  Serial.print("BSSID: ");
  Serial.println(buf);

  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI): ");
  Serial.println(rssi);
}

void loop() {
  triggerRun(SENSOR_1_POWER, SENSOR_1_SIGNAL, PUMP_1_POWER, SENSOR_1_CUTOFF);
  triggerRun(SENSOR_2_POWER, SENSOR_2_SIGNAL, PUMP_2_POWER, SENSOR_2_CUTOFF);
  delay(SLEEP_TIME);
}

int triggerRun(int sensorPower, int sensorSignal, int pumpPowerPin, int cutoffValue){
  int sensorValue = getSensorValue(sensorPower, sensorSignal);
  if(sensorValue < cutoffValue){
    pump(pumpPowerPin);
  }
}

int getSensorValue(int sensorPower, int sensorSignal){
  digitalWrite(sensorPower, ON);
  delay(500);
  int value = 0;
  for(int i = 0; i < 10; i++){
    value += analogRead(sensorSignal);
    delay(100);
  }
  value = value / 10;
  digitalWrite(sensorPower, OFF);
  return value;
}

void pump(int pumpPowerPin){
  if(DISABLED_PUMPS){
    digitalWrite(ONBOARD_LED, ON);
    delay(1000);
    digitalWrite(ONBOARD_LED, OFF);
    return;
  }
  digitalWrite(pumpPowerPin, ON);
  delay(1000);
  digitalWrite(pumpPowerPin, OFF);
}
