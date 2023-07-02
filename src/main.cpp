#include <Arduino.h>

#include "ultrassonic.hpp"

void setup() {
  Serial.begin(115200);
  Serial.printf("\n\rSistema de Monitoramento de Reservatório\n\r");
}

void loop() {
  Serial.printf("\n\rSupende tarefa LOOP");
  vTaskSuspend(NULL);
}