#ifndef BUTTONS_HPP
#define BUTTONS_HPP

#define DEBOUNCE_BUTTON 500
#define MANUAL_MODE "M"
#define AUTOMATIC_MODE "A"
#define STOP_MODE "S"

#include <Arduino.h>
#include "../../include/pins.h"
#include "../../include/RTS_settings.h"

extern SemaphoreHandle_t xSemaphore_ManualButton;
extern SemaphoreHandle_t xSemaphore_AutomaticButton;
extern SemaphoreHandle_t xSemaphore_StopButton;
extern SemaphoreHandle_t xMutex_SystemState;

struct srcSystem{
    const char* State;
    uint8_t timesPressed;
};

namespace Button{

    extern srcSystem SystemState;

    void setup(void);
    void SetManualButton(int pin);
    void SetAutomaticButton(int pin);
    void SetStopButton(int pin);
    void Task_HandleManual(void *parameter);
    void Task_HandleAutomatic(void *parameter);
    void Task_HandleStop(void *parameter);
    void SetState(srcSystem system_state);
    srcSystem GetState(void);
}




#endif
