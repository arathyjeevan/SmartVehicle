#include <SoftwareSerial.h>
#include <ArduinoJson.h>
SoftwareSerial s(2,3);

#include<Wire.h>
const int MPU6050_addr=0x68;
int16_t AccX,AccY,AccZ,Temp,GyroX,GyroY,GyroZ;
void setup(){
  s.begin(115200);
  Wire.begin();
  Wire.beginTransmission(MPU6050_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(9600);
}

StaticJsonBuffer<1000> jsonBuffer;
JsonObject& root = jsonBuffer.createObject();

void loop(){
  int AccelScaleFactor = 16384;
  double Ax,Ay,Az;
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
  
  Serial.print("AccX = "); Serial.print(Ax);
  Serial.print(" || AccY = "); Serial.print(Ay);
  Serial.print(" || AccZ = "); Serial.print(Az);
   Serial.println(" ");

   if (isnan(Ax) || isnan(Ay) || isnan(Az)) {
    return;
  }
  // If the DHT-11 is not connected to correct pin or if it doesnot
//work no data will be sent
  root["Ax"] = Ax;
  root["Ay"] = Ay;
  root["Az"] = Az;
 
if(s.available()>0)
{
 root.printTo(s);
}
  
  delay(100);
}
