# Read AHT21 Using APIs i2c primitives

## Exercise i2c and console
Starting point to understand zephyr i2c is zephyr shell.  
- This [golioth] article explains three shells, we use the Normal and i2c ones.
- use blinky as base, no optimization `-Og`, board overlay
- Node i2c@401a4000 is the lpi2c1 peripheral SCL (D15), SDA(14)
  - add a child aht21@38, no binding as we'll use primitives.

## make it work
- Sinlge pass-one measurment

## Use AhtStateMachine
- Single thread (main)
- Repetitive measurement

Define 3 states: `INIT,MEASUREMENT,SERVICE` from datasheet, Sensor Reading.
7.4 Sensor Reading Process   
1. After power-on, wait for ≥100ms. Before reading the temperature and humidity value, get a byte of status word by sending 0x71. If the status word and 0x18 are not equal to 0x18, initialize the 0x1B, 0x1C, 0x1E registers, details Please refer to our official website routine for the initialization process; if they are equal, proceed to the next step.

2. Wait 10ms to send the 0xAC command (trigger measurement). 
This command parameter has two bytes, the first byte is 0x33, and the second byte is 0x00.

3. Wait 80ms for the measurement to be completed, if the read status word Bit[7] is 0, it means the measurement is completed, and then six bytes can be read continuously; otherwise, continue to wait.

## Floating point printf
[formatted out](https://docs.zephyrproject.org/latest/services/formatted_output.html) also Kconfig Options in documentation.
- `west build -t menuconfig` to config libray/floating point - save minimimal config.
- `CONFIG_REQUIRES_FLOAT_PRINTF=y`
> Not using this CONFIG_SERIAL=y, CONFIG_HW_STACK_PROTECTION=y, CONFIG_ARM_MPU=y
- Picolibc Size flash 36296->41208 

## Minimum Error Handler
- SERVICE state provides a minimum handler.
- aht_t:str member provides status/info about the service


### Current Issues
- DTS.overlay - need to learn more on nodes
- Single thread doesn't take k_malloc(STR_LEN); try later on multi-threads

---
[golioth]:https://blog.golioth.io/how-to-use-zephyr-shell-for-interactive-prototyping-with-i2c-sensors/