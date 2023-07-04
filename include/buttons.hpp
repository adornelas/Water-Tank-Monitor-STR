#ifndef BUTTONS_HPP
#define BUTTONS_HPP

#define DEBOUNCE_BUTTON 300
#define MANUAL_MODE "M"
#define AUTOMATIC_MODE "A"
#define STOP_MODE "S"

#include <Arduino.h>
#include "pins.h"

extern SemaphoreHandle_t xSemaphore_ManualButton;
extern SemaphoreHandle_t xSemaphore_AutomaticButton;
extern SemaphoreHandle_t xSemaphore_StopButton;
extern SemaphoreHandle_t xMutex_SystemState;

struct srcSystem{
    const char* State;
    uint8_t timesPressed;
};

namespace Button{

    static srcSystem SystemState;

    void setup(void);
    void SetManualButtom(int pin);
    void SetAutomaticButtom(int pin);
    void SetStopButtom(int pin);
    void Task_Buttons(void *parameter);

    srcSystem GetButtonState(void);
}




#endif
