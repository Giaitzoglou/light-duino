#ifndef mqtt_h
#define mqtt_h

/*
 *  light-duino v2
 *  MQTT <-> DMX controller with hw switches, based on ESP8266 
 *  See attached Readme.md for details
 *  
 *  This is based on the work of Jorgen (aka Juergen Skrotzky, JorgenVikingGod@gmail.com), buy him a beer. ;-) 
 *  Rest if not noted otherwise by Peter Froehlich, tarwin@tarwin.de - Munich Maker Lab e.V. (January 2016)
 *  
 *  Published under Creative Commons Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
 *  You'll find a copy of the licence text in this repo. 
 */

#include <WiFiClient.h>
#include <PubSubClient.h>
#include "config.h"

bool mqttNewMessage = false;
String mqttTopic   = "";
String mqttPayload = "";

IPAddress server(0, 0, 0, 0);

// initial mqtt client with mqtt broker server ip address
WiFiClient client;
PubSubClient mqttClient(client, server);

// handle received serial data
void publishMQTTMessage(String strTopic, String strMessage) {
  // publish mqtt message with given topic
  mqttClient.publish(strTopic, strMessage);
  Serial.println("send MQTT: topic='" + strTopic + "', message='" + strMessage + "'");
}

void subscribeMQTTTopic(String strTopic) {
  mqttClient.subscribe(strTopic);
  Serial.println("subscribe MQTT: topic='" + strTopic + "'");
}

bool connectMQTT(String strUser, String strPass, String strHost, uint16_t port = mqtt_port) {
  // connect to mqtt broker
  mqttClient.set_server(strHost, port);
  bool MQTTconnected = false;
  int retries = 0;
  while(!MQTTconnected && retries < 10) {
    retries++;
    String strClientID = String(mqtt_client_id);
    if (mqttClient.connect(MQTT::Connect(strClientID).set_auth(strUser, strPass))) {
      MQTTconnected = true;
    }
    delay(10);
  }
  return MQTTconnected;
}

void processMQTTLoop() {
  // handle mqtt messages and wifi connection
  if (mqttClient.connected())
    mqttClient.loop();
}

#endif //mqtt_h