#include "ArduinoJson.h"
#include "./src/QuickInternetOfThings.h"

char* ssid = "ChinaNet-KNQ5ZC";
char* pwd = "zhang@163.com";

/**
连接阿里云信息
*/
// char* mqtt_server = "iot-06z00i7bokwqqid.mqtt.iothub.aliyuncs.com";
// uint16_t port = 1883;
// char* mqtt_clientId = "k00wmvASb4P.1000-00111|securemode=2,signmethod=hmacsha256,timestamp=1691332562802|";
// char* mqtt_user = "1000-00111&k00wmvASb4P";
// char* mqtt_pwd = "bcdb0b309f9e9b80a706cc5982c6429d3fa287a31753c5bda64623eda2d6772b";
// char* productkey = "k00wmvASb4P";
// char* deviceName = "1000-00111";

/**
连接quick-iot信息
*/
char* mqtt_server = "43.139.141.111";
uint16_t port = 1883;
char* mqtt_clientId = "quick_iot_esp8266";
char* mqtt_user = "quick";
char* mqtt_pwd = "quick";
char* productkey = "k00wmvASb4P";
char* deviceName = "1000-00111";

QuickInternetOfThings iot = QuickInternetOfThings(ssid, pwd, mqtt_server, port, mqtt_clientId, mqtt_user, mqtt_pwd, productkey, deviceName);

//回调
void callback(char* topic, byte* payload, unsigned int length) {
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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  iot.begin();
  // iot.disconnect();
}

void loop() {
  // put your main code here, to run repeatedly:
  iot.loop();
  char* tipic = "/sys/k00wmvASb4P/1000-00111/thing/event/property/post";
  StaticJsonDocument<200> jsonBuffer;
  // DynamicJsonDocument data(256);
  jsonBuffer["id"] = 1111111;
  jsonBuffer["version"] = "1.0";
  jsonBuffer["method"] = "thing.event.property.post";
  JsonObject params = jsonBuffer.createNestedObject("params");
  params["status"] = "开";
  params["CurrentTemperature"] = random(0, 100);
  params["CurrentHumidity"] = random(0, 100);
  char payload[1000];
  Serial.println(payload);
  // char json_string[256];
  // serializeJson(data, json_string);
  serializeJson(jsonBuffer, payload);
  Serial.println(payload);
  iot.publish(tipic, payload);
  delay(5000);
}
