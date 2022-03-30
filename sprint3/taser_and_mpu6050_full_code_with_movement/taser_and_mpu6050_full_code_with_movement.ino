#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include"WiFi.h"
#include "AsyncUDP.h"

Adafruit_MPU6050 mpu;
AsyncUDP udp;
AsyncUDP udp1;
const char* ssid = "TP-Link_5EFE";
const char* pass = "26545653";
String motionx;
String motiony;
String motionz;
String motion;

void setup(void)
{
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) 
  {
    Serial.println("Failed to find MPU6050 chip");
    
    while (1)
    {
      delay(10);
    }
  }
  
  Serial.println("MPU6050 Found!");
  mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  mpu.setMotionDetectionThreshold(1);
  mpu.setMotionDetectionDuration(20);
  mpu.setInterruptPinLatch(true);  // Keep it latched.  Will turn off when reinitialized.
  mpu.setInterruptPinPolarity(true);
  mpu.setMotionInterrupt(true);
  Serial.println("");
  delay(100);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  
  getAcceleroMotion();
  //udp.broadcastTo(motion.c_str(),33333);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  if (udp1.listen(33446)) 
  {
    receive_udp();
    delay(100);
   }
}

void getAcceleroMotion()
{
  
  if(mpu.getMotionInterruptStatus()) 
  {
    /* Get new sensor events with the readings */
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    /* Print out the values */
    Serial.print("AccelX:");
    Serial.print(a.acceleration.x);
    Serial.print(",");
    Serial.print("AccelY:");
    Serial.print(a.acceleration.y);
    Serial.print(",");
    Serial.print("AccelZ:");
    Serial.print(a.acceleration.z);
    Serial.print(", ");
    
    /*Serial.print("GyroX:");
    Serial.print(g.gyro.x);
    Serial.print(",");
    Serial.print("GyroY:");
    Serial.print(g.gyro.y);
    Serial.print(",");
    Serial.print("GyroZ:");
    Serial.print(g.gyro.z);
    Serial.println("");*/

    if(a.acceleration.x>9.5)
    {
      motionx="x front";
    }
    else if(a.acceleration.x<10.5)
    {
      motionx="x back";
    }
    else
    {
      motionx="x stationary";
    }

    if(a.acceleration.y>-0.5)
    {
      motiony="y right";
    }
    else if(a.acceleration.y<0.5)
    {
      motiony="y left";
    }
    else
    {
      motiony="y stationary";
    }
    
    if(a.acceleration.z>11)
    {
      motionz="z up";
    }
    else if(a.acceleration.z<8.4)
    {
      motionz="z down";
    }
    else
    {
      motionz="z stationary";
    }

    motion = motionx + ", " + motiony + ",  " + motionz;
    udp.broadcastTo(motion.c_str(),33333);
  }
  delay(10);
}

void receive_udp()
{
    Serial.print("UDP Listening on IP: ");
    Serial.println(WiFi.localIP());
    udp1.onPacket([](AsyncUDPPacket packet) 
    {
      Serial.print("UDP Packet Type: ");
      Serial.print(packet.isBroadcast() ? "Broadcast" : packet.isMulticast() ? "Multicast" : "Unicast");
      Serial.print(", From: ");
      Serial.print(packet.remoteIP());
      Serial.print(":");
      Serial.print(packet.remotePort());
      Serial.print(", To: ");
      Serial.print(packet.localIP());
      Serial.print(":");
      Serial.print(packet.localPort());
      Serial.print(", Length: ");
      Serial.print(packet.length()); //dlzka packetu
      Serial.print(", Data: ");
      Serial.write(packet.data(), packet.length());
      Serial.println();
      String myString = (const char*)packet.data();
      
      if (myString == "hit") 
      {
        //Serial.println("hitinam 2");
        digitalWrite(2, HIGH);
        delay(100);
        digitalWrite(2, LOW);
      } 
      else if (myString == "VYP") 
      {
        Serial.println("Vypinam 2");
        digitalWrite(2, LOW);
      }
      packet.printf("Got %u bytes of data", packet.length());
    });
}
