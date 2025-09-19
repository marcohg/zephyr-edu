# Exercise i2c and console
Used [golioth shell] reference to debug i2c
- use blinky as base, no optimization, board overlay
- Node i2c@401a4000 is the lpi2c1 peripheral SCL (D15), SDA(14)

## AHT21
AHT21_SLAVE_ADDR_7BIT 0x38
1.  AHT21_GET_STATUS 0x71
read 0x38 0x71 
  0x18 - next
2. wait 10ms

States
1. Init     100ms
2. GetStatus, AHT21_GET_STATUS 0x71
read 0x38 0x71 
  0x18 - next, requires init otherwise
3. InitializeAht,    	  //  AHT requires initialization (TBD)
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
[golioth shell]:https://blog.golioth.io/how-to-use-zephyr-shell-for-interactive-prototyping-with-i2c-sensors/)  