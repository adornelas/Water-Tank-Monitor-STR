#include "ultrassonic.hpp"

namespace Ultrassonic {

    void setup() {
        pinMode(trig, OUTPUT);
        pinMode(echo, INPUT);
        long t = pulseIn(echo, HIGH);
        int distance = t / 29 / 2;
        
    }

    int measureWaterLevel() {
        digitalWrite(trig, LOW);
        delayMicroseconds(4);
        digitalWrite(trig, HIGH);
        delayMicroseconds(10);
        digitalWrite(trig, LOW);
        long t = pulseIn(echo, HIGH);
        int distance = t / 29 / 2;

        return (MaxLevel - distance);
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