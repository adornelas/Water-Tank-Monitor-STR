#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "pins.h"
#include "RTS_settings.h"
#include "ultrassonic.hpp"
#include "motorsensing.hpp"
#include "buttons.hpp"

namespace LCD{
    extern LiquidCrystal_I2C lcd;

    extern int WaterLevel;
    extern srcSystem SystemState;

    void setup();
    void Task_LCD(void *parameters);
    void writeState(srcSystem system_state);
    void writeWaterLevel(int water_level);
    void writeCurrent(float current);
    void writeTemperature(float temperature);
}