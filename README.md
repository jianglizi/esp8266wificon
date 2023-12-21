# esp8266简洁易用网页配网库

#### 介绍

esp8266简洁易用网页配网库，支持配网界面自动跳转，支持保存wifi密码

#### 安装教程

arduino导入压缩包

#### 使用说明
示例


```c++
#include <wificon.h>

wificon wfc;

void setup() {
	wfc.wifiBegin();
}

void loop() { 
	wfc.wifiLoop();
}

```


api

void wifiBegin(void);（必要）

void wifiLoop(void);（必要）

void clearWiFi(void);（可选）

void setSerialOff(void);关闭串口输出

void setSerialOn(void);打开串口输出（默认开）

void setWifiDetectOff(void);（关闭WiFi断线检测，默认关，必要条件wifiLoop()）


#### 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request

