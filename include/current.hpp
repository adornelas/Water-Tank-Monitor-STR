#ifndef CURRENT_HPP
#define CURRENT_HPP

#include <Arduino.h>
#include "ACS712.h"

// ESP32 GPIOs
#define CURRENT_GPIO 12

namespace Current{

    extern SemaphoreHandle_t xMutex_Var_Current;
    extern float CurrentValue;

    void setup();
    void Task_MeasureCurrent(void *parameters);
    /// @brief Set the Global Variable Current
    /// @param current_value 
    void SetCurrentValue(int current_value);
    /// @brief Measure electric current value
    /// @param water_level pointer to water level variable.
    void measureCurrentValue(int *current_value);
}

#endif