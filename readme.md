# Zephyr Samples and Demos
After reading most of documentation 
- explore in more details samples amd demos
- modify examples
## Environment
- This is a free standing application 
- The `setup_environment` sources activate venv and env variables scripts 
- The $BOARD is `mimxrt1010_evk`
- Change to desired folder use west/cmake to develop
`west build --build-dir ../build` # builds  
`west flash -d ../build --runner jlink` or  
`west debug --build-dir ../build --runner jlink` # to flash or debug

## Blinky
We start with this project, moving out from workspace for free-standing 
- Doc [Blinky](https://docs.zephyrproject.org/latest/samples/basic/blinky/README.html#blinky)
- See notes for commits at blinly/ 
  - Blinky
  - Add second output (led)
