# P9 - SCADA - SPI
SCADA (Supervisory Control and Data Acquisition) system with 4 PIC18F4550. All microcontrollers communicate via SPI.

## GLCD slave
Displays the position of the obstacle in real time on a GLCD. It only receives the distance from the master.

## Motor slave
Generates the PWM signal for the motors to move. It receives the PWM duty cycle, or if static, it only receives start and stop orders.

## Sensor slave
This slave is the only one that send data back to the master. It sends the 16 - bit timer count to the slave in two parts, when prompted by the master.

## Master
Controls all slaves, and has a bluetooth module (HC-06) to receive instructions. The commands are:
- Start
- Stop
- Distance 1 (7cm)
- Distance 2 (15cm)
- Distance 3 (19cm)
- 
