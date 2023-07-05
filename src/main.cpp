#include <Arduino.h>

#include "motorsensing.hpp"
#include "ultrassonic.hpp"
#include "pins.h"
#include "buttons.hpp"
#include "connection.hpp"
#include "lcd.hpp"
#include "pump.hpp"
#include "RTS_settings.h"

SemaphoreHandle_t xMutex_Var_Water =  NULL;
SemaphoreHandle_t xMutex_Var_MotorInfo = NULL;
SemaphoreHandle_t xSemaphore_ManualButton = NULL;
SemaphoreHandle_t xSemaphore_AutomaticButton = NULL;
SemaphoreHandle_t xSemaphore_StopButton = NULL;
SemaphoreHandle_t xMutex_SystemState = NULL;

int WaterLevel;
MotorSensing::motorInfoStruct motorInfo;
srcSystem SystemState;

void setup() {
  Serial.begin(115200);
  #if PRINT_DEBUG
  Serial.printf("\n\rSistema de Monitoramento de Reservatório\n\r");
  #endif
  
  xMutex_Var_Water = xSemaphoreCreateMutex();
  if(xMutex_Var_Water == NULL)
  {
    #if PRINT_DEBUG
    Serial.printf("\n\rFalha em criar o Mutex para variavel global de nível de água");
    #endif
  }
  xMutex_Var_MotorInfo = xSemaphoreCreateMutex();
  if(xMutex_Var_MotorInfo == NULL)
  {
    #if PRINT_DEBUG
    Serial.printf("\n\rFalha em criar o Mutex para variavel global de informações do motor");
    #endif
  }
  xMutex_SystemState = xSemaphoreCreateMutex();
  if(xMutex_SystemState == NULL){
    #if PRINT_DEBUG
    Serial.printf("\n\rFalha em criar o Mutex para o botão desejado");
    #endif
  }

  MotorSensing::setup();
  Ultrassonic::setup();
  Button::setup();
  Pump::setup();
  Connection::setup();
  LCD::setup();

  xTaskCreate(Ultrassonic::Task_Measure_Water, "Measure_Water", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + MEASURE_WATER_PRIORITY, NULL);   
  xTaskCreate(MotorSensing::Task_MeasureMotor, "Measure_MotorInfo", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + MEASURE_MOTOR_PRIORITY, NULL);   
  xTaskCreate(Pump::Task_HandlePump, "HandlePump", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + PUMP_PRIORITY, NULL);
  xTaskCreate(Button::Task_HandleAutomatic, "Handle_Buttons", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + HANDLE_AUTOMATIC_PRIORITY, NULL);   
  xTaskCreate(Button::Task_HandleManual, "Handle_Buttons", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + HANDLE_MANUAL_PRIORITY, NULL);   
  xTaskCreate(Button::Task_HandleStop, "Handle_Buttons", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + HANDLE_STOP_PRIORITY, NULL);   
  xTaskCreate(Connection::Task_Upload_Status, "Task_Upload_Status", configMINIMAL_STACK_SIZE * 8, NULL, tskIDLE_PRIORITY + UPLOAD_STATUS_PRIORITY, NULL);
  xTaskCreate(LCD::Task_LCD, "LCD", configMINIMAL_STACK_SIZE * 8, NULL, tskIDLE_PRIORITY + LCD_PRIORITY, NULL);
}

void loop() {
  vTaskSuspend(NULL);
}