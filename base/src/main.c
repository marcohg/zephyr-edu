/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h> 
#include <zephyr/drivers/i2c.h> 


#define I2C_NODE DT_NODELABEL(aht21)
#define STR_LEN 50
const uint16_t measurement_period = 250;

typedef enum {
	INIT,
	MEASUREMENT,
	SERVICE,
} aht_state_t;

typedef struct aht_tag {
	aht_state_t state;
	char str[STR_LEN];
	int eio;								// error i/0
	struct i2c_dt_spec i2c;
	int meas_period;
	double RH;
	double C;
	double F;
} aht_t;


int GetStatus(aht_t *a) {
	uint8_t get_status[] = {0x71};
	
	if( (a->eio = i2c_read_dt(&a->i2c, get_status, 1)) ) {
		sprintf(a->str,"GetStatus i2c error");
		return 1;
	}
	
	if((get_status[0] & 0x18) != 0x18 ) {
		sprintf(a->str,"NeedsInitialization");
		return 1;
	}
	return 0;
}

static void FormatData(aht_t *a, uint8_t *buffer) {
	static const uint32_t pow_2_20 = 0x100000;
	uint32_t s_rh = (uint32_t)buffer[1] << 12 | (uint32_t)buffer[2] << 4 | buffer[3] >> 4;
	a->RH = 100*((double)s_rh/pow_2_20);
	uint32_t s_t = (uint32_t)buffer[3] << 16 | (uint32_t)buffer[4] << 8 | (uint32_t)buffer[5];
	s_t &= 0x0FFFFF;
	a->C = ((double)s_t/pow_2_20)*200 -50;
	a->F = a->C *9/5 + 32;
}

int GetMeasurement(aht_t *a) {
	const uint8_t trigger_meas[] = { 0xAC, 0x33, 0x00};
	if((a->eio = i2c_write_dt(&a->i2c, trigger_meas, 3)) ) {
		sprintf(a->str,"ErrorI2cTriggerMeasurement");
		return 1;
	}
	k_msleep(80);
	a->meas_period = measurement_period - 80;
	const int max_retry = 10;
	bool new_meas = false;
	for(int retries =0; !new_meas || retries < max_retry; ++retries) {
		uint8_t data[7];
		if((a->eio = i2c_read_dt(&a->i2c,data,7))) {
			sprintf(a->str,"ReadDataError i2c");
			return 1;
		}
		else
			if((data[0]&0x80)==0) {
				FormatData(a, data);
				new_meas = true;
			}
			else {
				k_msleep(20);	// retry - busy
				a->meas_period -= 20;
				if(a->meas_period < 0)
					a->meas_period = 0;
			}
	}
	
	if(new_meas)
		return 0;
	else {
		sprintf(a->str, "NoData %d", max_retry);
		return 1;
	}
}

 void HandleError(aht_t *aht) {

 }

int AhtStateMachine(aht_t *a) {
	switch(a->state) {
		case INIT:
			if (!i2c_is_ready_dt(&a->i2c)) { 
				sprintf(a->str,"i2c bus not ready");
				a->state = MEASUREMENT;
				return 1; 
			}
			k_msleep(100);
			if(GetStatus(a) == 0) {
				a->state = MEASUREMENT;
				k_msleep(10);	
			}				
			else 
				a->state = SERVICE;
			break;

		case MEASUREMENT:
			if(GetMeasurement(a)==0)	
				k_msleep(a->meas_period);
			break;

		case SERVICE:
			HandleError(a);
			break;
	}
	return 0;
}



int main(void)
{
	aht_t aht21 = {
	.i2c = I2C_DT_SPEC_GET(I2C_NODE),
	.state = INIT,
	.eio = 0,
	};
	while(1) {
		AhtStateMachine(&aht21);
		printk("RH: %f C: %f F: %f\n",aht21.RH, aht21.C, aht21.F);
	}

	return 0;
}
