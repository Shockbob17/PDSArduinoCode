#include <ezButton.h>
#include <AccelStepper.h>
#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
const int dirPin =9; //right z stepper 
const int stepPin = 10; 
const int dirPin2 = 7; //left z stepper
const int stepPin2 = 8;
const int dirPinx = 4; //x stepper 
const int stepPinx = 5;
ezButton button1(6); //Limit switch for z 
ezButton button2(11); //Limit switch for x
const int buttonPin = 2;  // reset button
const int scanPin = 3;  //scan button 
const int sweepPin = 12; // sweep x
int buttonState = 0;  // variable for reading the pushbutton status
int scanState = 0 ; //State to go upwards
int sweepState = 0; //State to start x sweep
int zheight = 0; //measurement to make sure we don't run out of the z axis

#define motorInterfaceType 1
AccelStepper myStepper(motorInterfaceType, stepPinx, dirPinx);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial2.begin(9600);

  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(stepPin2, OUTPUT);
  pinMode(dirPin2, OUTPUT);
  pinMode(dirPinx, OUTPUT);
  pinMode(stepPinx, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(scanPin, INPUT);
  pinMode(sweepPin,INPUT);
  myStepper.setSpeed(2000);
  myStepper.setMaxSpeed(1500);
  myStepper.setAcceleration(2000);
  Serial.println("Adafruit VL53L0X test.");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  lox.startRangeContinuous();
}

void loop() {
  // put your main code here, to run repeatedly
  if (Serial2.available()){
    Serial.println("Execute function");
    Serial.println("3");
  delay(1000);
    Serial.println("2");
    delay(1000); 
    Serial.println("1");
    delay(1000);
    String answer = omegasweeper();
    sendmessage(answer);
    Serial2.readStringUntil('\n');//flusH
  }
  else{
    buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH){
      Serial.println("Reset, setting reset to false");
      compiledreset();
    }
    sweepState =  digitalRead(sweepPin);
    if (sweepState == HIGH){
      Serial.println("Starting x test");
      omegasweeper();
      delay(2000);
    }
    scanState = digitalRead(scanPin);
    if (scanState == HIGH){
      Serial.println("start Scanning");
      int wee = scan();
      Serial.println(wee);
    }
    Serial.println("Waiting");
//    if (lox.isRangeComplete()) {
//      Serial.print("Distance in mm: ");
//      Serial.println(lox.readRange());
//    }
    delay(100);
  }
}  

void sendmessage(String input){
  Serial2.println(input);
  Serial2.readStringUntil('\n');//flush
}

int scan() {
  int btnState = 1;
  int count = 10;
  int counter = 0;
  int leeway = 2;
  int leecount = 0;
  directionup();
  for(int x = 0; x < 999999999999; x++){
    button1.loop();
    btnState = button1.getState();
    zheight += 1;
    // Serial.println(zheight);
    if (btnState == 0 || zheight > 3000){
      Serial.println("Breaking due to btn");
      break;
    }
    counter += 1;
    if (count == counter){
      counter = 0;
      if (lox.readRange() > 400){
        leecount += 1; 
        if (leecount == leeway){
          Serial.println("Breaking due to distance");
          return zheight;
        }
      }
    }
    spin();
  }
  Serial.println(zheight);
  Serial.println("stopped");
}

void resetbutton(){
  int btnState = 1;
  directiondown();
  while (btnState == 1){
    button1.loop(); // MUST call the loop() function first
    btnState = button1.getState();
    spin();
  // delayMicroseconds(10000);
  }
  Serial.println("Detected");
  delay(1000);
  //Once hit bottom, change direction
  directionup();
  for(int x = 0; x < 100; x++){
    spin();
  }
  zheight = 0;
}

void zeroingx(){
  int btnState = 1;
  while (btnState == 1){
    button2.loop();
    btnState = button2.getState();
    if (btnState == 0){
      delay(1000);
      myStepper.setCurrentPosition(0);
      myStepper.moveTo(-20);
      while (myStepper.distanceToGo() != 0){
        myStepper.run();
      }
      myStepper.setCurrentPosition(0);
      break;
    }
    else {
      myStepper.moveTo(9999999);
      myStepper.run();
    }
  }
}


String omegasweeper(){
  compiledreset();
  int value1 = round(scan()/160);
  resetbutton();
  moveleft();
  int value2 = round(scan()/160);
  resetbutton();
  moveleft();
  int value3 = round(scan()/160);
  resetbutton();
  moveleft();
  int value4 = round(scan()/160);
  String output = ("R1: "+ String(value1) + ",R2: " + String(value2) + ",R3: " + String(value3) + ",R4: " + String(value4) + ".");
  Serial.println(output);
  compiledreset();
  return output;
}

void compiledreset(){
  resetbutton();
  zeroingx();
}

void directionup(){
  digitalWrite(dirPin, LOW);
  digitalWrite(dirPin2, LOW);
}

void directiondown(){
  digitalWrite(dirPin, HIGH);
  digitalWrite(dirPin2, HIGH);
}

void moveleft(){
  myStepper.moveTo(-700);
  while (myStepper.distanceToGo() != 0){
    myStepper.run();
  }
  myStepper.setCurrentPosition(0);
}

void moveright(){
  myStepper.moveTo(700);
  while (myStepper.distanceToGo() != 0){
    myStepper.run();
  }
  myStepper.setCurrentPosition(0);
}

//function to call whenever spin motor
void spin(){ 
  digitalWrite(stepPin, HIGH);
  digitalWrite(stepPin2, HIGH);
  delayMicroseconds(1000);
  digitalWrite(stepPin, LOW);
  digitalWrite(stepPin2, LOW);
  delayMicroseconds(1000);
}
//
//
////This code will load firebase connection
////AFter done loading, constantly check the value
////WHen there is a change in the value, triggers the scan command
////When scanning, delay reading all values till the scan is complete
//
//void setup() {
//  // put your setup code here, to run once:
//  Serial.begin(9600);
//  Serial2.begin(9600);
//}
//
//void loop() {
//  // put your main code here, to run repeatedly:
////  Serial.println(Serial1.available());
//  if(Serial2.available())
//  {
//    String packet = Serial2.readStringUntil('.');
//    Serial.println(packet);
//    if(packet.length()>=3 && packet.indexOf('R')==0)  
//    {
//      
//      String value = packet.substring(2);
//      Serial.println("2");
//      delay(1000);
//      Serial.println("1");
//      delay(1000);
//      Serial2.println("Seny me.");
//    }
//    
////    while(!Serial1.availble());
//
//    
////    Serial1.readStringUntil('\n');//flush
//    
//  }
//delay(100);
//}
