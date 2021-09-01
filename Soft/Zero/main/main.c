#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"


#include <stdlib.h>
#include "my_gui.h"
#include "nets.h"

TaskHandle_t const Gui_Hand;
const char* const Task_Name0 = "My_Gui";

TaskHandle_t const Net_Hand;
const char* const Task_Name1 = "My_Net";

QueueHandle_t Queuedata = NULL;
SemaphoreHandle_t Startnet = NULL;

void app_main(void)
{
  Startnet = xSemaphoreCreateBinary();
  if(Startnet == NULL) printf("Semaphore err\n");
  Queuedata = xQueueCreate(1,sizeof(infor));
  if(Queuedata == NULL) printf("Queue err\n");
  xTaskCreatePinnedToCore(vTaskGui,Task_Name0,1024*4,NULL,1,Gui_Hand,1);
  xTaskCreatePinnedToCore(vTaskNet,Task_Name1,1024*4,NULL,1,Net_Hand,0);

}
