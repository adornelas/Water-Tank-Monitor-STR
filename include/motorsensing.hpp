#ifndef MOTORSSENSING_HPP
#define MOTORSSENSING_HPP

#include <Arduino.h>
#include "ACS712.h"
#include <DHT.h>
#include <DHT_U.h>

// ESP32 GPIOs
#define CURRENT_GPIO 12
#define TEMPERATURE_GPIO 13

namespace MotorSensing{

    struct motorInfoStruct{
        float current;
        float temperature;
    };

    extern struct motorInfoStruct motorInfo;

    extern SemaphoreHandle_t xMutex_Var_MotorInfo;

    void setup();
    void Task_MeasureMotor(void *parameters);
    
    /// @brief Set the Global Variable Current
    /// @param current_value 
    void SetMotorInfoValue(float current_value, float temperatura_value);

    /// @brief Measure Motor's electric current value and temperature
    /// @param motor_info pointer to the struct of motor info
    void measureMotor(motorInfoStruct *motor_info);
}

#endif