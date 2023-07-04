#include "motorsensing.hpp"

namespace MotorSensing {

    motorInfoStruct motorInfo;

    ACS712 sensor(ACS712_05B, CURRENT_GPIO);
    DHT dht(TEMPERATURE_GPIO, DHT11);

    void setup() {
        // TODO: Garantir que motor esteja desligado neste setup
        Serial.println("Calibrating electric current sensor... Ensure that no current flows through the sensor at this moment");
        sensor.calibrate();
        dht.begin();
    }

    void Task_MeasureMotor(void *parameters){
        int current_value;
        motorInfoStruct motor_info;
        while (1)
        {
            // TODO: desabilitar interrupções para calculo correto dos valores
            // Serial.println(F("T:Medindo a temperatura"));
            measureMotor(&motor_info);
            // Serial.print(F("T:mediu "));
            Serial.println(motor_info.temperature);
            // Serial.print(F("T:Medindo a corrente:"));
            Serial.println(motor_info.current);
            SetMotorInfoValue(motor_info.current, motor_info.temperature);
            // Serial.println(F("T:setou, Delay"));
            // Serial.println(F("T:Final da task measure tempertture"));
            vTaskDelay(1000/portTICK_PERIOD_MS);
        }
    }

    void SetMotorInfoValue(float current_value, float temperatura_value)
    {
        // Obtains the Global Variable Mutex
        xSemaphoreTake(xMutex_Var_MotorInfo,portMAX_DELAY );
        motorInfo.current = current_value;
        motorInfo.temperature = temperatura_value;
        // Serial.println(F("T:atribuiu"));
        // Releases the Global Variable Mutex
        xSemaphoreGive(xMutex_Var_MotorInfo);
    }

    void measureMotor(motorInfoStruct *motor_info) {
        float I;
        float t;
        I = sensor.getCurrentDC();
        // TODO: garantir que as leituras sejam feitas com ao menos 2s de espaçamento
        t = dht.readTemperature();
        if (isnan(t)) {
            Serial.println(F("Failed to read from DHT sensor!"));
        } else {
            motor_info->temperature = t;
        }
        motor_info->current = I;
    }

    
    void getMotorInfo(float *current_value, float *temperatura_value)
    {
        // Obtains the Global Variable Mutex
        xSemaphoreTake(xMutex_Var_MotorInfo,portMAX_DELAY );
        *current_value = motorInfo.current;
        *temperatura_value = motorInfo.temperature;
        // Serial.println(F("T:atribuiu"));
        // Releases the Global Variable Mutex
        xSemaphoreGive(xMutex_Var_MotorInfo);
    }

}