/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)
#define D8_NODE DT_ALIAS(dio8)


/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec d8 = GPIO_DT_SPEC_GET(D8_NODE, gpios); 



int main(void)
{
	bool led_state = true;

	if (!gpio_is_ready_dt(&led) 
		|| !gpio_is_ready_dt(&d8)) {
		return 0;
	}
	
	if ( gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0 || // sets gpio as output 
		gpio_pin_configure_dt(&d8, GPIO_OUTPUT_ACTIVE) < 0) {
			return 0;
		}
		
	while (1) {
		if( gpio_pin_toggle_dt(&led) < 0 ||	
			gpio_pin_set_dt(&d8, led_state) < 0 ) {
				return 0;
			}
		printf("LED state: %s\n", led_state ? "ON" : "OFF");
		led_state = !led_state;
		
		k_msleep(SLEEP_TIME_MS);
	}

	return 0;
}
