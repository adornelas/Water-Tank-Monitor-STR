#include <Arduino.h>

#include "motorsensing.hpp"
#include "ultrassonic.hpp"
#include "pins.h"
// #include "button.hpp"

SemaphoreHandle_t xMutex_Var_Water =  NULL;
SemaphoreHandle_t xMutex_Var_MotorInfo = NULL;
int WaterLevel;
MotorSensing::motorInfoStruct motorInfo;

MotorSensing::motorInfoStruct GetMotorInfoValue()
{
  MotorSensing::motorInfoStruct info_struct;
  // Obtains the Global Variable Mutex
  xSemaphoreTake(xMutex_Var_MotorInfo,portMAX_DELAY );
  info_struct.current = motorInfo.current;
  info_struct.temperature =  motorInfo.temperature ;
  // Releases the Global Variable Mutex
  xSemaphoreGive(xMutex_Var_MotorInfo);
  return info_struct;
}

int GetWaterLevel()
{
  int water_level;
  // Obtains the Global Variable Mutex
  xSemaphoreTake(xMutex_Var_Water,portMAX_DELAY );
  WaterLevel = water_level;
  // Releases the Global Variable Mutex
  xSemaphoreGive(xMutex_Var_Water);
  return water_level;
}

void Task_Pump(void *parameters){
  MotorSensing::motorInfoStruct motor_info;
  int water__level;
  // srcSystem stateBuffer;
  while(1){
    // stateBuffer = Button::GetButtonState();
    
    // printf(stateBuffer.State);
    // Le os dados das informações do motor
    motor_info = GetMotorInfoValue();
    water__level = GetWaterLevel();
    Serial.println(water__level);
    // Aciona o motor de acordo com o modo de funcionamento
    // 
  }
  
}


void setup() {
  Serial.begin(115200);
  Serial.printf("\n\rSistema de Monitoramento de Reservatório\n\r");
  
  xMutex_Var_Water = xSemaphoreCreateMutex();
  if(xMutex_Var_Water == NULL)
  {
    Serial.printf("\n\rFalha em criar o Mutex para variavel global de nível de água");
  }
  xMutex_Var_MotorInfo = xSemaphoreCreateMutex();
  if(xMutex_Var_MotorInfo == NULL)
  {
    Serial.printf("\n\rFalha em criar o Mutex para variavel global de informações do motor");
  }
  MotorSensing::setup();
  Ultrassonic::setup();
  xTaskCreate(Ultrassonic::Task_Measure_Water, "Measure_Water", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + 1, NULL);   
  xTaskCreate(MotorSensing::Task_MeasureMotor, "Measure_Current", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + 2, NULL);   

  // Button::setup();

  // Button::SetManualButtom(MANUAL_BUTTON);
  // Button::SetAutomaticButtom(AUTOMATIC_BUTTON);
  // Button::SetStopButtom(STOP_BUTTON);

  // pinMode(RELAY_PIN, OUTPUT);
  // xTaskCreate(Task_Pump, "Measure_Current", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + 1, NULL);   
  // digitalWrite(RELAY_PIN, LOW);

}

void loop() {
  Serial.printf("\n\rSupende tarefa LOOP");
  vTaskSuspend(NULL);
}