#include <Arduino.h>

#include "ultrassonic.hpp"

SemaphoreHandle_t xMutex_Var_Water = NULL;
int WaterLevel;

void setup() {
  Serial.begin(115200);
  Serial.printf("\n\rSistema de Monitoramento de Reservatório\n\r");
  
  xMutex_Var_Water = xSemaphoreCreateMutex();
  if(xMutex_Var_Water == NULL)
  {
    Serial.printf("\n\rFalha em criar o Mutex para variavel global");
  }
  Ultrassonic::setup();

}

void loop() {
  Serial.printf("\n\rSupende tarefa LOOP");
  vTaskSuspend(NULL);
}