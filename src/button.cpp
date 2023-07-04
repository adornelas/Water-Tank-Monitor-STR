#include "buttons.hpp"
#include "pins.h"


void IRAM_ATTR PushManualButton(void){
    static uint32_t last_time = 0;

    // BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // Algoritmo de debounce do botão
    if( (millis() - last_time) >= DEBOUNCE_BUTTON){
        last_time = millis();
        // xSemaphoreGiveFromISR(xSemaphore_ManualButton, (BaseType_t)(pdFALSE));
        xSemaphoreGiveFromISR(xSemaphore_ManualButton, NULL);
    }
}

void IRAM_ATTR PushAutomaticButton(void){
    static uint32_t last_time = 0;

    // BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // Algoritmo de debounce do botão
    if( (millis() - last_time) >= DEBOUNCE_BUTTON){
        last_time = millis();
        xSemaphoreGiveFromISR(xSemaphore_AutomaticButton, NULL);
        // xSemaphoreGiveFromISR(xSemaphore_AutomaticButton, (BaseType_t)(pdFALSE));
    }
}

void IRAM_ATTR PushStopButton(void){
    static uint32_t last_time = 0;

    // BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // Algoritmo de debounce do botão
    if( (millis() - last_time) >= DEBOUNCE_BUTTON){
        last_time = millis();
        // xSemaphoreGiveFromISR(xSemaphore_StopButton, (BaseType_t)(pdFALSE));
        xSemaphoreGiveFromISR(xSemaphore_StopButton, NULL);
    }
}

namespace Button{
    static srcSystem system_state;

    void setup(void)
    {
        xMutex_SystemState = xSemaphoreCreateMutex();
        if(xMutex_SystemState == NULL){
            Serial.printf("\n\rFalha em criar o Mutex para o botão desejado");
        }

        xSemaphoreTake(xMutex_SystemState,portMAX_DELAY);

        system_state.State = STOP_MODE;
        system_state.timesPressed = 0;

        xSemaphoreGive(xMutex_SystemState);
        SetManualButtom(MANUAL_BUTTON);
        SetAutomaticButtom(AUTOMATIC_BUTTON);
        SetStopButtom(STOP_BUTTON);
    }

    void SetManualButtom(int pin){
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

    void SetAutomaticButtom(int pin){
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

    void SetStopButtom(int pin){
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


    void Task_Buttons(void *parameter)
    {
        srcSystem retState;
        while (1)
        {
            Serial.println(F("Chegou antes do if"));
            if(xSemaphoreTake(xSemaphore_ManualButton,portMAX_DELAY) == pdTRUE){
                Serial.println(F("Chegou 1"));
                xSemaphoreTake(xMutex_SystemState,portMAX_DELAY);
                Serial.println(F("Chegou 2"));

                if(SystemState.State != MANUAL_MODE)
                    SystemState.timesPressed = 0;
                else
                    SystemState.timesPressed++;
                
                SystemState.State = MANUAL_MODE;
                retState.State = SystemState.State;
                retState.timesPressed = SystemState.timesPressed;

                xSemaphoreGive(xMutex_SystemState);
                Serial.println(F("Chegou 3"));

            }
            Serial.println(F("Chegou fora do if"));

            if(xSemaphoreTake(xSemaphore_AutomaticButton,portMAX_DELAY) == pdTRUE){
                xSemaphoreTake(xMutex_SystemState,portMAX_DELAY);

                if(SystemState.State != AUTOMATIC_MODE)
                    SystemState.timesPressed = 0;
                else
                    SystemState.timesPressed++;
                
                SystemState.State = AUTOMATIC_MODE;
                retState.State = SystemState.State;
                retState.timesPressed = SystemState.timesPressed;

                xSemaphoreGive(xMutex_SystemState);
            }

            if(xSemaphoreTake(xSemaphore_StopButton,portMAX_DELAY) == pdTRUE){
                xSemaphoreTake(xMutex_SystemState,portMAX_DELAY);

                if(SystemState.State != STOP_MODE)
                    SystemState.timesPressed = 0;
                else
                    SystemState.timesPressed++;
                
                SystemState.State = STOP_MODE;
                retState.State = SystemState.State;
                retState.timesPressed = SystemState.timesPressed;

                xSemaphoreGive(xMutex_SystemState);
            }

        }
        
    }

    srcSystem GetButtonState(void){
        srcSystem retState;
        Serial.println(F("Chegou antes do if"));
        if(xSemaphoreTake(xSemaphore_ManualButton,portMAX_DELAY) == pdTRUE){
            Serial.println(F("Chegou 1"));
            xSemaphoreTake(xMutex_SystemState,portMAX_DELAY);
            Serial.println(F("Chegou 2"));

            if(SystemState.State != MANUAL_MODE)
                SystemState.timesPressed = 0;
            else
                SystemState.timesPressed++;
            
            SystemState.State = MANUAL_MODE;
            retState.State = SystemState.State;
            retState.timesPressed = SystemState.timesPressed;

            xSemaphoreGive(xMutex_SystemState);
            Serial.println(F("Chegou 3"));

        }
        Serial.println(F("Chegou fora do if"));

        if(xSemaphoreTake(xSemaphore_AutomaticButton,portMAX_DELAY) == pdTRUE){
            xSemaphoreTake(xMutex_SystemState,portMAX_DELAY);

            if(SystemState.State != AUTOMATIC_MODE)
                SystemState.timesPressed = 0;
            else
                SystemState.timesPressed++;
            
            SystemState.State = AUTOMATIC_MODE;
            retState.State = SystemState.State;
            retState.timesPressed = SystemState.timesPressed;

            xSemaphoreGive(xMutex_SystemState);
        }

        if(xSemaphoreTake(xSemaphore_StopButton,portMAX_DELAY) == pdTRUE){
            xSemaphoreTake(xMutex_SystemState,portMAX_DELAY);

            if(SystemState.State != STOP_MODE)
                SystemState.timesPressed = 0;
            else
                SystemState.timesPressed++;
            
            SystemState.State = STOP_MODE;
            retState.State = SystemState.State;
            retState.timesPressed = SystemState.timesPressed;

            xSemaphoreGive(xMutex_SystemState);
        }

        return retState;
    }
}

