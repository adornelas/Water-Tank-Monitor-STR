#pragma once

#include <Arduino.h>

// ESP32 GPIOs
#define trig 12
#define echo 13


namespace Ultrassonic{

    extern SemaphoreHandle_t xMutex_Var_Water;
    extern int WaterLevel;

    const int MaxLevel = 13;

    void setup();
    void Task_Measure_Water(void *parameters);
    /// @brief Set the Global Variable Water Level
    /// @param water_level 
    void SetWaterLevel(int water_level);
    /// @brief Measure water level based on MaxLevel.
    /// @return Water height in cm.
    int measureWaterLevel();
}