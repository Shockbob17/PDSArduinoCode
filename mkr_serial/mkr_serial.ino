#include <WiFiNINA.h>
#include "arduino_secrets.h"            //contains wifi credentials
#include "Firebase_Arduino_WiFiNINA.h"  //Firebase Arduino based on WiFiNINA-install from manage libraries
#include <Adafruit_Sensor.h>
#include <HCSR04.h>
#include <TimeLib.h>

#define FIREBASE_HOST "pdsbotbot-default-rtdb.firebaseio.com"  //Firebase Realtime database URL
#define FIREBASE_AUTH "9YjIQiCMHeK6ZlgtueFtr2snTHc2KYXxX8DotuKH" //from Firebase Database secrets

char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;     // the Wifi radio's status

int unavailable = 6;
int availabled = 7;

FirebaseData firebaseData;
String reading = "/com";
String writing = "/tray";

uint32_t delayMS;
int timestamp;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(unavailable, OUTPUT);
  pinMode(availabled, OUTPUT);
  digitalWrite(unavailable, HIGH);

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to network: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  // you're connected now, so print out the data:
  Serial.println("You're connected to the network!");

  Serial.println("----------------------------------------");
  Serial.println("----------------------------------------");

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH, SECRET_SSID, SECRET_PASS);

  Firebase.reconnectWiFi(true); //Let's say that if the connection is down, try to reconnect automatically
  Serial.println("Start sensing");
  digitalWrite(unavailable, LOW);
  digitalWrite(availabled, HIGH);
}

void loop() {
  int state = readcommandfirebase();
  if (state == 1) {
    digitalWrite(unavailable, HIGH);
    digitalWrite(availabled, LOW);
    Serial.println("Command Received, sending");
    Serial1.println("Begin the function!");
    Serial.println("Awaiting Response");
    while (!Serial1.available());
    String answer = (Serial1.readStringUntil('.'));
    //    Serial.println(answer);
    char *token; // this will hold each item after being split
    char charArray[answer.length() + 1];
    answer.toCharArray(charArray, sizeof(charArray));

    // split the input string by commas and iterate through each item
    token = strtok(charArray, ",");
    while (token != NULL) {
      // do something with the current item
      if (token[2] == ':') {
        String suba = String(token).substring(2 + 1);
        Serial.println(suba);
        String subpath = '/' + String(token[1]);
        writestringtofirebase(suba, writing, subpath);
      }
      // get the next item
      token = strtok(NULL, ",");
    }
    //    Do something with the answer
    writeinttofirebase(0, reading, "/active");
    Serial1.readStringUntil('\n');//flush
    delay(10000);
  }
  digitalWrite(unavailable, LOW);
  digitalWrite(availabled, HIGH);
  delay(1000);
  
  // put your main code here, to run repeatedly
}


int readcommandfirebase() {
  Serial.println("Read a value");

  if (Firebase.getInt(firebaseData, reading + "/active"))
  {
    Serial.print("Value: ");
    Serial.println(firebaseData.intData());
    return (firebaseData.intData());
  }
  else
  {
    Serial.println("ERROR: " + firebaseData.errorReason());
    Serial.println();
  }

  Serial.println("------------------------------------------------------------------------------------");
}

void writeinttofirebase(int t, String path, String subpath) {
  if (Firebase.setInt(firebaseData, path + subpath, t))
  {
    Serial.println("Value reset to " + String(t));
  }
  else
  {
    Serial.println("ERROR : " + firebaseData.errorReason());
    Serial.println();
  }
}

void writestringtofirebase(String t, String path, String subpath) {
  if (Firebase.setString(firebaseData, path + subpath, t))
  {
    Serial.println("Value reset to " + String(t));
  }
  else
  {
    Serial.println("ERROR : " + firebaseData.errorReason());
    Serial.println();
  }
}


//This code will load firebase connection
//AFter done loading, constantly check the value
//WHen there is a change in the value, triggers the scan command
//When scanning, delay reading all values till the scan is complete

//void setup() {
//  // put your setup code here, to run once:
//  Serial.begin(9600);
//  Serial1.begin(9600);
//}
//
//void loop() {
//  // put your main code here, to run repeatedly:
////  Serial.println(Serial1.available());
//  if(Serial1.available())
//  {
//    String packet = Serial1.readStringUntil('.');
//    Serial.println(packet);
//    if(packet.length()>=3 && packet.indexOf('R')==0)
//    {
//
//      String value = packet.substring(2);
//      Serial.println("2");
//      delay(1000);
//      Serial.println("1");
//      delay(1000);
//      Serial1.println("Seny me.");
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

//bool state = 0;
//
//void setup() {
//  // put your setup code here, to run once:
//  Serial.begin(9600);
//  Serial1.begin(9600);
//}
//
//void loop() {
//  // put your main code here, to run repeatedly:
//
//  state = !state;
//
//  String packet = "R:" + String(state) +".";
//// Send the signal
//  Serial.println(packet);
//  Serial1.println(packet);
//  //  Wait till singal is sent back
//  if (Serial1.available()){
//    Serial.println(Serial1.readString());
//  }
////  Wait till singal is sent back
//  delay(5000);
//}
