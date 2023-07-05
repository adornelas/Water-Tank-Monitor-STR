#include "motorsensing.hpp"

namespace MotorSensing {

    motorInfoStruct motorInfo;

    ACS712 sensor(ACS712_05B, CURRENT_GPIO);
    DHT dht(TEMPERATURE_GPIO, DHT22);

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
        struct timeval t0,t1, dt;

        while (1)
        {
            gettimeofday(&t0,NULL);
            // TODO: desabilitar interrupções para calculo correto dos valores
            measureMotor(&motor_info);
            #if PRINT_DEBUG
            Serial.print(F("Temperatura: "));
            Serial.println(motor_info.temperature);
            Serial.print(F("Corrente: "));
            Serial.println(motor_info.current);
            #endif
            SetMotorInfoValue(motor_info.current, motor_info.temperature);
            gettimeofday(&t1,NULL);
            timersub(&t1, &t0, &dt);
            Serial.print("Task_MeasureMotor:");
            Serial.print(dt.tv_sec);
            Serial.print(".");
            Serial.println(dt.tv_usec);
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
        t = dht.readTemperature();
        if (isnan(t)) {
            #if PRINT_DEBUG
            Serial.println(F("Failed to read from DHT sensor!"));
            #endif
            motor_info->temperature = -1.0;
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