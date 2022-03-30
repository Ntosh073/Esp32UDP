#include "WiFi.h"
#include "AsyncUDP.h"
const char* ssid = "TP-Link_5EFE";
const char* pass = "26545653";
//const int rele = 23;
AsyncUDP udp;
void setup()
{
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

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
void loop()
{

   while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  if (udp.listen(33447)) 
  {
    receive_udp();
    udp.broadcast("Brusley test here??");
    delay(100);

  }//*/

}
