#pragma once

#include <Arduino.h>
#include "pins.h"
#include "motorsensing.hpp"
#include "ultrassonic.hpp"
#include "buttons.hpp"
#include "RTS_settings.h"

namespace Pump{
    extern struct motorInfoStruct motorInfo;
    extern int WaterLevel;
    extern srcSystem SystemState;


    void setup();
    void Task_HandlePump(void *parameters);
}