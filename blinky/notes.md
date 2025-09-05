# Blinky Notes
- Get the devicetree specifications in the `struct gpio_dt_spec`

## Explore ../build/zephyr/include/generated/zephyr/devicetree_generated.h 
1. Get the led0-> Node identifier
  is `led-1` -> full path `DT_N_S_leds_S_led_1` 

2. Get the pin specification in gpio_dt_spec
add a debug point and examine LED, GPIO_11
```
(gdb) p led
$1 = {port = 0x60006388 <__device_dts_ord_61>, pin = 11 '\v', dt_flags = 1}

(gdb) p *led.port
$2 = {name = 0x60006b5c "gpio@401b8000", config = 0x60006cb8 <mcux_igpio_0_config>, api = 0x60006628 <mcux_igpio_driver_api>, 
  state = 0x20200026 <__devstate_dts_ord_61>, data = 0x20200218 <mcux_igpio_0_data>, ops = {
    init = 0x60005a25 <mcux_igpio_0_init>, deinit = 0x0}, flags = 0 '\000'}
```
- Optimize gcc -Og, 

GDB when TUI, `set tui mouse-events on` to allow mouse to work properly.

3. Step in `gpio_pin_toggle_dt(&led)` at main.c:38 pin access:
```
(gdb) l
158	 * @param mask GPIO pin number macro
159	 */
160	static inline void GPIO_PortToggle(GPIO_Type *base, uint32_t mask)  
161	{
162	#if (defined(FSL_FEATURE_IGPIO_HAS_DR_TOGGLE) && (FSL_FEATURE_IGPIO_HAS_DR_TOGGLE == 1))
163	    base->DR_TOGGLE = mask;   <<--- HERE
164	#else
165	    base->DR ^= mask;
166	#endif /* FSL_FEATURE_IGPIO_HAS_DR_TOGGLE */
167	}

(gdb) bt
#0  GPIO_PortToggle (mask=2048, base=0x401b8000) at /home/marco/zephyrproject/modules/hal/nxp/mcux/mcux-sdk-ng/drivers/igpio/fsl_gpio.h:163
#1  mcux_igpio_port_toggle_bits (dev=<optimized out>, mask=2048) at /home/marco/zephyrproject/zephyr/drivers/gpio/gpio_mcux_igpio.c:270
#2  0x600053da in z_impl_gpio_port_toggle_bits (pins=<optimized out>, port=<optimized out>) at /home/marco/zephyrproject/zephyr/include/zephyr/drivers/gpio.h:1485
#3  gpio_port_toggle_bits (pins=<optimized out>, port=<optimized out>) at /home/marco/Documents/fw/zephyr/build/zephyr/include/generated/zephyr/syscalls/gpio.h:234
#4  gpio_pin_toggle (pin=<optimized out>, port=<optimized out>) at  /home/marco/zephyrproject/zephyr/include/zephyr/drivers/gpio.h:1722
#5  gpio_pin_toggle_dt (spec=spec@entry=0x60006694 <led>) at        /home/marco/zephyrproject/zephyr/include/zephyr/drivers/gpio.h:1737
#6  0x6000248a in main () at /home/marco/Documents/fw/zephyr/blinky/src/main.c:38
```
## EVK Schematic
- User Led is  GPIO_11 ACTIVE_HIGH  - GPIO1 401B_8000
- Arduino D8 - extra out, TP34, GPIO_SD_02  
- User button is GPIO_SD_06

GPIO1 base address: 401B_8000h.
GPIO2 base address: 4200_0000h.
GPIO5 base address: 400C_0000h.

Instance  Port          Pad         Mode  12x12pin
GPIO2     GPIO2_IO02    GPIO_SD_02  ALT5  74
GPIOMUX   GPIOMUX_IO11  GPIO_11     ALT5  1   (default)

## Adding Extra GPIO output
- Easy is add a child node (arduino) D8 to the leds.
- Note that we are using the 'same' name, but supposed to used &leds?
- use mcuxpresso to add it and sort the doc  
