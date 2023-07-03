#ifndef Button_h
#define Button_h
#define DEBOUNCE_BUTTON 300

#include <Arduino.h>

// static pois nenhuma pode ser acessada de fora do monitor
static SemaphoreHandle_t xSemaphore_ManualButton = NULL;
static SemaphoreHandle_t xSemaphore_AutomaticButton = NULL;
static SemaphoreHandle_t xSemaphore_StopButton = NULL;
static SemaphoreHandle_t xMutex_SystemState = NULL;

namespace Button{
    static int pinManualButton;
    static int pinAutomaticButton;
    static int pinStopButton;

    static unsigned char SystemState;

    void setup(void);
    void SetManualButtom(int value);
    void SetAutomaticButtom(int value);
    void SetStopButtom(int value);

    unsigned char GetButtonState(void);
    
    static void IRAM_ATTR PushManualButton(void);
    static void IRAM_ATTR PushAutomaticButton(void);
    static void IRAM_ATTR PushStopButton(void);

}

void Button::setup(void){
    xMutex_SystemState = xSemaphoreCreateMutex();
    if(xMutex_SystemState == NULL){
        Serial.printf("\n\rFalha em criar o Mutex para o botão desejado");
    }
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

unsigned char Button::GetButtonState(void){
    unsigned char retState;

    if(xSemaphoreTake(xSemaphore_ManualButton,portMAX_DELAY) == pdTRUE){
        xSemaphoreTake(xMutex_SystemState,portMAX_DELAY);

        SystemState = 'M';
        retState = SystemState;

        xSemaphoreGive(xMutex_SystemState);
    }

    if(xSemaphoreTake(xSemaphore_AutomaticButton,portMAX_DELAY) == pdTRUE){
        xSemaphoreTake(xMutex_SystemState,portMAX_DELAY);

        SystemState = 'A';
        retState = SystemState;

        xSemaphoreGive(xMutex_SystemState);
    }

    if(xSemaphoreTake(xSemaphore_StopButton,portMAX_DELAY) == pdTRUE){
        xSemaphoreTake(xMutex_SystemState,portMAX_DELAY);

        SystemState = 'S';
        retState = SystemState;

        xSemaphoreGive(xMutex_SystemState);
    }

    return retState;
}




#endif
