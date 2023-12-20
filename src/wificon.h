#ifndef _WIFICON_H_
#define _WIFICON_H_

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <FS.h>


class wificon
{
private:
  static bool boolwifistatus;
  static char sta_ssid[32];
  static char sta_password[64];
  static bool booloffSerial;
  static void initSoftAP(void);
  static void initWebServer(void);
  static void initDNS(void);
  static void handleRoot(void);
  static void handleRootPost(void);
  static void connectWifi(void);
  static void STA_CON(String Mname, String Mssid);
  static void AP_GetWifi(void);
  static bool saveWifi(void);
  static bool FS_W(String str,int sw);
  static String FS_R(int sw);
  static void mySerial(String str, bool nend);


public:
  void wifiBegin(void);
  void wifiLoop(void);
  void clearWiFi(void);


};

bool wificon::booloffSerial = false;
bool wificon::boolwifistatus = false;

IPAddress apIP(192, 168, 1, 1); //esp8266-AP-IP地址
DNSServer dnsServer;  //创建dnsServer实例
ESP8266WebServer server(80);  //创建WebServer

//暂时存储wifi账号密码
char wificon::sta_ssid[32] = {0};
char wificon::sta_password[64] = {0};
const char* AP_NAME = "配网WIFI";
const byte DNS_PORT = 53; //DNS端口号

//配网页面代码
const char* page_html = "\
<!DOCTYPE html>\r\n\
<html lang='en'>\r\n\
<head>\r\n\
<meta charset='UTF-8'>\r\n\
<meta name='viewport' content='width=device-width, initial-scale=1.0'>\r\n\
<title>Document</title>\r\n\
</head>\r\n\
<body>\r\n\
<form name='input' action='/' method='POST'>\r\n\
        wifi名称: <br>\r\n\
        <input type='text' name='ssid'><br>\r\n\
        wifi密码:<br>\r\n\
        <input type='text' name='password'><br>\r\n\
        <input type='submit' value='保存'>\r\n\
    </form>\r\n\
</body>\r\n\
</html>\r\n\
";




void wificon:: wifiBegin(void){
  Serial.begin(115200);
  Serial.println();
  WiFi.hostname("Smart-ESP8266");//设置ESP8266设备名
  //SPIFFS.format();
  if(SPIFFS.begin()){ // 启动SPIFFS
    Serial.println("SPIFFS Started.");
  } else {
    Serial.println("SPIFFS Failed to Start.");
  }
  connectWifi();
}

void wificon:: wifiLoop(void){
  while (!boolwifistatus)
  {
    server.handleClient();
    dnsServer.processNextRequest();
  }
  
  
}


void wificon:: clearWiFi(void) {
  SPIFFS.format();
  WiFi.disconnect();
}


void wificon:: mySerial(String str, bool nend) {
  if (! booloffSerial) {
    if (nend)
      Serial.println(str);
    else
      Serial.print(str);
  }
}


void wificon:: initSoftAP(void){//初始化AP模式
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  if(WiFi.softAP(AP_NAME)){
    Serial.println("ESP8266 SoftAP is right");
  }
}

void wificon:: initWebServer(void){//初始化WebServer
  //server.on("/",handleRoot);
  //上面那行必须以下面这种格式去写否则无法强制门户
  server.on("/", HTTP_GET, handleRoot);//设置主页回调函数
  server.onNotFound(handleRoot);//设置无法响应的http请求的回调函数
  server.on("/", HTTP_POST, handleRootPost);//设置Post请求回调函数
  server.begin();//启动WebServer
  Serial.println("WebServer started!");
}

void wificon:: initDNS(void){//初始化DNS服务器
  if(dnsServer.start(DNS_PORT, "*", apIP)){//判断将所有地址映射到esp8266的ip上是否成功
    Serial.println("start dnsserver success.");
  }
  else Serial.println("start dnsserver failed.");
}

void wificon:: handleRoot(void) {//访问主页回调函数
  server.send(200, "text/html", page_html);
}

