/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h> 
#include <zephyr/drivers/i2c.h> 

#define I2C_NODE DT_NODELABEL(aht21)
static const struct i2c_dt_spec aht21_i2c = I2C_DT_SPEC_GET(I2C_NODE);
#define POW_2_20 (0x100000)	// 2^20

int main(void)
{
	printk("Sleep 1 1sec, AHT21 is connected\n");
	if (!i2c_is_ready_dt(&aht21_i2c)) { 
		printk("the i2c bus for the ADS1115 is not ready\n"); 
    return -1; 
  }
	uint8_t buffer[10];
	// 1. 100ms Power-on and GET_STATUS0
	k_msleep(100);	//
	uint8_t get_status[] = {0x71};
	if( i2c_read_dt(&aht21_i2c, get_status, 1) ) 
		return -1;
	if((get_status[0] & 0x18) != 0x18 ) {
		printk("Initialization needed\n");
		return -1; 
	}
		
	// 2. wait 10ms and Trigger measurement
	k_msleep(10);	//
	const uint8_t trigger_meas[] = { 0xAC, 0x33, 0x00};
	if( i2c_write_dt(&aht21_i2c, trigger_meas, 3) ) 
		return -1;
	
	// 4. wait 80ms Meas completed, read 7 bytes, 
	bool busy = true;
	int32_t delay = 80;
	while(busy) {
		k_msleep(delay);	//
		if( i2c_read_dt(&aht21_i2c,buffer,7) )
			return -1;
		if((buffer[0]&0x80)==0) {
			busy = false;
			// format data
			uint32_t s_rh = (uint32_t)buffer[1] << 12 | (uint32_t)buffer[2] << 4 | buffer[3] >> 4;
			double relative_humidity = 100*((float)s_rh/POW_2_20);
  		uint32_t s_t = (uint32_t)buffer[3] << 16 | (uint32_t)buffer[4] << 8 | (uint32_t)buffer[5];
			s_t &= 0x0FFFFF;
			double temperature = ((float)s_t/POW_2_20)*200 -50;
			int32_t RH = relative_humidity * 10;
			int32_t T = temperature *10;
			int32_t F = (temperature *9/5 + 32)*10;
			printk("RH: %d C: %d F: %d x10\n",RH, T, F);
			k_msleep(5000);		// period
		}
		else {
			delay = 10;	// retry
		}
	}
 
	// i2c_write_dt(aht21, sendConfigBuff,3 );
	return 0;
}
