#include "ultrassonic.hpp"

namespace Ultrassonic {

    int WaterLevel;

    void setup() {
        pinMode(TRIG, OUTPUT);
        pinMode(ECHO, INPUT);
        long t = pulseIn(ECHO, HIGH);
        int distance = t / 29 / 2;

    }

    void Task_Measure_Water(void *parameters){
        int water_level;
        while (1)
        {        
        // TODO: desabilitar interrupções para calculo correto do nível
        measureWaterLevel(&water_level);
        #if PRINT_DEBUG
        Serial.print(F("W:mediu "));
        Serial.println(water_level);
        #endif
        SetWaterLevel(water_level);
        vTaskDelay(MEASURE_WATER_PERIOD/portTICK_PERIOD_MS);
        }
    }

    void SetWaterLevel(int water_level)
    {
        xSemaphoreTake(xMutex_Var_Water,portMAX_DELAY );
        WaterLevel = water_level;
        xSemaphoreGive(xMutex_Var_Water);
    }

    void measureWaterLevel(int *water_level) {
        digitalWrite(TRIG, LOW);
        vTaskDelay(4/portTICK_PERIOD_MS);
        digitalWrite(TRIG, HIGH);
        vTaskDelay(10/portTICK_PERIOD_MS);
        digitalWrite(TRIG, LOW);
        long t = pulseIn(ECHO, HIGH);
        int distance = t / 29 / 2;

        *water_level = (MaxLevel - distance);
    }

    int GetWaterLevel(){
        int water_level;
        xSemaphoreTake(xMutex_Var_Water,portMAX_DELAY );
        water_level = WaterLevel;
        xSemaphoreGive(xMutex_Var_Water);
        return water_level;
    }

}