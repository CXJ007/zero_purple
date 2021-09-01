#include "nets.h"

//esp_netif_t * my_sta = NULL;


extern QueueHandle_t Queuedata;
extern SemaphoreHandle_t Startnet;

#define EXAMPLE_ESP_WIFI_SSID      "555"
#define EXAMPLE_ESP_WIFI_PASS      "123456789"
#define EXAMPLE_ESP_MAXIMUM_RETRY  10

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1
static EventGroupHandle_t s_wifi_event_group;
static const char *TAG = "wifi station";

static int s_retry_num = 0;
static void event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY) {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "retry to connect to the AP");
        } else {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        ESP_LOGI(TAG,"connect to the AP fail");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

esp_err_t wifi_init_sta(void)
{
    esp_err_t err = 0;
    s_wifi_event_group = xEventGroupCreate();
    err += esp_netif_init();
    err += esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();//要保存这个return删除wife时要删除
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    err += esp_wifi_init(&cfg);
    if(err != 0)printf("%d\n",err);
    if(err != 0) {ESP_LOGI(TAG, "init err");return 1;}
    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    err += esp_event_handler_instance_register(WIFI_EVENT,
                                        ESP_EVENT_ANY_ID,
                                        &event_handler,
                                        NULL,
                                        &instance_any_id);
    err += esp_event_handler_instance_register(IP_EVENT,
                                        IP_EVENT_STA_GOT_IP,
                                        &event_handler,
                                        NULL,
                                        &instance_got_ip);
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            .password = EXAMPLE_ESP_WIFI_PASS,
            /* Setting a password implies station will connect to all security modes including WEP/WPA.
             * However these modes are deprecated and not advisable to be used. Incase your Access point
             * doesn't support WPA2, these mode can be enabled by commenting below line */
	        .threshold.authmode = WIFI_AUTH_WPA2_PSK,

            .pmf_cfg = {
                .capable = true,
                .required = false
            },
        },
    };
    err += esp_wifi_set_mode(WIFI_MODE_STA);
    err += esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    err += esp_wifi_start();
    if(err != 0) {ESP_LOGI(TAG, "wife start err");return 1;}
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
            WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
            pdFALSE,
            pdFALSE,
            portMAX_DELAY);
    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
                 EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
    } else if (bits & WIFI_FAIL_BIT) {
        ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s",
                 EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
    } else {
        ESP_LOGE(TAG, "UNEXPECTED EVENT");
    }
    esp_event_handler_instance_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, instance_got_ip);
    esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, instance_any_id);
    vEventGroupDelete(s_wifi_event_group);
    return 0;
}

