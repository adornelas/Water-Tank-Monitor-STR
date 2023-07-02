#include <Arduino.h>

#include "ultrassonic.hpp"

int count1 = 0;
int count2 = 0;

void task1(void * parameters){
  for(;;){
    Serial.print("Task 1 counter: ");
    Serial.println(count1++);
    
    vTaskDelay(500/portTICK_PERIOD_MS);
  }
}

void task2(void * parameters){
  for(;;){
    Serial.print("Task 2 counter: ");
    Serial.println(count2++);
    
    vTaskDelay(500/portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(9600);

  xTaskCreate(
    task1,
    "Task 1",
    1000,
    NULL,
    1,
    NULL
  );

  xTaskCreate(
    task2,
    "Task 1",
    1000,
    NULL,
    2,
    NULL
  );

}

void loop() {
  
}