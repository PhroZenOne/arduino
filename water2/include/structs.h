#include <Arduino.h>

#ifndef WATER2_STRUCTS_H
#define WATER2_STRUCTS_H

struct Setup {
    Setup(const uint8_t sensorPowerPin, const uint8_t pumpPowerPin, const uint8_t sensorSignalPin, float pumpCutoffPercent, int sensorMinMoisture, int sensorMaxMoisture) {
        this->sensorPowerPin = sensorPowerPin;
        this->pumpPowerPin = pumpPowerPin;
        this->sensorSignalPin = sensorSignalPin;
        this->moistureCutoffPercent = pumpCutoffPercent;
        this->sensorMinMoisture = sensorMinMoisture;
        this->sensorMaxMoisture = sensorMaxMoisture;
    }
    int sensorPowerPin;
    int pumpPowerPin;
    int sensorSignalPin;
    float moistureCutoffPercent;
    int sensorMaxMoisture;
    int sensorMinMoisture;
};

struct RunResult {
    float moisturePercent;
    bool pumpActivated;
};

#endif //WATER2_STRUCTS_H
