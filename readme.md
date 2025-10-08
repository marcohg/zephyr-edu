# Zephyr Samples and Demos
Zephyr project has very good documentation.  
Reading the documentation, I found difficult where to pause and to apply.  
These some thought in my learning:
- Explore in more details samples amd demos
- Modify examples - use your board, try other boards.
- First focus in learning the build process, slow down.
  - Use command line west to build, flash and debug.
  - VS code just as an editor (while learning).
  - AI in provides very good results when search for help. 

## Environment
- Examples follow free standing applications. 
- The `build` directory is created one level up (cmake) the example folder.
- Modify the bashrc to ease typing.
```
# Learning zephyr - assume Getting Started workspace at ~/zephyrproject 
echo  start virtual environment: setup_enviroment  
setup_environment() {
  source  ~/zephyrproject/.venv/bin/activate
  source  ~/zephyrproject/zephyr/zephyr-env.sh  # will run $HOME/.zephyrrc
  echo "BOARD is: $BOARD"
}
alias build='west build --build-dir ../build'
alias flash='west flash --build-dir ../build --runner jlink'
alias debug='west debug --build-dir ../build --runner jlink'
```
- $BOARD is `mimxrt1010_evk` (defined in $HOME/.zephyrrc)

## Blinky
We start with this project, moving out from workspace for free-standing 
- Doc [Blinky](https://docs.zephyrproject.org/latest/samples/basic/blinky/README.html#blinky)
- See notes for commits at blinly/ 
  - Blinky
  - Add second output (led)

## Threads
- Test threads creation

## base
- My first application: use i2c 
- Start with API primitives (sensor)
- Use the i2c shell to scan-debug the hardware (added some targets to play)
- Use AHT21 to measure relative humididty and temperature.
- See the base/readme.md notes
- make it a thread, retrospective, skim device tree docs.

## GDB commands
- jlink: JLinkGDBServer
- info breakpoints - list all break
- monitor reset - resets targetcd
- disable breakpoint, enable breakpoint
- set variable foo = 123

## Explore Second serial
- Assign a separate uart to Console and Shell.
- NXP Pin Tool was used find second uart routed to Arduino connector
[58] GPIO_AD_02   LPUART4_TX    A5
[59] GPIO_AD_01   LPUART4_RX    A4

### lpuart4 
- Set property `status = okay` to enable in the project overlay.
- Keep console in lpuart1 and use lpuart4 for shell.
- Enable float in printf:
  - `west build -t menuconfig` ->  `Requires floating point support in printf` 
  - Use `Save minimum config` to get the kconfig changes to set in prj.config
- Need to explore more pin-ctrl, like pinmux_lpuart4/group0/{
	- pinmux = <&iomuxc_gpio_ad_01_lpuart4_rxd>,<&iomuxc_gpio_ad_02_lpuart4_txd>;
  - A4  SDA ADC12_1 LPUART4:RXD
  - A5  SCL ADC12_2 LPUART4:TXD

## mimxrt1010_evk 5V Supply selector J1.
- Jumper 1-2 selects OpenSda (J41) convenbient as only one USB cable needed, but can not 
  supply enough current. The board powers up in `MAINTENANCE` mode.
- We jumper 3-9 the get power from USB OTG, this requires an extra cable at J9

## Modbus rtu_server
- use [RTU server](https://docs.zephyrproject.org/latest/samples/subsys/modbus/rtu_server/README.html#modbus-rtu-server)
- Tested on FRDM-K64, arduino 15 (D9), arduino_serial (uart3)

## Next test
- Generate dtsi information *.mex to *.dtsi using pin tool
- Generator location 
- Simple hello_world  and add second uart, toggle pin.
/home/marco/zephyrproject/modules/hal/nxp/mcux/scripts/pinctrl/gen_board_pinctrl.py


https://community.nxp.com/t5/Zephyr-Project-Knowledge-Base/Zephyr-custom-boards-and-applications/ta-p/2008567
