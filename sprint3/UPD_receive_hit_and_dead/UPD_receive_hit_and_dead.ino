#include "WiFi.h"
#include "AsyncUDP.h"
const char* ssid = "TP-Link_5EFE";
const char* pass = "26545653";
AsyncUDP udp;
//bool ran=false;

void setup()
{
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  pinMode(5,OUTPUT);
  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  udp.begin(33446);
}

void receive_udp()
{
  
    Serial.print("UDP Listening on IP: ");
    Serial.println(WiFi.localIP());
    udp.onPacket([](AsyncUDPPacket packet) 
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
      Serial.print(packet.length());
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
      
      else 
      {
        Serial.println("dead");
        digitalWrite(5, HIGH);
        digitalWrite(2, HIGH);
        delay(100);
        digitalWrite(2, LOW);
        delay(700);
        digitalWrite(5, LOW);
      }
      packet.printf("Got %u bytes of data", packet.length());
    });
}

void loop()
{
  /*digitalWrite(2,HIGH);
  delay(500);
  digitalWrite(2,LOW);
  delay(500);*/
  
   while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  
  if (udp.listen(33446)) 
  {
    
    receive_udp();
    
    //if(!ran)
    //{
      udp.broadcast(("Stress vest IP: "+WiFi.localIP().toString()).c_str());
      
      //ran=true;
    //}
    delay(100);
     
  }//*/
}
