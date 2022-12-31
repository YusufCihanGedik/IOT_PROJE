//Welcome to The IoT Projects
//IoT Based Fire Alarm Notification and control system using ESP8266

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "ThingSpeak.h"
#include <UniversalTelegramBot.h>   // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h>

#define BOTtoken "5797458670:AAGSL3EwTLIFebluP9ZuO9e8OfJ-TVqy1T0"  // your Bot Token (Get from Botfather)
 const char* writeAPIKey = "DIPV5IT5WCZ28L5B";     //  Thingspeak  write API Key
const char* readAPIKey  = "W2LXBCSROPEAJI64"; 
const char* thingSpeakHost = "api.thingspeak.com";     //ThingSpeak sitesinin hostu
String thingtweetAPIKey = "41NE3IKP9WPESJ2P";    //ThingTweet sayfasından aldığımız API Key
// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
#define CHAT_ID "1690850255"
 X509List cert(TELEGRAM_CERTIFICATE_ROOT); 
 WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
int LED = D2;
int RELAY = D4;
int Flame_sensor = D1;
int Flame_detected;
String tsData;



char ssid[] = "gedik";
char pass[] = "123456789";

void notifyOnFire()
{
  Flame_detected = digitalRead(Flame_sensor);
  Serial.println(Flame_detected);
  //delay(100);
  if (Flame_detected == 0) {
    Serial.println("DİKKAT!!YANGIN ALARIMI!!");
    bot.sendMessage(CHAT_ID, "DİKKAT!!YANGIN ALARIMI!!", "");
    digitalWrite(LED, HIGH);
    digitalWrite(RELAY, HIGH);
       
 
 
    bot.sendMessage(CHAT_ID, "Yangın Söndü", "");
  }
  else
  {
     
    Serial.println("YANGIN SONDÜRÜLDÜ!!");
    digitalWrite(LED, LOW);
    digitalWrite(RELAY, LOW);
  }
}
void setup()
{
  Serial.begin(9600);
 ThingSpeak.begin(client);
  pinMode(LED, OUTPUT);
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, LOW);
  pinMode(Flame_sensor, INPUT_PULLUP);
   configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
  client.setTrustAnchors(&cert);
   WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  
}
void loop()
{
notifyOnFire();
 if (client.connect("api.thingspeak.com",80)) {                           // thingtweet ile tweet at
      tsData = "api_key="+thingtweetAPIKey+"&status="+"DİKKAT YANGIN VAR!!!!!";
      client.print("POST /apps/thingtweet/1/statuses/update HTTP/1.1\n");
      client.print("Host: api.thingspeak.com\n");
      client.print("Connection: close\n");
      client.print("Content-Type: application/x-www-form-urlencoded\n");
      client.print("Content-Length: ");
      client.print(tsData.length());
      client.print("\n\n");
      client.print(tsData);
      Serial.println("Tweet Send");

    }

}
