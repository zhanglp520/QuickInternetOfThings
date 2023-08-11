/**
QuickInternetOfThings.h - Library for connection mqtt
Create by zhanglp,7 August 2023
*/

#ifndef QuickIOT_H
#define QuickIOT_H

#include "Arduino.h"

// #define MQTT_CALLBACK_SIGNATURE void (*callback)(char*, byte*, unsigned int)

class QuickInternetOfThings {
private:
  char* _ssid;
  char* _pwd;
  char* _url;
  uint16_t _port;
  char* _clientId;
  char* _mqttUser;
  char* _mqttPwd;
  char* _productkey;
  char* _deviceName;
  // MQTT_CALLBACK_SIGNATURE;
  void connection_wifi();
  void reconnect();
  void subscribe(char* tipic);
  // void unSubscribe(char* tipic);
  static void callback(char* topic, byte* payload, unsigned int length);

public:
  QuickInternetOfThings(char* ssid, char* pwd, char* url, uint16_t port, char* clientId, char* mqttUser, char* mqttPwd, char* productkey, char* deviceName);
  void begin();
  void loop();
  void connect();
  void publish(char* tipic, char* payload);
  void disconnect();
};
#endif