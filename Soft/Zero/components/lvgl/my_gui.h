#ifndef MY_GUI_H
#define MY_GUI_H

#include <string.h>
#include <stdio.h>

#include "esp_timer.h"
#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"

#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lv_port_fs.h"


void Gui_Init(void);
void Gui_Start(void);
void vTaskGui( void * pvParameters );

#endif