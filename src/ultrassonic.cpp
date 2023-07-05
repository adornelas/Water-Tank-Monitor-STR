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
        // Serial.println(F("W:Medindo a agua"));
        measureWaterLevel(&water_level);
        Serial.print(F("W:mediu "));
        Serial.println(water_level);
        SetWaterLevel(water_level);
        // Serial.println(F("W:Delay"));
        // Serial.println(F("W:Final da task measure water"));
        vTaskDelay(1000/portTICK_PERIOD_MS);
        }
    }

    void SetWaterLevel(int water_level)
    {
        // Obtains the Global Variable Mutex
        xSemaphoreTake(xMutex_Var_Water,portMAX_DELAY );
        WaterLevel = water_level;
        // Serial.println(F("W:atribuiu"));
        // Releases the Global Variable Mutex
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

    void getWaterLevel(int *water_level){

        xSemaphoreTake(xMutex_Var_Water,portMAX_DELAY );
        *water_level = WaterLevel;
        // Releases the Global Variable Mutex
        xSemaphoreGive(xMutex_Var_Water);
    }

}