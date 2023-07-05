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
        struct timeval t0,t1, dt;

        while (1)
        {
        gettimeofday(&t0,NULL);
        // TODO: desabilitar interrupções para calculo correto do nível
        measureWaterLevel(&water_level);
        #if PRINT_DEBUG
        Serial.print(F("Nível de água: "));
        Serial.println(water_level);
        #endif
        SetWaterLevel(water_level);
        gettimeofday(&t1,NULL);
        timersub(&t1, &t0, &dt);
        Serial.print("Task_Measure_Water:");
        Serial.print(dt.tv_sec);
        Serial.print(".");
        Serial.println(dt.tv_usec);
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