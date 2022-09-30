#include <Arduino.h>
#include <SoftwareSerial.h>
#include <WiFiEsp.h>
#include "WebClient.h"
#include "structs.h"

#define SENSOR_0_POWER 2
#define SENSOR_1_POWER 3
#define PUMP_0_POWER 4
#define PUMP_1_POWER 5
#define SENSOR_0_SIGNAL A1
#define SENSOR_1_SIGNAL A2
#define ONBOARD_LED 13
#define WIFI_RX 6
#define WIFI_TX 7

#define OFF LOW
#define ON HIGH

#define ENABLED_PUMPS 1
#define ENABLED_WLAN 1

#define SENSOR_0_CUTOFF_PERCENT 0.2f
#define SENSOR_1_CUTOFF_PERCENT 0.2f

#define SLEEP_TIME (3600L*1000*2)

SoftwareSerial WifiSerial(WIFI_RX, WIFI_TX);
WebClient webClient;

const Setup setups[] = {
        Setup(SENSOR_0_POWER, PUMP_0_POWER, SENSOR_0_SIGNAL, SENSOR_0_CUTOFF_PERCENT, 1005, 283),
        Setup(SENSOR_1_POWER, PUMP_1_POWER, SENSOR_1_SIGNAL, SENSOR_1_CUTOFF_PERCENT, 0, 400)
};

char ssid[] = "bongas";
char pass[] = "linjerat";
int wlanStatus = WL_IDLE_STATUS;

int getSensorValue(const Setup &setup) {
    digitalWrite(setup.sensorPowerPin, ON);
    delay(500);
    int value = 0;
    for (int i = 0; i < 10; i++) {
        value += analogRead(setup.sensorSignalPin);
        delay(100);
    }
    value = value / 10;
    digitalWrite(setup.sensorPowerPin, OFF);
    return value;
}

void pump(int pumpPowerPin) {
#if ENABLED_PUMPS
    digitalWrite(pumpPowerPin, ON);
    delay(1000);
    digitalWrite(pumpPowerPin, OFF);
#else
    digitalWrite(ONBOARD_LED, ON);
    delay(1000);
    digitalWrite(ONBOARD_LED, OFF);
#endif
}


RunResult triggerRun(const Setup &setup) {
    RunResult runResult = RunResult();
    int sensorValue = getSensorValue(setup);

    Serial.println(String("Sensor values: ") + sensorValue + " min " + setup.sensorMinMoisture + " max " +
                   setup.sensorMaxMoisture);

    if (setup.sensorMaxMoisture > setup.sensorMinMoisture) {
        runResult.moisturePercent =
                ((sensorValue - setup.sensorMinMoisture) + 0.0f) / (setup.sensorMaxMoisture - setup.sensorMinMoisture) +
                0.0f;
    } else {
        runResult.moisturePercent = 1.0f - (((sensorValue - setup.sensorMaxMoisture) + 0.0f) /
                                            (setup.sensorMinMoisture - setup.sensorMaxMoisture) + 0.0f);
    }
    runResult.pumpActivated = runResult.moisturePercent < setup.moistureCutoffPercent;
    if (runResult.pumpActivated) {
        pump(setup.pumpPowerPin);
    }
    return runResult;
}

void loop() {
    delay(5000);
    int size = sizeof(setups) / sizeof(*setups);
    RunResult results[size];
    for (int i = 0; i < size; i++) {
        results[i] = triggerRun(setups[i]);
        Serial.println(String("Sensor value ") + String(results[i].moisturePercent) + " on setup " + i);
    }
#if ENABLED_WLAN
    webClient.sendResults(results, size);
#endif
    delay(SLEEP_TIME);
}

void setup() {
    pinMode(ONBOARD_LED, OUTPUT);
    digitalWrite(ONBOARD_LED, OFF);

    pinMode(SENSOR_0_POWER, OUTPUT);
    digitalWrite(SENSOR_0_POWER, OFF);
    pinMode(SENSOR_1_POWER, OUTPUT);
    digitalWrite(SENSOR_1_POWER, OFF);

    pinMode(SENSOR_0_SIGNAL, INPUT);
    pinMode(SENSOR_1_SIGNAL, INPUT);

    pinMode(PUMP_0_POWER, OUTPUT);
    digitalWrite(PUMP_0_POWER, OFF);

    pinMode(PUMP_1_POWER, OUTPUT);
    digitalWrite(PUMP_1_POWER, OFF);

    Serial.begin(9600);

#if ENABLED_WLAN

    WifiSerial.begin(9600);
    WiFiEspClass::init(&WifiSerial);
    while(WiFi.status()  == WL_NO_SHIELD){
        Serial.println("Wifi not present");
        delay(10000);
    }
    while(wlanStatus != WL_CONNECTED){
        Serial.print("Connecting");
        wlanStatus = WiFi.begin(ssid, pass);
    }
#endif
}

