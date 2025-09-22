# Read AHT21 Using APIs i2c primitives

## Exercise i2c and console
Starting point to understand zephyr i2c is zephyr shell.  
- This [golioth] article explains three shells, we use the Normal and i2c ones.
- use blinky as base, no optimization `-Og`, board overlay
- Node i2c@401a4000 is the lpi2c1 peripheral SCL (D15), SDA(14)
  - add a child aht21@38, no binding as we'll use primitives.
## make it work
- One measurment
- No float printf, see [formatted out](https://docs.zephyrproject.org/latest/services/formatted_output.html)
- DTS.overlay - need to learn more on nodes



## AHT21 states
The following description will evolve in a state machine, separate thread.
`AHT21_SLAVE_ADDR_7BIT 0x38`

1. Init 100ms
2. GetStatus, AHT21_GET_STATUS 0x71
  - read 0x38 0x71 
  - If 0x18, next. Otherwise, requires init (not implemented) 
  - InitializeAht,    	  //  AHT requires initialization (TBD)
4. PowerupEnds, 				  // 10ms delay at end of init sequence
  Transfer(measurement_trigger)
  write  0x38 0xAC 0x33 0x00

5. MeasurementTriggered, // Wait before sending a meassure data request (80ms)
  Transfer(measurement_request)
    write 0x38 0xAC 0x33 0x00
  
6. MeasureDataRequested, // wait for the i2c data to complete, 
  read 0x38 7 bytes
  if (buffer[0]& 0x80) == 0 )
    format received data


RetryDataRequest,     // sensor was busy, retry
MeasuringPeriod,  		// Waiting to comply measurement period
---
[golioth]:https://blog.golioth.io/how-to-use-zephyr-shell-for-interactive-prototyping-with-i2c-sensors/