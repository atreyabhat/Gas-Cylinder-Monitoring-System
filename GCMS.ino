#include "HX711.h"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <SPI.h>

#define BLYNK_PRINT Serial

char ssid[] = "Agbspot";       // your network SSID (name)
char pass[] = "agbhotspot";  // your network key


HX711 cell(D2, D3); // The two pins connected to the Load Cell and Node MCU 

const int buzzer = 12;
char auth[] = "54dc2d949ce8406e9c922927d756b73b";


void setup() {
  Serial.begin(19200);
   Blynk.begin(auth, ssid, pass);

pinMode(buzzer,OUTPUT);
 

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  


  
}
long value = 0;
long val = 0;
long tarevalue=100;
float count = 0;
int flag1 = 0;
int flag2 = 0;

void loop() {
    Blynk.run();


  
  count = count + 1;
  
  val = cell.read(); // most recent reading
  value = (((val-8395800)/22872.0)*70)-tarevalue; 
  Serial.println(value);
   Blynk.virtualWrite(V5,value);
   Blynk.virtualWrite(V6,(value/4));

//CHECKING FOR OVERLOAD
 if( value > 10000)
  {
    Serial.println("OVERLOAD");
    delay(5000); 
  }
  
   
    
//CHECKING FOR THRESHOLD VALUE
  
   if((value < 100)&&(flag1==0))
{

   Blynk.notify("Cylinder Net Weight is below 100gm (25% is remaining), Book a Replacement Soon");
     flag1 = 1; 
  for(int i=0;i<5;i++)
  {  
  digitalWrite(buzzer, HIGH);
  delay(200);
  digitalWrite(buzzer, LOW);
  delay(200);  
  }
    // Blynk.email("atreya29@gmail.com", "Subject: Book Your Cylinder", " Cylinder Net Weight is below 50gm (10% is remaining), Book a Replacement Immediately");
    
}

//CHECKING FOR EMPTY CYLINDER
 if((value<25)&&(flag2==0))
 {
  Serial.println("The Cyinder is Empty");
  Blynk.notify("Your Cylinder is Empty");

  digitalWrite(buzzer, HIGH);
  delay(2000);
  digitalWrite(buzzer, LOW);
  delay(200);  
  flag2=1;
 }


//CHECKING IF WEIGHT INCREASES TO DEFAULT VALUE AFTER CYLINDER REPLACEMENT, AND HENCE RESET THE DEVICE

 if((flag2==1)&&(value>20))
{
  delay(2000);
  if(value>200)
  {
      Serial.println("The Device has been Reset");
      Blynk.notify("The Device has been Reset,Replacement is Done");
      for(int i=0;i<4;i++)
     {  
       digitalWrite(buzzer, HIGH);
       delay(100);
       digitalWrite(buzzer, LOW);
       delay(50);  
  }
     
      flag1=0;
      flag2=0;
}
}
}
