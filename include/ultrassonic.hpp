#pragma once

#include <Arduino.h>

// ESP32 GPIOs
#define trig 12
#define echo 13


namespace Ultrassonic{

    const int MaxLevel = 13;

    void setup();
    /// @brief Measure water level based on MaxLevel.
    /// @return Water height in cm.
    int measureWaterLevel();
}