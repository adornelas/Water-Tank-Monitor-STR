#include <Arduino.h>

#include "ultrassonic.hpp"

SemaphoreHandle_t xMutex_Var = NULL;
int WaterLevel;

void setup() {
  Serial.begin(115200);
  Serial.printf("\n\rSistema de Monitoramento de Reservatório\n\r");
  
  xMutex_Var = xSemaphoreCreateMutex();
  if(xMutex_Var == NULL)
  {
    Serial.printf("\n\rFalha em criar o Mutex para variavel global");
  }
}

void loop() {
  Serial.printf("\n\rSupende tarefa LOOP");
  vTaskSuspend(NULL);
}