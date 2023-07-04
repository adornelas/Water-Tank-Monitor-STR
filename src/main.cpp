#include <Arduino.h>

#include "motorsensing.hpp"
#include "ultrassonic.hpp"
#include "pins.h"
#include "buttons.hpp"
// #include "connection.hpp"

SemaphoreHandle_t xMutex_Var_Water =  NULL;
SemaphoreHandle_t xMutex_Var_MotorInfo = NULL;
SemaphoreHandle_t xSemaphore_ManualButton = NULL;
SemaphoreHandle_t xSemaphore_AutomaticButton = NULL;
SemaphoreHandle_t xSemaphore_StopButton = NULL;
SemaphoreHandle_t xMutex_SystemState = NULL;

int WaterLevel;
MotorSensing::motorInfoStruct motorInfo;
srcSystem SystemState;


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
  // MotorSensing::motorInfoStruct motor_info;
  // int water__level;
  srcSystem stateBuffer;
  while(1)
  {
    Serial.println(F("Começou task_pump"));
    stateBuffer = Button::GetState();
    Serial.print(F("     Pegou state: "));    
    Serial.print(stateBuffer.State);
    Serial.println(stateBuffer.timesPressed);
    // Le os dados das informações do motor
    // motor_info = GetMotorInfoValue();
    // water__level = GetWaterLevel();
    
    // Aciona o motor de acordo com o modo de funcionamento
    //
    vTaskDelay(2000/portTICK_PERIOD_MS);
    // digitalWrite(RELAY_PIN, LOW);

  }
  
}


// void Upload_Status(void *parameters){
//   float current;
//   float temperature;
//   int level;

//   while(1){
//     xSemaphoreTake(xMutex_Var_Water,portMAX_DELAY );
//     level = WaterLevel;
//     // Releases the Global Variable Mutex
//     xSemaphoreGive(xMutex_Var_Water);

//     xSemaphoreTake(xMutex_Var_MotorInfo,portMAX_DELAY );
//     current = MotorSensing::motorInfo.current;
//     temperature = MotorSensing::motorInfo.temperature;
//     xSemaphoreGive(xMutex_Var_MotorInfo);

//     Serial.print(F("CONNECTION:"));
//     Serial.print(F("Current:"));
//     Serial.print(current);

//     Serial.print(F("CONNECTION:"));
//     Serial.print(F("level:"));
//     Serial.print(level);

//     Serial.print(F("CONNECTION:"));
//     Serial.print(F("temperature:"));
//     Serial.print(temperature);

//     Connection::uploadInfos(current, temperature, level);

//     vTaskDelay(1000*SEND_INTERVAL_TIME);
//   }
  
// }

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
  xMutex_SystemState = xSemaphoreCreateMutex();
  if(xMutex_SystemState == NULL){
      Serial.printf("\n\rFalha em criar o Mutex para o botão desejado");
  }

  MotorSensing::setup();
  Ultrassonic::setup();
  Button::setup();
  // Connection::setup();

  xTaskCreate(Ultrassonic::Task_Measure_Water, "Measure_Water", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + 3, NULL);   
  xTaskCreate(MotorSensing::Task_MeasureMotor, "Measure_MotorInfo", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + 2, NULL);   
  xTaskCreate(Task_Pump, "Pump", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + 1, NULL);
  xTaskCreate(Button::Task_HandleAutomatic, "Handle_Buttons", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + 4, NULL);   
  xTaskCreate(Button::Task_HandleManual, "Handle_Buttons", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + 5, NULL);   
  xTaskCreate(Button::Task_HandleStop, "Handle_Buttons", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + 6, NULL);   
  // xTaskCreate(Upload_Status, "Upload_Status", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + 6, NULL);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);

}

void loop() {
  Serial.printf("\n\rSupende tarefa LOOP");
  vTaskSuspend(NULL);
}