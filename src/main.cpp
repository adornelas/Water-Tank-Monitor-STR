#include <Arduino.h>
#include <TimeLib.h>

#include "motorsensing.hpp"
#include "ultrassonic.hpp"
#include "pins.h"
#include "buttons.hpp"
#include "connection.hpp"
#include "lcd.hpp"
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

void LigaMotor(){
  digitalWrite(RELAY_PIN, LOW);
}

void DesligaMotor(){
  digitalWrite(RELAY_PIN, HIGH);
}

void Task_Pump(void *parameters){
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
      // Modo automático, mas apertado um número impar de vezes
      if(stateBuffer.State == MANUAL_MODE && !(stateBuffer.timesPressed % 2)){
          DesligaMotor();
        }
    }
    vTaskDelay(PUMP_PERIOD/portTICK_PERIOD_MS);
  }
  
}

void Task_Upload_Status(void *parameters){
  MotorSensing::motorInfoStruct motor_info;
  int water_level;

  while(1){
    water_level = Ultrassonic::GetWaterLevel();

    motor_info =  MotorSensing::getMotorInfoValue();

    if(WiFi.status() == WL_CONNECTED){
      Connection::uploadInfos(&motor_info.current, &motor_info.temperature, &water_level);
    }
    else
    {
      #if PRINT_DEBUG
      Serial.println("Desconectado da internet");
      #endif
    }

    vTaskDelay(UPLOAD_STATUS_PERIOD*SEND_INTERVAL_TIME/portTICK_PERIOD_MS);
  }
  
}

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
  Connection::setup();
  LCD::setup();

  xTaskCreate(Ultrassonic::Task_Measure_Water, "Measure_Water", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + MEASURE_WATER_PRIORITY, NULL);   
  xTaskCreate(MotorSensing::Task_MeasureMotor, "Measure_MotorInfo", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + MEASURE_MOTOR_PRIORITY, NULL);   
  xTaskCreate(Task_Pump, "Pump", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + PUMP_PRIORITY, NULL);
  xTaskCreate(Button::Task_HandleAutomatic, "Handle_Buttons", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + HANDLE_AUTOMATIC_PRIORITY, NULL);   
  xTaskCreate(Button::Task_HandleManual, "Handle_Buttons", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + HANDLE_MANUAL_PRIORITY, NULL);   
  xTaskCreate(Button::Task_HandleStop, "Handle_Buttons", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + HANDLE_STOP_PRIORITY, NULL);   
  xTaskCreate(Task_Upload_Status, "Task_Upload_Status", configMINIMAL_STACK_SIZE * 8, NULL, tskIDLE_PRIORITY + UPLOAD_STATUS_PRIORITY, NULL);
  xTaskCreate(LCD::Task_LCD, "LCD", configMINIMAL_STACK_SIZE * 8, NULL, tskIDLE_PRIORITY + LCD_PRIORITY, NULL);
  
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
}

void loop() {
  #if PRINT_DEBUG
  Serial.printf("\n\rSupende tarefa LOOP");
  #endif
  vTaskSuspend(NULL);
}