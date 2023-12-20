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

wifiBegin;（必要）

wifiLoop;（必要）

clearWiFi;（可选）


#### 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request

