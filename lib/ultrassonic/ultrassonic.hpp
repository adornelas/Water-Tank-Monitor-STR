#pragma once

#include <Arduino.h>
#include "../../include/pins.h"
#include "../../include/RTS_settings.h"

extern SemaphoreHandle_t xMutex_Var_Water;

namespace Ultrassonic{

    extern int WaterLevel;

    const int MaxLevel = 15;

    void setup();
    void Task_Measure_Water(void *parameters);
    /// @brief Set the Global Variable Water Level
    /// @param water_level 
    void SetWaterLevel(int water_level);
    /// @brief Measure water level based on MaxLevel.
    /// @param water_level pointer to water level variable.
    void measureWaterLevel(int *water_level);
    /// @brief Get the Global Variable Water Level
    /// @return water level
    int GetWaterLevel();
}