#ifndef _MPU6050IIC_H
#define _MPU6050IIC_H

#include <string.h>
#include <math.h>

#include "driver/i2c.h"
#include "esp_log.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"

typedef struct iic_bus
{
    int sda_io_num;
    int scl_io_num;
    bool sda_pullup_en;
    bool scl_pullup_en;
}iic_bus;

typedef struct mpu_handle_struct
{
    iic_bus mpu_bus;
    i2c_port_t i2c_num;
    uint32_t clk_speed;
    uint8_t addr;

}mpu_handle;

void Mpu6050_Init(mpu_handle *dev);
esp_err_t Mpu6050_Write(mpu_handle *dev,unsigned char slave_addr, unsigned char reg_addr,
                unsigned char length, unsigned char const *data);
esp_err_t Mpu6050_Read(mpu_handle *dev,unsigned char slave_addr, unsigned char reg_addr,
               unsigned char length,const unsigned char *data);

//about MPUDMP
#define DEFAULT_MPU_HZ  (100)
#define q30  1073741824.0f
#define g    9.8

void Mpu6050_Delay_ms(unsigned long num_ms);
void mget_ms(unsigned long *time);

extern mpu_handle MPU;
#define Mpu6050_Dmp_Write(slave_addr,reg_addr,length,data)    Mpu6050_Write(&MPU,(slave_addr),(reg_addr),(length),(data))
#define Mpu6050_Dmp_Read(slave_addr,reg_addr,length,data)     Mpu6050_Read(&MPU,(slave_addr),(reg_addr),(length),(data))
void Mpu_Dmp_Init(void);
esp_err_t mpu_dmp_get_data(float *pitch,float *roll,float *yaw);
esp_err_t MPU_Get_Temperature(float *data);
esp_err_t MPU_Get_Accelerometer(float *data,unsigned char *fsr);
esp_err_t MPU_Get_Gyroscope(float *data,unsigned short *fsr);

 

#endif