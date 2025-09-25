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

