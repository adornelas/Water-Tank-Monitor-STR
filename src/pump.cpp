#include "pump.hpp"

namespace Pump {

    motorInfoStruct motorInfo;

    void setup() {
        // TODO: Garantir que motor esteja desligado neste setup
        #if PRINT_DEBUG
        Serial.println("Calibrating electric current sensor... Ensure that no current flows through the sensor at this moment");
        #endif
    }

    void Task_HandlePump(void *parameters){
        int current_value;
        motorInfoStruct motor_info;
        while (1)
        {
            vTaskDelay(MEASURE_MOTOR_PERIOD/portTICK_PERIOD_MS);
        }
    }

}