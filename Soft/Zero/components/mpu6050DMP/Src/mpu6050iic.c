#include "mpu6050iic.h"

// #define LOG_LOCAL_LEVEL ESP_LOG_DEBUG
// static const char *TAG = "mpu6050_iic";

void Mpu6050_Init(mpu_handle *dev)
{
    esp_err_t ret;
    i2c_config_t i2c_conf;
    memset(&i2c_conf,0,sizeof(i2c_config_t));
    i2c_conf.mode = I2C_MODE_MASTER;
    i2c_conf.sda_io_num = dev->mpu_bus.sda_io_num;
    i2c_conf.scl_io_num = dev->mpu_bus.scl_io_num;
    i2c_conf.sda_pullup_en = dev->mpu_bus.sda_pullup_en;
    i2c_conf.scl_pullup_en = dev->mpu_bus.scl_pullup_en;
    i2c_conf.master.clk_speed = dev->clk_speed;
    ret = i2c_param_config(dev->i2c_num,&i2c_conf);
    assert(ret==ESP_OK);
    ret = i2c_driver_install(dev->i2c_num,i2c_conf.mode ,0,0,0);
    assert(ret==ESP_OK);
}

esp_err_t Mpu6050_Write(mpu_handle *dev,unsigned char slave_addr, unsigned char reg_addr,
                unsigned char length, unsigned char const *data)
{
    esp_err_t ret;
    i2c_cmd_handle_t cmd_handle; 
    cmd_handle = i2c_cmd_link_create();
    i2c_master_start(cmd_handle);
    i2c_master_write_byte(cmd_handle,(slave_addr<<1)|I2C_MASTER_WRITE, I2C_MASTER_ACK );
    i2c_master_write_byte(cmd_handle,reg_addr, I2C_MASTER_ACK );
    i2c_master_write(cmd_handle,data,length,I2C_MASTER_ACK);
    i2c_master_stop(cmd_handle);
    ret = i2c_master_cmd_begin(dev->i2c_num, cmd_handle,10/portTICK_PERIOD_MS);
    assert(ret==ESP_OK);
    i2c_cmd_link_delete(cmd_handle);
    return ret;
}

esp_err_t Mpu6050_Read(mpu_handle *dev,unsigned char slave_addr, unsigned char reg_addr,
               unsigned char length,const unsigned char *data)
{
    esp_err_t ret;
    i2c_cmd_handle_t cmd_handle; 
    cmd_handle = i2c_cmd_link_create();
    i2c_master_start(cmd_handle);
    i2c_master_write_byte(cmd_handle,(slave_addr<<1)|I2C_MASTER_WRITE, I2C_MASTER_ACK );
    i2c_master_write_byte(cmd_handle,reg_addr,I2C_MASTER_ACK );
    i2c_master_start(cmd_handle);
    i2c_master_write_byte(cmd_handle,(slave_addr<<1)|I2C_MASTER_READ, I2C_MASTER_ACK );
    i2c_master_read(cmd_handle, data, length, I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd_handle);
    ret = i2c_master_cmd_begin(dev->i2c_num, cmd_handle, 10/portTICK_PERIOD_MS);
    assert(ret==ESP_OK);
    i2c_cmd_link_delete(cmd_handle);
    return ret;
}

void Mpu6050_Delay_ms(unsigned long num_ms)
{
    vTaskDelay(num_ms/portTICK_PERIOD_MS);
}

void mget_ms(unsigned long *time)
{
    //
}

static signed char gyro_orientation[9] = {-1, 0, 0,
                                           0,-1, 0,
                                           0, 0, 1};

static unsigned short inv_row_2_scale(const signed char *row)
{
    unsigned short b;

    if (row[0] > 0)
        b = 0;
    else if (row[0] < 0)
        b = 4;
    else if (row[1] > 0)
        b = 1;
    else if (row[1] < 0)
        b = 5;
    else if (row[2] > 0)
        b = 2;
    else if (row[2] < 0)
        b = 6;
    else
        b = 7;      // error
    return b;
}

