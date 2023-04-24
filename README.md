# PDSArduinoCode
This is a repository with the code that was developed for the arduino mega, MKR 1010 and telegram bot for SUTD DAI TERM 4 PDS project.

Below is the circuit diagram of the circuit that was used in the project.
!["REEEE"](https://github.com/Shockbob17/PDSArduinoCode/blob/main/Circuit_Diagram.png?raw=true)

## Mega_serial.ino
This file is found inside the mega_serial folder and is the code that is run on the arduino mega.
It is responsible for driving the motors and collecting the sensor readings in order to determine the number of trays per stack.

The code requires the following libraries: <br>
ezButton: A library that is used to collect the limit switch values. <br>
AccelStepper: A library that is used to control the stepper motors. <br>
Adafruit_VL53L0X: A library for the time-of-flight sensor used. <br>

It has several functions:<br>
Moveleft: A function to move it towards the next stack of trays. <br>
Resetbutton: A function used to return the bar to a height of 0.<br>
Scan: A function used to move the bar upwards till the time of flight sensor detects there is no longer a tray in front of it. It will then return the z-height reached, which can be used to find the number of trays in a stack. <br>
Compiledreset: A function used to return the sensor to its origin position at the bottom right.	

There functions would then be called during the main function, omegasweeper: <br>
1. It will call Compiledreset to ensure it is at the origin point. <br>
2. It will run Scan for each stack of tray and then bring the scanner back to a z-height of 0 with the Resetbutton command. <br>
3. It will use Moveleft to move to the next stack of trays. <br>
4. It will then divide the z-height returned with a constant for the height of an individual tray being scanned to find how many trays are in each stack. <br>
5. Repeat steps 2-4 for each stack of trays. <br>
6. The function will then format the number of trays in each stack into a proper string and send it to the MKR 1010 for it to upload to the firebase. <br>
