#include "motorsensing.hpp"

namespace MotorSensing {

    SemaphoreHandle_t xMutex_Var_MotorInfo;
    motorInfoStruct motorInfo;

    ACS712 sensor(ACS712_05B, CURRENT_GPIO);
    DHT dht(TEMPERATURE_GPIO, DHT11);

    void setup() {
        // TODO: Garantir que motor esteja desligado neste setup
        Serial.println("Calibrating electric current sensor... Ensure that no current flows through the sensor at this moment");
        sensor.calibrate();
        dht.begin();

        xTaskCreate(Task_MeasureMotor, "Measure_Current", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + 1, NULL);   
    }

    void Task_MeasureMotor(void *parameters){
        int current_value;
        motorInfoStruct motor_info;
        // TODO: desabilitar interrupções para calculo correto dos valores
        measureMotor(&motor_info);
        SetMotorInfoValue(motor_info.current, motor_info.temperature);
    }

    void SetMotorInfoValue(float current_value, float temperatura_value)
    {
        // Obtains the Global Variable Mutex
        xSemaphoreTake(xMutex_Var_MotorInfo,portMAX_DELAY );
        motorInfo.current = current_value;
        motorInfo.temperature = temperatura_value;
        // Releases the Global Variable Mutex
        xSemaphoreGive(xMutex_Var_MotorInfo);
    }

    void measureMotor(motorInfoStruct *motor_info) {
        float I = sensor.getCurrentDC();
        // TODO: garantir que as leituras sejam feitas com ao menos 2s de espaçamento
        float t = dht.readTemperature();
        if (isnan(t)) {
            Serial.println(F("Failed to read from DHT sensor!"));
        } else {
            motor_info->temperature = t;
        }
        motor_info->current = I;
    }

}