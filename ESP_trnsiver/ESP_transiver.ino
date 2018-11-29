/*
 Name:		ESP_trnsiver.ino
 Created:	11/26/2018 8:39:36 PM
 Author:	Yarkoba4i
*/

#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

int x = 0;

void setup() {

	
	Serial.begin(115200);
	Serial.println("*** Slave - START ***");
}

void loop() {

	
	delay(50);
}


void receiveEvent(int howMany)
{
	while (1 < Wire.available()) // loop through all but the last
	{
		char c = Wire.read();      // receive byte as a character
		Serial.print(c);           // print the character
	}
	int x = Wire.read();         // receive byte as an integer
	Serial.println(x);           // print the integer
}