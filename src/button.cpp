#include "buttons.hpp"
#include "pins.h"

BaseType_t xHigherPriorityTaskWoken = pdFALSE;

void IRAM_ATTR PushManualButton(void){
    static uint32_t last_time = 0;

    xHigherPriorityTaskWoken = pdFALSE;

    // Algoritmo de debounce do botão
    if( (millis() - last_time) >= DEBOUNCE_BUTTON){
        last_time = millis();
        xSemaphoreGiveFromISR(xSemaphore_ManualButton, &xHigherPriorityTaskWoken);
    }
}

void IRAM_ATTR PushAutomaticButton(void){
    static uint32_t last_time = 0;

    xHigherPriorityTaskWoken = pdFALSE;

    // Algoritmo de debounce do botão
    if( (millis() - last_time) >= DEBOUNCE_BUTTON){
        last_time = millis();
        xSemaphoreGiveFromISR(xSemaphore_AutomaticButton, &xHigherPriorityTaskWoken);
    }
}

void IRAM_ATTR PushStopButton(void){
    static uint32_t last_time = 0;

    xHigherPriorityTaskWoken - pdFALSE;

    // Algoritmo de debounce do botão
    if( (millis() - last_time) >= DEBOUNCE_BUTTON){
        last_time = millis();
        xSemaphoreGiveFromISR(xSemaphore_StopButton, &xHigherPriorityTaskWoken);
    }
}

namespace Button{

    srcSystem SystemState;

    void setup(void)
    {
        srcSystem system_state;

        xMutex_SystemState = xSemaphoreCreateMutex();
        if(xMutex_SystemState == NULL){
            Serial.printf("\n\rFalha em criar o Mutex para o botão desejado");
        }

        system_state.State = STOP_MODE;
        system_state.timesPressed = 0;
        SetState(system_state);

        SetManualButton(MANUAL_BUTTON);
        SetAutomaticButton(AUTOMATIC_BUTTON);
        SetStopButton(STOP_BUTTON);
    }

    void SetManualButton(int pin){
        pinMode(pin, INPUT);
        attachInterrupt(pin, 
                        PushManualButton, 
                        RISING);

        vSemaphoreCreateBinary(xSemaphore_ManualButton);

        if(xSemaphore_ManualButton == NULL){
            Serial.printf("\n\rFalha em criar o semafaro para botão manual");
        }
        xSemaphoreTake(xSemaphore_ManualButton,(TickType_t)100);
    }

    void SetAutomaticButton(int pin){
        pinMode(pin, INPUT);
        attachInterrupt(pin, 
                        PushAutomaticButton, 
                        RISING);

        vSemaphoreCreateBinary(xSemaphore_AutomaticButton);
        if(xSemaphore_ManualButton == NULL){
            Serial.printf("\n\rFalha em criar o semafaro para botão automático");
        }
        xSemaphoreTake(xSemaphore_AutomaticButton,(TickType_t)100);
    }

    void SetStopButton(int pin){
        pinMode(pin, INPUT);
        attachInterrupt(pin, 
                        PushStopButton, 
                        RISING);

        vSemaphoreCreateBinary(xSemaphore_StopButton);
        if(xSemaphore_ManualButton == NULL){
            Serial.printf("\n\rFalha em criar o semafaro para botão stop");
        }
        xSemaphoreTake(xSemaphore_StopButton,(TickType_t)100);
    }

    void SetState(srcSystem system_state){
        xSemaphoreTake(xMutex_SystemState,portMAX_DELAY );
        SystemState = system_state;
        // Serial.println(F("T:atribuiu"));
        xSemaphoreGive(xMutex_SystemState);
    }

    srcSystem GetState(void)
    {
        srcSystem system_state;

        xSemaphoreTake(xMutex_SystemState,portMAX_DELAY );
        system_state = SystemState;
        xSemaphoreGive(xMutex_SystemState);
        return system_state;
    }

    void Task_HandleManual(void *parameter)
    {
        srcSystem system_state;
        srcSystem system_state_write;
        while (1)
        {
            if(xSemaphoreTake(xSemaphore_ManualButton,portMAX_DELAY) == pdTRUE){
                system_state = GetState();

                if(system_state.State != MANUAL_MODE)
                    system_state_write.timesPressed = 0;
                else
                    system_state_write.timesPressed++;
                
                system_state_write.State = MANUAL_MODE;
                SetState(system_state_write);
            }
        }
        
    }

    void Task_HandleAutomatic(void *parameter)
    {
        srcSystem system_state;
        srcSystem system_state_write;
        while (1)
        {
            if(xSemaphoreTake(xSemaphore_AutomaticButton,portMAX_DELAY) == pdTRUE){
                system_state = GetState();

                if(system_state.State != AUTOMATIC_MODE)
                    system_state_write.timesPressed = 0;
                else
                    system_state_write.timesPressed++;
                
                system_state_write.State = AUTOMATIC_MODE;
                SetState(system_state_write);
            }

        }
        
    }

    void Task_HandleStop(void *parameter)
    {
        srcSystem system_state;
        srcSystem system_state_write;
        while (1)
        {
            if(xSemaphoreTake(xSemaphore_StopButton,portMAX_DELAY) == pdTRUE){
                system_state = GetState();

                if(system_state.State != STOP_MODE)
                    system_state_write.timesPressed = 0;
                else
                    system_state_write.timesPressed++;
                
                system_state_write.State = STOP_MODE;
                SetState(system_state_write);
            }

        }
        
    }

}