void wificon:: handleRootPost(void) {//Post回调函数
  Serial.println("handleRootPost回调函数运行");
  if (server.hasArg("ssid")) {//判断是否有账号参数
    Serial.print("got ssid:");
    strcpy(sta_ssid, server.arg("ssid").c_str());//将账号参数拷贝到sta_ssid中
    Serial.println(sta_ssid);
    //密码与账号同理
  if (server.hasArg("password")) {
    Serial.print("got password:");
    strcpy(sta_password, server.arg("password").c_str());
    Serial.println(sta_password);
  } else {
    Serial.println("error, not found password");
    server.send(200, "text/html", "<meta charset='UTF-8'>error, not found password");
    return;
  }
  } else {//没有参数
    Serial.println("error, not found ssid");
    server.send(200, "text/html", "<meta charset='UTF-8'>error, not found ssid");//返回错误页面
    return;
  }
  server.send(200, "text/html", "<meta charset='UTF-8'>保存成功");//返回保存成功页面
  delay(500);

  if( saveWifi() ){connectWifi();}
  
}


void wificon:: connectWifi(void){
  String s=FS_R(1);
  if(s=="NULL"){
    mySerial("未检测到WiFi，执行配网程序", true);
    AP_GetWifi();
  }else{
    mySerial("已保存：", false);
    mySerial(FS_R(1), true);
    mySerial(FS_R(2), true);
    STA_CON(FS_R(1),FS_R(2));
  }
}

void wificon:: STA_CON(String Mname, String Mssid) {
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(Mname.c_str(), Mssid.c_str());
  mySerial("开始连接WiFi[", false);
  mySerial(Mname, false);
  mySerial(Mssid, false);
  mySerial("]:", false);
  int i = 0;
  int SECtime=20;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); 
    mySerial(",", false);
    i++;
    if (i > SECtime - 1) {
      Serial.println();
      mySerial("WiFi连接超时,正在重试", true);
      AP_GetWifi();
      break;
    }
  }
  if (WiFi.isConnected()) {
    Serial.println();

    boolwifistatus= true ;
    dnsServer.stop();
    server.close();

    mySerial("连接成功", true);
    mySerial("本机IP:", false);
    mySerial(WiFi.localIP().toString(), true);
  }
  mySerial(">>>>> end <<<<<", true);
  Serial.println();
}

void wificon:: AP_GetWifi(void){
  initSoftAP();
  initWebServer();
  initDNS();
}


bool wificon:: saveWifi(void){
  mySerial("开始保存WiFi", true);
  FS_W(sta_ssid,1);
  FS_W(sta_password,2);

  mySerial("已保存：", false);
  mySerial(FS_R(1), true);
  mySerial(FS_R(2), true);
  return true;
}


bool wificon:: FS_W(String str,int sw) {
  if(sw==1){
    File dataFile = SPIFFS.open("/wificon/wifissid.txt", "w");
    dataFile.print(str);
    dataFile.close();
    mySerial("保存ssid:", false);
    mySerial(str, true);
  }else if(sw==2){
    
    File dataFile = SPIFFS.open("/wificon/wifipassword.txt", "w");
    dataFile.print(str);
    dataFile.close();
    mySerial("保存password:", false);
    mySerial(str, true);
  }
  
  
  return true;
}


String wificon:: FS_R(int sw) {
  if(sw==1){
     if (!SPIFFS.exists("/wificon/wifissid.txt")) {
        mySerial("ssid不存在", true);

        File dataFile = SPIFFS.open("/wificon/wifissid.txt", "r");
        String retStr;
        for (int i = 0; i < dataFile.size(); i++) {
          retStr += (char)dataFile.read(); }
        dataFile.close();

        mySerial(retStr, true);



        return "NULL";}
      File dataFile = SPIFFS.open("/wificon/wifissid.txt", "r");
      String retStr;
      for (int i = 0; i < dataFile.size(); i++) {
        retStr += (char)dataFile.read(); }
      dataFile.close();
      return retStr;
    } else if(sw==2){ 
      if(!SPIFFS.exists("/wificon/wifipassword.txt")){
        mySerial("passwd不存在", true);
        return "NULL";}
      File dataFile = SPIFFS.open("/wificon/wifipassword.txt", "r");
      String retStr;
      for (int i = 0; i < dataFile.size(); i++) {
        retStr += (char)dataFile.read(); }
      dataFile.close();
      return retStr;
    }else{return "NULL";}
}

#endif