#include "ultrassonic.hpp"

namespace Ultrassonic {

    SemaphoreHandle_t xMutex_Var_Water;
    int WaterLevel;

    void setup() {
        pinMode(trig, OUTPUT);
        pinMode(echo, INPUT);
        long t = pulseIn(echo, HIGH);
        int distance = t / 29 / 2;

        // Cria tarefa do sensor
        xTaskCreate(Task_Measure_Water, "Measure_Water", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + 1, NULL);   
    }

    void Task_Measure_Water(void *parameters){
        int water_level;
        // TODO: desabilitar interrupções para calculo correto do nível
        measureWaterLevel(&water_level);
        SetWaterLevel(water_level);
    }

    void SetWaterLevel(int water_level)
    {
        // Obtains the Global Variable Mutex
        xSemaphoreTake(xMutex_Var_Water,portMAX_DELAY );
        WaterLevel = water_level;
        // Releases the Global Variable Mutex
        xSemaphoreGive(xMutex_Var_Water);
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