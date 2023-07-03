#ifndef Button_h
#define Button_h

#define DEBOUNCE_BUTTON 300
#define MANUAL_MODE "M"
#define AUTOMATIC_MODE "A"
#define STOP_MODE "S"

#include <Arduino.h>

// static pois nenhuma pode ser acessada de fora do monitor
static SemaphoreHandle_t xSemaphore_ManualButton = NULL;
static SemaphoreHandle_t xSemaphore_AutomaticButton = NULL;
static SemaphoreHandle_t xSemaphore_StopButton = NULL;
static SemaphoreHandle_t xMutex_SystemState = NULL;

typedef struct{
    const char* State;
    uint8_t timesPressed;
} srcSystem_t;

namespace Button{
    static int pinManualButton;
    static int pinAutomaticButton;
    static int pinStopButton;

    static srcSystem_t SystemState;

    void setup(void);
    void SetManualButtom(int value);
    void SetAutomaticButtom(int value);
    void SetStopButtom(int value);

    srcSystem_t GetButtonState(void);
    
    static void IRAM_ATTR PushManualButton(void);
    static void IRAM_ATTR PushAutomaticButton(void);
    static void IRAM_ATTR PushStopButton(void);

}

void Button::setup(void){
    xMutex_SystemState = xSemaphoreCreateMutex();
    if(xMutex_SystemState == NULL){
        Serial.printf("\n\rFalha em criar o Mutex para o botão desejado");
    }

    xSemaphoreTake(xMutex_SystemState,portMAX_DELAY);

    SystemState.State = STOP_MODE;
    SystemState.timesPressed = 0;

    xSemaphoreGive(xMutex_SystemState);
}

void Button::SetManualButtom(int value){
    pinManualButton=value;

    pinMode(pinManualButton, INPUT);
    attachInterrupt(digitalPinToInterrupt(pinManualButton), 
                    Button::PushManualButton, 
                    RISING);

    vSemaphoreCreateBinary(xSemaphore_ManualButton);

    if(xSemaphore_ManualButton == NULL){
        Serial.printf("\n\rFalha em criar o semafaro para botão manual");
    }
}

void Button::SetAutomaticButtom(int value){
    pinAutomaticButton=value;

    pinMode(pinAutomaticButton, INPUT);
    attachInterrupt(digitalPinToInterrupt(pinAutomaticButton), 
                    Button::PushAutomaticButton, 
                    RISING);

    vSemaphoreCreateBinary(xSemaphore_AutomaticButton);
    if(xSemaphore_ManualButton == NULL){
        Serial.printf("\n\rFalha em criar o semafaro para botão automático");
    }
}

void Button::SetStopButtom(int value){
    pinStopButton=value;

    pinMode(pinStopButton, INPUT);
    attachInterrupt(digitalPinToInterrupt(pinStopButton), 
                    Button::PushStopButton, 
                    RISING);

    vSemaphoreCreateBinary(xSemaphore_StopButton);
    if(xSemaphore_ManualButton == NULL){
        Serial.printf("\n\rFalha em criar o semafaro para botão stop");
    }
}

static void IRAM_ATTR PushManualButton(void){
    static uint32_t last_time = 0;

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // Algoritmo de debounce do botão
    if( (millis() - last_time) >= DEBOUNCE_BUTTON){
        last_time = millis();
        xSemaphoreGiveFromISR(xSemaphore_ManualButton, NULL);
    }
}

static void IRAM_ATTR PushAutomaticButton(void){
    static uint32_t last_time = 0;

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // Algoritmo de debounce do botão
    if( (millis() - last_time) >= DEBOUNCE_BUTTON){
        last_time = millis();
        xSemaphoreGiveFromISR(xSemaphore_AutomaticButton, NULL);
    }
}

static void IRAM_ATTR PushStopButton(void){
    static uint32_t last_time = 0;

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // Algoritmo de debounce do botão
    if( (millis() - last_time) >= DEBOUNCE_BUTTON){
        last_time = millis();
        xSemaphoreGiveFromISR(xSemaphore_StopButton, NULL);
    }
}

srcSystem_t Button::GetButtonState(void){
    srcSystem_t retState;

    if(xSemaphoreTake(xSemaphore_ManualButton,portMAX_DELAY) == pdTRUE){
        xSemaphoreTake(xMutex_SystemState,portMAX_DELAY);

        if(SystemState.State != MANUAL_MODE)
            SystemState.timesPressed = 0;
        else
            SystemState.timesPressed++;
        
        SystemState.State = MANUAL_MODE;
        retState.State = SystemState.State;
        retState.timesPressed = SystemState.timesPressed;

        xSemaphoreGive(xMutex_SystemState);
    }

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




#endif
