/*
  SIT210 - Task 2.1P
  Reading Temperature

  Mubasher Fareed
  Student ID: 215426559
*/

#include "WiFiNINA.h"
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int status = WL_IDLE_STATUS;
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

// Initialize our values
int number1 = 0;
int number2 = random(0,100);
int number3 = random(0,100);
int number4 = random(0,100);
String myStatus = "";

#define sensorPin A7 // Temperature sensor connected to A7

int analogReading;
float Vout;
float tempCelcius;
float tempfahrenheit;

void setup() {
  Serial.begin(9600);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }
  
  ThingSpeak.begin(client);  // Initialize ThingSpeak   
}

void loop() {

  // Connect to WiFi
  if(WiFi.status() != WL_CONNECTED){

    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);

    status = WiFi.begin(ssid, pass);

    Serial.println("\nConnected.");

    delay(5000); 
  }
  
  // Record temperature
  analogReading = analogRead(sensorPin);

  Vout = analogReading * (3.3/1024);

  tempCelcius = (Vout - 0.5) * 100;
  tempfahrenheit = (tempCelcius * 9.0/5.0) + 32.0;

  // Serial.println(tempCelcius);

  // set the fields with the values
  ThingSpeak.setField(1, tempCelcius);
  ThingSpeak.setField(2, tempfahrenheit);

  // set the status
  ThingSpeak.setStatus("Here's the temperature");

  // write to the ThingSpeak channel 
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

  delay(15000);  
}
