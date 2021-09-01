#ifndef _SD_H
#define _SD_H


#include "driver/sdspi_host.h"
#include "esp_log.h"
#include "esp_err.h"
//#include "driver/spi_common.h"
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
//#include "driver/sdmmc_host.h"

#define MOUNT_POINT "/LVGL"

void SdInit(void);

#endif
