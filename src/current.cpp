#include "current.hpp"

namespace Current {

    SemaphoreHandle_t xMutex_Var_Current;
    float CurrentValue;
    ACS712 sensor(ACS712_05B, CURRENT_GPIO);

    void setup() {
        // TODO: Garantir que motor esteja desligado neste setup
        Serial.println("Calibrating electric current sensor... Ensure that no current flows through the sensor at this moment");
        sensor.calibrate();
        xTaskCreate(Task_MeasureCurrent, "Measure_Current", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + 1, NULL);   
    }

    void Task_MeasureCurrent(void *parameters){
        int current_value;
        // TODO: desabilitar interrupções para calculo correto do nível
        measureCurrentValue(&current_value);
        SetCurrentValue(current_value);
    }

    void SetCurrentValue(int current_value)
    {
        // Obtains the Global Variable Mutex
        xSemaphoreTake(xMutex_Var_Current,portMAX_DELAY );
        CurrentValue = current_value;
        // Releases the Global Variable Mutex
        xSemaphoreGive(xMutex_Var_Current);
    }

    void measureCurrentValue(int *current_value) {
        float I = sensor.getCurrentDC();

        *current_value = I;
    }

}