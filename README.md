# esp8266wificon
esp8266 网页配网库
简洁易用，支持认证界面自动跳转，支持保存掉电不丢失。


示例

#include <wificon.h>

wificon wfc;

void setup() {
  wfc.wifiBegin();

}

void loop() {
  wfc.wifiLoop();

}

api

wifiBegin;（必要）

wifiLoop;（必要）

clearWiFi;（可选）
