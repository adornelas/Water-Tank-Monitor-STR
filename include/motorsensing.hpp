#ifndef MOTORSSENSING_HPP
#define MOTORSSENSING_HPP

#include <Arduino.h>
#include "ACS712.h"
#include "pins.h"
#include <DHT.h>
#include <DHT_U.h>

extern SemaphoreHandle_t xMutex_Var_MotorInfo;

namespace MotorSensing{

    struct motorInfoStruct{
        float current;
        float temperature;
    };

    extern struct motorInfoStruct motorInfo;

    void setup();
    void Task_MeasureMotor(void *parameters);
    
    /// @brief Set the Global Variable Current
    /// @param current_value 
    void SetMotorInfoValue(float current_value, float temperatura_value);

    /// @brief Measure Motor's electric current value and temperature
    /// @param motor_info pointer to the struct of motor info
    void measureMotor(motorInfoStruct *motor_info);

    /// @brief Measure Motor's electric current value and temperature
    /// @param motor_info pointer to the struct of motor info
    void getMotorInfo(float *current_value, float *temperatura_value);
}

#endif