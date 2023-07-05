#include "pump.hpp"

namespace Pump {

    void setup() {
        pinMode(RELAY_PIN, OUTPUT);
        digitalWrite(RELAY_PIN, HIGH);
    }

    void Task_HandlePump(void *parameters){
        srcSystem stateBuffer;
        MotorSensing::motorInfoStruct motor_info;
        int water_level;

        while(1)
        {
            stateBuffer = Button::GetState();
            water_level = Ultrassonic::GetWaterLevel();
            motor_info =  MotorSensing::getMotorInfoValue();
            #if PRINT_DEBUG
            Serial.print("MODO: ");
            Serial.print(stateBuffer.State);
            Serial.println(stateBuffer.timesPressed);
            #endif

            if(stateBuffer.State == STOP_MODE || 
            motor_info.temperature > 26.0 || 
            abs(motor_info.current) > 1.0){

                DesligaMotor();   
            }
            
            else{
            if(stateBuffer.State == AUTOMATIC_MODE && water_level > 10){
                DesligaMotor();
            }

            if(stateBuffer.State == AUTOMATIC_MODE && 
                water_level < 10 &&
                motor_info.temperature < 26.0 &&
                abs(motor_info.current) < 1.0){

                LigaMotor();
            }
            
            if(stateBuffer.State == MANUAL_MODE && (stateBuffer.timesPressed % 2)){
                LigaMotor();
                }

            if(stateBuffer.State == MANUAL_MODE && !(stateBuffer.timesPressed % 2)){
                DesligaMotor();
                }
            }
            vTaskDelay(PUMP_PERIOD/portTICK_PERIOD_MS);
        }
  
}


    void LigaMotor(){
        digitalWrite(RELAY_PIN, LOW);
    }

    void DesligaMotor(){
        digitalWrite(RELAY_PIN, HIGH);
    }


}