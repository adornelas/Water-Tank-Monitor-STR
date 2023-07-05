#include "motorsensing.hpp"

namespace MotorSensing {

    motorInfoStruct motorInfo;

    ACS712 sensor(ACS712_05B, CURRENT_GPIO);
    DHT dht(TEMPERATURE_GPIO, DHT11);

    void setup() {
        // TODO: Garantir que motor esteja desligado neste setup
        #if PRINT_DEBUG
        Serial.println("Calibrating electric current sensor... Ensure that no current flows through the sensor at this moment");
        #endif
        sensor.calibrate();
        dht.begin();
    }

    void Task_MeasureMotor(void *parameters){
        int current_value;
        motorInfoStruct motor_info;
        while (1)
        {
            // TODO: desabilitar interrupções para calculo correto dos valores
            measureMotor(&motor_info);
            #if PRINT_DEBUG
            Serial.print(F("Temp: "));
            Serial.println(motor_info.temperature);
            Serial.print(F(" Corr: "));
            Serial.println(motor_info.current);
            #endif
            SetMotorInfoValue(motor_info.current, motor_info.temperature);
            vTaskDelay(MEASURE_MOTOR_PERIOD/portTICK_PERIOD_MS);
        }
    }

    void SetMotorInfoValue(float current_value, float temperatura_value)
    {
        xSemaphoreTake(xMutex_Var_MotorInfo,portMAX_DELAY );
        motorInfo.current = current_value;
        motorInfo.temperature = temperatura_value;
        xSemaphoreGive(xMutex_Var_MotorInfo);
    }

    void measureMotor(motorInfoStruct *motor_info) {
        float I;
        float t;
        I = sensor.getCurrentDC();
        // TODO: garantir que as leituras sejam feitas com ao menos 2s de espaçamento
        t = dht.readTemperature();
        if (isnan(t)) {
            #if PRINT_DEBUG
            Serial.println(F("Failed to read from DHT sensor!"));
            #endif
        } else {
            motor_info->temperature = t;
        }
        motor_info->current = I;
    }
   
    motorInfoStruct getMotorInfoValue()
    {
        motorInfoStruct motor_info;
        xSemaphoreTake(xMutex_Var_MotorInfo,portMAX_DELAY );
        motor_info = motorInfo;
        xSemaphoreGive(xMutex_Var_MotorInfo);
        return motor_info;
    }

}