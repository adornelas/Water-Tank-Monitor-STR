#include "ultrassonic.hpp"

namespace Ultrassonic {

    void setup() {
        pinMode(trig, OUTPUT);
        pinMode(echo, INPUT);
        long t = pulseIn(echo, HIGH);
        int distance = t / 29 / 2;

        // Cria tarefa do sensor
        xTaskCreate(Task_Measure, "Measure", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + 1, NULL);   
    }

    void Task_Measure(void *parameters){
        int water_level;
        // TODO: desabilitar interrupções para calculo correto do nível
        measureWaterLevel(&water_level);
        SetWaterLevel(water_level);
    }

    void SetWaterLevel(int water_level)
    {
        // Obtem o Mutex Variavel Global
        WaterLevel = water_level;
        // libera o Mutex Variavel Global
    }

    void measureWaterLevel(int *water_level) {
        digitalWrite(trig, LOW);
        delayMicroseconds(4);
        digitalWrite(trig, HIGH);
        delayMicroseconds(10);
        digitalWrite(trig, LOW);
        long t = pulseIn(echo, HIGH);
        int distance = t / 29 / 2;

        *water_level = (MaxLevel - distance);
    }

}

// #include <Arduino.h>

// #include <freertos/semphr.h>
// #include <freertos/task.h>
// #define WAIT_TIME 2

// SemaphoreHandle_t xMutex_Ultrassonic = NULL;

// void le(){
//   // tempo da ultima leitura do sensor
//   static uint32_t last_time = 0; 
 
//   if( (millis() - last_time) >= WAIT_TIME)
//   {
//     last_time = millis();
//     xSemaphoreGiveFromISR(xSemaphore_Pulso, (BaseType_t)(pdFALSE));
//   }
// }