esp_err_t _http_event_handle(esp_http_client_event_t *evt)
{
    switch(evt->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGI(TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGI(TAG, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_HEADER");
            printf("%.*s", evt->data_len, (char*)evt->data);
            break;
        case HTTP_EVENT_ON_DATA:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            // if (!esp_http_client_is_chunked_response(evt->client)) {
            //     printf("%.*s", evt->data_len, (char*)evt->data);
            // }
            //分块在这里
            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_FINISH");
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
            break;
    }
    return ESP_OK;
}

esp_err_t weather_json(char *buf,infor *dat)
{
    uint8_t i=0,j=0;
    char buffer[40] = {0};
    char *del = "-";
    char *p = NULL;
    cJSON *cjson_hand = cJSON_Parse(buf);
    if(cjson_hand == NULL){
        cJSON_Delete(cjson_hand);
        //printf("0");
        return ESP_FAIL;
    }
    cJSON *cjson_results = cJSON_GetObjectItem(cjson_hand,"results");
    if(cjson_results == NULL){
        cJSON_Delete(cjson_hand);
        //printf("1");
        return ESP_FAIL;
    }
    cJSON *cjson_location = cJSON_GetArrayItem(cjson_results, 0);
    if(cjson_location == NULL){
        cJSON_Delete(cjson_hand);
        //printf("2");
        return ESP_FAIL;
    }
    cJSON *cjson_daily = cJSON_GetObjectItem(cjson_location,"daily");
    if(cjson_daily == NULL){
        cJSON_Delete(cjson_hand);
        //printf("3");
        return ESP_FAIL;
    }
    uint8_t num = cJSON_GetArraySize(cjson_daily);
    cJSON *cjson_infor = NULL;
    cJSON *cjson_code_day = NULL;
    cJSON *cjson_high = NULL;
    cJSON *cjson_low = NULL;
    cJSON *cjson_humidity =NULL;
    cJSON *cjson_date = NULL;
    for(i=0;i<num;i++){
        j = 0;
        cjson_infor = cJSON_GetArrayItem(cjson_daily,i);
        if(cjson_infor == NULL){
            cJSON_Delete(cjson_hand);
            //printf("4");
            return ESP_FAIL;
        }
        cjson_code_day = cJSON_GetObjectItem(cjson_infor,"code_day");
        cjson_high = cJSON_GetObjectItem(cjson_infor,"high");
        cjson_low = cJSON_GetObjectItem(cjson_infor,"low");
        cjson_humidity = cJSON_GetObjectItem(cjson_infor,"humidity");
        cjson_date = cJSON_GetObjectItem(cjson_infor,"date");
        //printf("data code  %s\n",cjson_humidity->valuestring);
        dat->weather[i] = atoi(cjson_code_day->valuestring);
        dat->temperature[i] = (atoi(cjson_high->valuestring)+atoi(cjson_low->valuestring))/2;
        dat->humidity[i] = atoi(cjson_humidity->valuestring);
        strcpy(buffer,cjson_date->valuestring);
        //printf("%s\n",buffer);
        p = strtok(buffer,del);
        while(p != NULL){
            //printf("%s     %d\n",p,j);
            switch(j){
                case 1: strcpy(dat->fdata[i].month,p);break; //不要大于2
                case 2: strcpy(dat->fdata[i].day,p);break;
            }
            j++;
            p = strtok(NULL,del);
        }
        //printf("%s  %s\n",dat->fdata[i].month,dat->fdata[i].day);
    }
    cJSON_Delete(cjson_hand);
    return ESP_OK;
}

void weather_get(infor *dat)
{
    esp_http_client_config_t config = {
    .url = "https://api.seniverse.com/v3/weather/daily.json?key=SWMpeKFOuUVOXI5nL&location=yibin&language=en&unit=c&start=0&days=3",
    .event_handler = _http_event_handle,
    .buffer_size = 2048,//足够大数据不分块
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK) {
        uint16_t datnum = esp_http_client_get_content_length(client);
        char buf[datnum];
        memset(buf,0,datnum);
        esp_http_client_read(client,buf,datnum);
        //printf("%s\n",buf);
        esp_http_client_close(client);
        esp_http_client_cleanup(client);
        if(weather_json(buf,dat) == ESP_OK) dat->flag[3] = 1; //更新;
        else dat->flag[3] = 0; //更新
    }else{
        esp_http_client_close(client);
        esp_http_client_cleanup(client);
    }
}

esp_err_t time_json(char *buf,infor *dat)
{
    char buffer[40] = {0};
    char *del = "- :";
    char *p = NULL;
    uint8_t i = 0;
    cJSON *cjson_hand = cJSON_Parse(buf);
    if(cjson_hand == NULL){
        cJSON_Delete(cjson_hand);
        //printf("0");
        return ESP_FAIL;
    }
    cJSON *cjson_sysTime2 = cJSON_GetObjectItem(cjson_hand,"sysTime2");
    if(cjson_sysTime2 == NULL){
        cJSON_Delete(cjson_hand);
        //printf("1");
        return ESP_FAIL;
    }
    strcpy(buffer,cjson_sysTime2->valuestring);
    cJSON_Delete(cjson_hand);
    //printf("%s\n",buffer);
    p = strtok(buffer,del);
    while(p != NULL){
        //printf("%s     %d\n",p,i);
        switch(i){
            case 0: strcpy(dat->year,p);break;
            case 1: strcpy(dat->month,p);break;
            case 2: strcpy(dat->day,p);break;
            case 3: dat->time[0] = atoi(p);break;
            case 4: dat->time[1] = atoi(p);break;
            case 5: dat->time[2] = atoi(p);break;
        }
        i++;
        p = strtok(NULL,del);
    }
    return ESP_OK;
}

void time_get(infor *dat)
{
    esp_http_client_config_t config = {
    .url = "http://quan.suning.com/getSysTime.do",
    .event_handler = _http_event_handle,
    .buffer_size = 2048,//足够大数据不分块
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK) {
        uint16_t datnum = esp_http_client_get_content_length(client);
        char buf[datnum];
        memset(buf,0,datnum);
        esp_http_client_read(client,buf,datnum);
        //printf("%s\n",buf);
        esp_http_client_close(client);
        esp_http_client_cleanup(client);
        if(time_json(buf,dat) == ESP_OK) dat->flag[1] = 1;//日期更新
        else dat->flag[1] = 0;//日期更新
    }else{
        esp_http_client_close(client);
        esp_http_client_cleanup(client);
    }
    //dat->flag[0] = 1;//时间
    //dat->flag[1] = 1;//日期更新
}


void vTaskNet( void * pvParameters )
{
   esp_err_t err;
   nvs_flash_init();
   infor *netinfor = (infor*)malloc(sizeof(infor));
   memset(netinfor,0,sizeof(infor));
   err = wifi_init_sta();
   //esp_wifi_stop();
   // printf("%s\n  %s\n %s\n %d %d  %d",allinfor->year,allinfor->month,allinfor->day,allinfor->time[0],allinfor->time[1],allinfor->time[2]);
   // // printf("         %d   %d   %d \n",allinfor->weather[0],allinfor->weather[1],allinfor->weather[2]);
   // // printf("         %d   %d   %d \n",allinfor->temperature[0],allinfor->temperature[1],allinfor->temperature[2]);
   // // printf("         %d   %d   %d \n",allinfor->humidity[0],allinfor->humidity[1],allinfor->humidity[2]);
   for( ;; )
   {
      if(xSemaphoreTake(Startnet,portMAX_DELAY) == pdTRUE){
         esp_wifi_start();
         esp_wifi_connect();
         if(err == ESP_OK)time_get(netinfor);
         if(err == ESP_OK)weather_get(netinfor);
         esp_wifi_stop();
         netinfor->flag[2] = 1;
         xQueueOverwrite(Queuedata,netinfor);
      }   
   }
}