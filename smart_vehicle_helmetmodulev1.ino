#include<Wire.h>
#include <SoftwareSerial.h>       //Software Serial library
SoftwareSerial espSerial(2, 3);   //Pin 2 and 3 act as RX and TX. Connect them to TX and RX of ESP8266      
#define DEBUG true

String mySSID = "OnePlus 6T";       // WiFi SSID
String myPWD = "123123123"; // WiFi Password
String myAPI = "CMSB1GTLFGK4F2SH";   // API Key
String myHOST = "api.thingspeak.com";
String myPORT = "80";
String myFIELD = "field1"; 
String myFIELD1 ="field2";
String myFIELD2 ="field3";
String myFIELD3 ="field4";
int Signal;
const int MPU6050_addr=0x68;
int16_t AccX,AccY,AccZ;

void setup()
{ Wire.begin();
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
    
    
    String sendData = "GET /update?api_key="+ myAPI +"&"+ myFIELD +"="+String(Ax)+"&"+ myFIELD1 +"="+String(Ay)+"&"+ myFIELD2 +"="+String(Az)+"&"+ myFIELD3 +"="+String(BPM); 
    espData("AT+CIPMUX=1", 1000, DEBUG);       //Allow multiple connections
    espData("AT+CIPSTART=0,\"TCP\",\""+ myHOST +"\","+ myPORT, 1000, DEBUG);
    espData("AT+CIPSEND=0," +String(sendData.length()+4),1000,DEBUG);  
    espSerial.find(">"); 
    espSerial.println(sendData);
    Serial.print("Value to be sent: ");
    Serial.println(Signal);
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
  
