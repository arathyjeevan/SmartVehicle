#include<Wire.h>
#include <SoftwareSerial.h>       //Software Serial library
SoftwareSerial espSerial(2, 3);   //Pin 2 and 3 act as RX and TX. Connect them to TX and RX of ESP8266      
#define DEBUG true

String mySSID = "OnePlus 6T";       // WiFi SSID
String myPWD = "123123123"; // WiFi Password
String myAPI = "PNE5H510YX30AD14";   // API Key
String myHOST = "api.thingspeak.com";
String myPORT = "80";
String myFIELD = "field1"; 
String myFIELD1 ="field2";
String myFIELD2 ="field3";
String myFIELD3 ="field4";
String myFIELD4 ="field5";
String myFIELD5 ="field6";
String myFIELD6 ="field7";

int Signal;
const int MPU6050_addr=0x68;
int16_t AccX,AccY,AccZ;
const int knock1 = 15; // the piezo is connected to DIGITAL PIN 7
const int knock2 = 16;
const int knock3 = 17;
int f1 = 0;      // variable to store the value read from the sensor pin
int f2 = 0;
int f3 = 0;

void setup()
{ pinMode(knock1, INPUT); // declare the ledPin as as OUTPUT
  pinMode(knock2, INPUT);
  pinMode(knock3, INPUT);
  Wire.begin();
  Wire.beginTransmission(MPU6050_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(9600);
  espSerial.begin(115200);
  
  espData("AT+RST", 1000, DEBUG);                      //Reset the ESP8266 module
  espData("AT+CWMODE=1", 1000, DEBUG);                 //Set the ESP mode as station mode
  espData("AT+CWJAP=\""+ mySSID +"\",\""+ myPWD +"\"", 1000, DEBUG);   //Connect to WiFi network
  /*while(!esp.find("OK")) 
  {          
  }*/
  delay(1000);
  
}
      //Wait for connection

  void loop()
  {
  int AccelScaleFactor = 16384;
  double Ax,Ay,Az;
  int BPM;
  Signal =analogRead(A0);
  Wire.beginTransmission(MPU6050_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_addr,14,true);
  AccX=Wire.read()<<8|Wire.read();
  AccY=Wire.read()<<8|Wire.read();
  AccZ=Wire.read()<<8|Wire.read();
  Ax = (double)AccX/AccelScaleFactor;
  Ay = (double)AccY/AccelScaleFactor;
  Az = (double)AccZ/AccelScaleFactor;
  BPM = Signal;
  f1 = digitalRead(knock1);
  f2 = digitalRead(knock2);
  f3 = digitalRead(knock3);
    
    String sendData = "GET /update?api_key="+ myAPI +"&"+ myFIELD +"="+String(Ax)+"&"+ myFIELD1 +"="+String(Ay)+"&"+ myFIELD2 +"="+String(Az)+"&"+ myFIELD3 +"="+String(BPM)+"&"+ myFIELD4 +"="+String(f1)+"&"+ myFIELD5 +"="+String(f2)+"&"+ myFIELD6 +"="+String(f3); 
    espData("AT+CIPMUX=1", 1000, DEBUG);       //Allow multiple connections
    espData("AT+CIPSTART=0,\"TCP\",\""+ myHOST +"\","+ myPORT, 1000, DEBUG);
    espData("AT+CIPSEND=0," +String(sendData.length()+4),1000,DEBUG);  
    espSerial.find(">"); 
    espSerial.println(sendData);
    Serial.print("Value to be sent: ");
    Serial.println(Signal); Serial.print("- ");
     Serial.println(f1);Serial.print("- ");
      Serial.println(f2);Serial.print("- ");
       Serial.println(f3);
    espData("AT+CIPCLOSE=0",1000,DEBUG);
    delay(1000);
  }

  String espData(String command, const int timeout, boolean debug)
{
  Serial.print("AT Command ==> ");
  Serial.print(command);
  Serial.println("     ");
  
  String response = "";
  espSerial.println(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (espSerial.available())
    {
      char c = espSerial.read();
      response += c;
    }
  }
  if (debug)
  {
    //Serial.print(response);
  }
  return response;
}
  
