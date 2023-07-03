#ifndef Button_h
#define Button_h
#define DEBOUNCE_BUTTON 300

#include <Arduino.h>

// static pois nenhuma pode ser acessada de fora do monitor
static SemaphoreHandle_t xSemaphore_ManualButton = NULL;
static SemaphoreHandle_t xSemaphore_AutomaticButton = NULL;
static SemaphoreHandle_t xSemaphore_StopButton = NULL;

namespace Button{
    static int ManualButton;
    static int AutomaticButton;
    static int StopButton;

    void SetManualButtom(int value);
    void SetAutomaticButtom(int value);
    void SetStopButtom(int value);
    
    void IRAM_ATTR PushManualButton(void);
    void IRAM_ATTR PushAutomaticButton(void);
    void IRAM_ATTR PushStopButton(void);

}

void Button::SetManualButtom(int value){
    ManualButton=value;

    pinMode(ManualButton, INPUT);
    attachInterrupt(digitalPinToInterrupt(ManualButton), 
                    Button::PushManualButton, 
                    RISING);

    vSemaphoreCreateBinary(xSemaphore_ManualButton);
}

void Button::SetAutomaticButtom(int value){
    AutomaticButton=value;

    pinMode(AutomaticButton, INPUT);
    attachInterrupt(digitalPinToInterrupt(AutomaticButton), 
                    Button::PushAutomaticButton, 
                    RISING);

    vSemaphoreCreateBinary(xSemaphore_AutomaticButton);
}

void Button::SetStopButtom(int value){
    StopButton=value;

    pinMode(StopButton, INPUT);
    attachInterrupt(digitalPinToInterrupt(StopButton), 
                    Button::PushStopButton, 
                    RISING);

    vSemaphoreCreateBinary(xSemaphore_StopButton);
}

void IRAM_ATTR PushAutomaticButton(void){
    static uint32_t last_time = 0;

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // Algoritmo de debounce do botão
    if( (millis() - last_time) >= DEBOUNCE_BUTTON){
        last_time = millis();
        xSemaphoreGiveFromISR(xSemaphore_ManualButton, NULL);
    }
}




#endif
