#include "main.h"

void BlinkTask(void *parameter)
{
    while(1)
    {
        digitalWrite(LED, HIGH);
        vTaskDelay(pdMS_TO_TICKS(1000));
        digitalWrite(LED, LOW);
        vTaskDelay(pdMS_TO_TICKS(1000));
        Serial.print("Free stack space: ");
        Serial.println(uxTaskGetStackHighWaterMark(NULL));
    }
}

void startBlinkTask()
{
    xTaskCreatePinnedToCore(BlinkTask, "BlinkTask", 800, NULL, 1, NULL, 1);
}