static inline unsigned short inv_orientation_matrix_to_scalar(
    const signed char *mtx)
{
    unsigned short scalar;

    /*
       XYZ  010_001_000 Identity Matrix
       XZY  001_010_000
       YXZ  010_000_001
       YZX  000_010_001
       ZXY  001_000_010
       ZYX  000_001_010
     */

    scalar = inv_row_2_scale(mtx);
    scalar |= inv_row_2_scale(mtx + 3) << 3;
    scalar |= inv_row_2_scale(mtx + 6) << 6;


    return scalar;
}

uint8_t run_self_test(void)
{
	int result;
	//char test_packet[4] = {0};
	long gyro[3], accel[3]; 
	result = mpu_run_self_test(gyro, accel);
	if (result == 0x3) 
	{
		/* Test passed. We can trust the gyro data here, so let's push it down
		* to the DMP.
		*/
		float sens;
		unsigned short accel_sens;
		mpu_get_gyro_sens(&sens);
		gyro[0] = (long)(gyro[0] * sens);
		gyro[1] = (long)(gyro[1] * sens);
		gyro[2] = (long)(gyro[2] * sens);
		dmp_set_gyro_bias(gyro);
		mpu_get_accel_sens(&accel_sens);
		accel[0] *= accel_sens;
		accel[1] *= accel_sens;
		accel[2] *= accel_sens;
		dmp_set_accel_bias(accel);
		return 0;
	}else return 1;
}

void Mpu_Dmp_Init(void)
{   
    esp_err_t ret;
    uint8_t i = 0;
    while (mpu_init()!=0){
        i++;
        if(i>100){
            i= 0;
            printf("error\n");
            assert(1);
            return;
        }
    }
    ret = mpu_set_sensors(INV_XYZ_GYRO|INV_XYZ_ACCEL);
    assert(ret==ESP_OK);
    ret = mpu_configure_fifo(INV_XYZ_GYRO|INV_XYZ_ACCEL);
    assert(ret==ESP_OK);
    ret = mpu_set_sample_rate(DEFAULT_MPU_HZ);
    assert(ret==ESP_OK);
    ret = dmp_load_motion_driver_firmware();
    assert(ret==ESP_OK);
    ret = dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation));
    assert(ret==ESP_OK);
    ret = dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |
        DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |
        DMP_FEATURE_GYRO_CAL);
    assert(ret==ESP_OK);
    ret = dmp_set_fifo_rate(DEFAULT_MPU_HZ);
    run_self_test();
    ret = mpu_set_dmp_state(1);
    assert(ret==ESP_OK);
}

esp_err_t mpu_dmp_get_data(float *pitch,float *roll,float *yaw)
{
	float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;
	unsigned long sensor_timestamp;
	short gyro[3], accel[3], sensors;
	unsigned char more;
	long quat[4]; 
	if(dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors,&more))return 1;	 
	if(sensors&INV_WXYZ_QUAT) 
	{
		q0 = quat[0] / q30;	
		q1 = quat[1] / q30;
		q2 = quat[2] / q30;
		q3 = quat[3] / q30; 
		*pitch = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3;	// pitch-90.0 <---> +90.0
		*roll  = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3;	// roll-180.0<---> +180.0
		*yaw   = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;	//yaw-180.0<---> +180.0
	}else return ESP_FAIL;
	return ESP_OK;
}

//mpu_get_accel_fsr();
esp_err_t MPU_Get_Accelerometer(float *data,unsigned char *fsr)
{
    uint8_t i;
    esp_err_t ret;
    short buffer[3];
    ret = mpu_get_accel_reg(buffer, NULL);
    for(i=0;i<3;i++) data[i] = (1.0)*(*fsr)*g*buffer[i]/32768;
    return ret;
}
//mpu_get_gyro_fsr(unsigned short *fsr)
esp_err_t MPU_Get_Gyroscope(float *data,unsigned short *fsr)
{
    uint8_t i;
    esp_err_t ret;
    short buffer[3];
    ret = mpu_get_gyro_reg(buffer, NULL);
    for(i=0;i<3;i++) data[i] = 1.0*(*fsr)*buffer[i]/32768;
    return ret;
}

esp_err_t MPU_Get_Temperature(float *data)
{
    esp_err_t ret;
    long buffer;
    ret = mpu_get_temperature(&buffer, NULL);
    *data = (buffer*1.0)/(1<<16);
    return ret;
}