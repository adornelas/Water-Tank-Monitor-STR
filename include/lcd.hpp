#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "pins.h"
#include "RTS_settings.h"

extern SemaphoreHandle_t xMutex_Var_Water;

namespace LCD{

    extern int WaterLevel;

    const int MaxLevel = 15;

    void setup();
    void Task_LCD(void *parameters);
}