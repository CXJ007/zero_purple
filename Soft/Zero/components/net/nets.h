#ifndef NET_H
#define NET_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_http_client.h"
#include <string.h>
#include <stdlib.h>
#include "cJSON.h"

typedef struct{
    char month[3];
    char day[3];
}future;

typedef struct{
    uint8_t weather[3];
    uint8_t temperature[3];
    uint8_t humidity[3];
    future fdata[3];

    uint8_t time[3];
    char year[5];
    char month[3];
    char day[3];
    uint8_t flag[4];  //0秒的闪烁 1日期更新  2 控制wife  3天气
}infor;

esp_err_t wifi_init_sta(void);
void weather_get(infor *dat);
void time_get(infor *dat);
void vTaskNet( void * pvParameters );
#endif