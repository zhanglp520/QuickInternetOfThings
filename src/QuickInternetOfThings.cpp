/**
QuickInternetOfThings.cpp - Library for connection mqtt
Create by zhanglp,7 August 2023
*/

#include "Arduino.h"
#include "ESP8266WiFi.h"
#include "PubSubClient.h"
#include "./QuickInternetOfThings.h"

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;
int pin = 2;  //LED_BUILTIN

QuickInternetOfThings::QuickInternetOfThings(char* ssid, char* pwd, char* url, uint16_t port, char* clientId, char* mqttUser, char* mqttPwd, char* productkey, char* deviceName) {
  Serial.println("QuickInternetOfThings  init.");
  this->_ssid = ssid;
  this->_pwd = pwd;
  this->_url = url;
  this->_port = port;
  this->_clientId = clientId;
  this->_mqttUser = mqttUser;
  this->_mqttPwd = mqttPwd;
  this->_productkey = productkey;
  this->_deviceName = deviceName;
}
//开始
void QuickInternetOfThings::begin() {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  connection_wifi();
  client.setServer(this->_url, this->_port);
  client.setCallback(QuickInternetOfThings::callback);
  //重写PubSubClient.h头文件中的MQTT_KEEPALIVE默认的15
  //CONNECT指令中需包含KeepAlive（保活时间）。保活心跳时间取值范围为30秒~1200秒。如果心跳时间不在此区间内，物联网平台会拒绝连接。建议取值300秒以上。如果网络不稳定，将心跳时间设置高一些。
  client.setKeepAlive(60);  //500
  connect();
}
//循环
void QuickInternetOfThings::loop() {
  if (!client.connected()) {
    reconnect();
  }
  delay(5000);
  client.loop();
}
//连接
void QuickInternetOfThings::connect() {
  if (client.connect(this->_clientId, this->_mqttUser, this->_mqttPwd)) {
    Serial.println("connected");
    char* tipic = "/sys/k00wmvASb4P/1000-00111/thing/event/property/post";
    subscribe(tipic);
  } else {
    Serial.print("failed, rc=");
    Serial.print(client.state());
    Serial.println(" try again in 5 seconds");
  }
}
//发布
void QuickInternetOfThings::publish(char* tipic, char* payload) {
  Serial.println("QuickInternetOfThings  publish.");
  if (client.publish(tipic, payload, false)) {
    Serial.println("Publish Topic:");
    Serial.println(tipic);
    Serial.println("Publish message:");
    Serial.println(payload);
    Serial.println("Publish success.");
  } else {
    Serial.println("Publish Failed.");
  }
}
//订阅
void QuickInternetOfThings::subscribe(char* tipic) {
  client.subscribe(tipic);
  Serial.println("subscribe success.");
}
//撤销订阅
// void QuickInternetOfThings::unSubscribe(char* tipic) {
//   client.unSubscribe(tipic);
//   Serial.println("unSubscribe success.");
// }
//断开连接
void QuickInternetOfThings::disconnect() {
  char* tipic = "/sys/k00wmvASb4P/1000-00111/thing/event/property/post";
  // unSubscribe(tipic);
  client.disconnect();
  Serial.println("disconnect success.");
}
//连接wifi
void QuickInternetOfThings::connection_wifi() {
  delay(1000);
  Serial.println("欢迎使用esp8266连接wifi.");
  Serial.print("connecting to ");
  Serial.println(this->_ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(this->_ssid, this->_pwd);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(pin, HIGH);
    delay(500);
    Serial.print(".");
    digitalWrite(pin, LOW);
    delay(500);
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
//重连
void QuickInternetOfThings::reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    // String clientId = "ESP8266Client-";
    // clientId += String(random(0xffff), HEX);
    // Attempt to connect
    // if (client.connect(clientId.c_str())) {
    Serial.println(this->_clientId);
    Serial.println(this->_mqttUser);
    Serial.println(this->_mqttPwd);
    connect();
  }
}
//回调
void QuickInternetOfThings::callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);  // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }
}