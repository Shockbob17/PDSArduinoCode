# PDSArduinoCode
This is a repository with the code that was developed for the arduino mega, MKR 1010 and telegram bot for SUTD DAI TERM 4 PDS project.

Below is the circuit diagram of the circuit that was used in the project.
!["REEEE"](https://github.com/Shockbob17/PDSArduinoCode/blob/main/Circuit_Diagram.png?raw=true)

## Mega_serial.ino
This file is found inside the mega_serial folder and is the code that is run on the arduino mega.
It is responsible for driving the motors and collecting the sensor readings in order to determine the number of trays per stack.

The code requires the following libraries:
ezButton: A library that is used to collect the limit switch values.
AccelStepper: A library that is used to control the stepper motors.
Adafruit_VL53L0X: A library for the time-of-flight sensor used. 

It has several functions:
Moveleft: A function to move it towards the next stack of trays. \n
Resetbutton: A function used to return the bar to a height of 0.
Scan: A function used to move the bar upwards till the time of flight sensor detects there is no longer a tray in front of it. It will then return the z-height reached, which can be used to find the number of trays in a stack. 
Compiledreset: A function used to return the sensor to its origin position at the bottom right.	
