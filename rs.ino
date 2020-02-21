#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <Time.h>
#include <TimeLib.h> 
#include <DNSServer.h>
#include <ESP8266HTTPClient.h>
#include <WiFiUdp.h>
#include <WiFiClientSecure.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <FS.h>  
#define rs D3 //rest
#define btn D1 //d4触发
#define btn2 D2 //d5触发

unsigned int ren=0;//人数
int cfo=0;//激光1
int cft=0;//激光2
int bj=-1;//标记点 -1无触发  0 进人 1出人

//=====================================================================================================================================================
String Hostname = "rs";     //主机名，连上WiFi后通过该值访问8266
String url = Hostname + ".com";    //配网时的访问地址，主机名+.dog，或者直接访问6.6.6.6（尽量用com、cn、org等存在的域名，否则浏览器可能会以搜索执行）
const char *AP_name = "RS";    //配网时8266开启的热点名
int Signal_filtering = -60;   //搜索结果过滤掉信号强度低于这个值的WiFi（不了解勿改动）
//=====================================================================================================================================================

const byte DNS_PORT = 53;
String WiFi_State;    //WiFi配置状态  0：未配置  1：已配置
#define WiFi_State_Addr 0   //记录WiFi配置状态的EEPROM地址
String disp = "";
String type = "";
String parm = "";
long time_out = millis();

IPAddress apIP(6, 6, 6, 6);
ESP8266WebServer webServer(80);
DNSServer dnsServer;



IPAddress timeServer(120, 25, 115, 20); // 阿里云ntp服务器 如果失效可以使用 120.25.115.19   120.25.115.20
#define STD_TIMEZONE_OFFSET +8 //设置中国
const int timeZone = 8;     // 修改北京时区
WiFiUDP Udp;
unsigned int localPort = 8888;  // 修改udp 有些路由器端口冲突时修改
int servoLift = 1500;


int pd=1;

unsigned int months19[12]={0,0,0,0,0,0,0,0,0,0,0,0};
unsigned int months20[12]={0,0,0,0,0,0,0,0,0,0,0,0};
unsigned int months21[12]={0,0,0,0,0,0,0,0,0,0,0,0};
unsigned int months22[12]={0,0,0,0,0,0,0,0,0,0,0,0};
unsigned int months23[12]={0,0,0,0,0,0,0,0,0,0,0,0};
unsigned int months24[12]={0,0,0,0,0,0,0,0,0,0,0,0};
unsigned int months25[12]={0,0,0,0,0,0,0,0,0,0,0,0};
unsigned int dy=0;
unsigned int days=0;
unsigned int zs=0;









void zdo()  // 
{  
  int millis_time=millis();
  int opera=0;
  while(digitalRead(btn)==LOW){
    if(millis()-millis_time>2000){
      opera=2;
      break;
    }else if (millis()-millis_time>1){
      opera=1;
    }
       ESP.wdtFeed();//喂狗
  }
  if(opera==1){
   if(cft==0){
    cfo=1;
        }else if(cft==1){
        bj=1;
         cft=0;
      }
    
    


    
  Serial.println("one触发");
  
   //
  }else if(opera==2){
  //
  Serial.println("失效");
  }
}


void zdt()  //
{  
  int millis_time=millis();
  int opera=0;
  while(digitalRead(btn2)==LOW){
    if(millis()-millis_time>2000){
      opera=2;
      break;
    }else if (millis()-millis_time>1){
      opera=1;
    }
       ESP.wdtFeed();//喂狗
  }
  if(opera==1){

   if(cfo==0){
    cft=1;
        }else if(cfo==1){
         cfo=0;
        bj=0;
      }
      
  Serial.println("two触发");
   //
  }else if(opera==2){
  //
  Serial.println("失效");
  }
}




void setup() 
{
  attachInterrupt(digitalPinToInterrupt(rs), blink, FALLING);
  Serial.begin(115200);

pinMode(D8, OUTPUT);
digitalWrite(D8, LOW);
pinMode( btn, INPUT_PULLUP);
pinMode( btn2, INPUT_PULLUP);

Serial.println("load.....");  //中断监听
attachInterrupt( digitalPinToInterrupt(btn), zdo, CHANGE); 
attachInterrupt( digitalPinToInterrupt(btn2), zdt, CHANGE); 
  
 Serial.print("IP number assigned by DHCP is ");
  Serial.println(WiFi.localIP());
  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());
  Serial.println("waiting for sync");
  
  //
     Serial.println("");
  EEPROM.begin(10);
  SPIFFS.begin();
  //  设置主机名
  WiFi.hostname(Hostname);
  //pinMode(2, OUTPUT);
  //获取WiFi配置状态

  
  WiFi_State = EEPROM.read(WiFi_State_Addr);
  delay(300);
  if (WiFi_State == "1")   //已配置
  {
    Serial.println("Configured!");
    Serial.print("Connecting");
    unsigned millis_time = millis();
    while ((WiFi.status() != WL_CONNECTED) && (millis() - millis_time < 5000))
    {
      delay(250);
      Serial.print(".");
    }
    Serial.println("");
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.print("IP Address: ");
      Serial.println(WiFi.localIP());
      Serial.print("http://");
      Serial.println(Hostname);
     // digitalWrite(2, HIGH);
     while( pd==1){
  


setSyncProvider(getNtpTime);
  Serial.println(year());
    Serial.println(month());
      Serial.println(day());
       Serial.println(hour());
        Serial.println(minute());
      
}
    }
    else
    {
      Serial.println("Connection failed!");
      Serial.println("Please reset!");
     // digitalWrite(2, LOW);
    }
  }
  else if (WiFi_State == "0")   //未配置
  {
    //digitalWrite(2, LOW);
    WiFi.disconnect(true);
    Serial.println("");
    Serial.print("Start WiFi config \nsoftAP srarted --> ");
    Serial.println(AP_name);
    Serial.print("http://");
    Serial.println(url);
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(AP_name);
  }
  else
  {
    Serial.println("First using!");
    EEPROM.write(WiFi_State_Addr, 0);
    EEPROM.commit();
    delay(300);
    //    Serial.println("Reset! Please wait...");
    //    ESP.restart();
    Serial.println("Press the 'rst' button!");
    while (1) {
    //
    }
  }

  // Web开启服务
  webServer.begin();
  Serial.println("HTTP Server started");
  webServer.on("/", wwwroot);
    webServer.on("/index", index2);
  webServer.on("/wificonfig", wifiConfig);
  webServer.on("/wifiscan", wifiScan);
  webServer.on("/opera", opera);
  webServer.on("/sb", sb);
  webServer.on("/people", people);
  // 配置DNS服务
  dnsServer.setTTL(300);
  dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
  dnsServer.start(DNS_PORT, url, apIP);

   delay(500);
   

  
 //开机更新
  EEPROM.begin(4096);


  EEPROM.get(300,days);  //读取当天人数
  EEPROM.get(304,zs);  //读取总人数
 
  
 //读取当月总人数

  if(year()==2019){  EEPROM.get(400+4*month(),dy);   }
   if(year()==2020){ EEPROM.get(500+4*month(),dy);   }
    if(year()==2021){ EEPROM.get(600+4*month(),dy);  }
     if(year()==2022){ EEPROM.get(700+4*month(),dy);   }
      if(year()==2023){ EEPROM.get(800+4*month(),dy);   }
       if(year()==2024){ EEPROM.get(900+4*month(),dy);   }
        if(year()==2025){ EEPROM.get(1000+4*month(),dy);  }
     

   Serial.println("系统记录总数：");
     Serial.println(zs);
       Serial.println("系统记录当月人数：");
         Serial.println(dy);
           Serial.println("当天人数：");
             Serial.println(days);


//读取人数信息
   for(int i=0;i<12;i++){
   EEPROM.get(400+4*(i+1),months19[i]); 
   EEPROM.get(500+4*(i+1),months20[i]);  
   EEPROM.get(600+4*(i+1),months21[i]);
   EEPROM.get(700+4*(i+1),months22[i]);
   EEPROM.get(800+4*(i+1),months23[i]);
   EEPROM.get(900+4*(i+1),months24[i]);
   EEPROM.get(1000+4*(i+1),months25[i]);
    }

  
















}






void loop() 
{
  dnsServer.processNextRequest();
  webServer.handleClient();
if(minute()==22){
  //
  EEPROM.put(300,days);  //读取当天人数
  EEPROM.put(304,zs);  //读取总人数
 
  
 //读取当月总人数

  if(year()==2019){  EEPROM.put(400+4*month(),dy);   }
   if(year()==2020){ EEPROM.put(500+4*month(),dy);   }
    if(year()==2021){ EEPROM.put(600+4*month(),dy);  }
     if(year()==2022){ EEPROM.put(700+4*month(),dy);   }
      if(year()==2023){ EEPROM.put(800+4*month(),dy);   }
       if(year()==2024){ EEPROM.put(900+4*month(),dy);   }
        if(year()==2025){ EEPROM.put(1000+4*month(),dy);  }
    EEPROM.commit();
  
  }

if(hour()==23&&minute()==00){
  unsigned int a=0;
  EEPROM.put(300,a);  //重置当天人数
     EEPROM.commit();
  setSyncProvider(getNtpTime);
  }
//人数判断
  
  if(bj!=-1){
       

    if(bj==0){
      ++ren;
      ++days;
      ++zs;
      ++dy;
      digitalWrite(D8, HIGH);
      delay(10);
       digitalWrite(D8, LOW);
      }else if(bj==1){
        if(ren==0){ren=1;}
         --ren;
        
        
        }

       cfo=0;//激光1
       cft=0;//激光2
       bj=-1;
      
    Serial.println("当前人数：");
   Serial.println(ren);
    }




}



void digitalClockDisplay(){
  //
  Serial.print(hour());
  printDigits(minute());
 Serial.println();

}
void printDigits(int digits){
  // utility for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}


const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("连接时间服务器");
  sendNTPpacket(timeServer);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("时间服务器应答");
      pd=0;
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("链接服务器失败:-(");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;    
  packetBuffer[3] = 0xEC; 

  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;                
  Udp.beginPacket(address, 123);
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}


//Web服务根目录

void index2(){
   File file = SPIFFS.open("/index2.html", "r");
    webServer.streamFile(file, "text/html");
    file.close();
  
  }
void wwwroot()
{
  //  如果已经配置WiFi则访问data目录里的index.html页面，否则访问config.html页面进行配置
  if (WiFi_State == "1")
  {
    File file = SPIFFS.open("/index.html", "r");
    webServer.streamFile(file, "text/html");
    file.close();
  }
  else if (WiFi_State == "0")
  {
    File file = SPIFFS.open("/config.html", "r");
    webServer.streamFile(file, "text/html");
    file.close();
  }
}

//用于WiFi配置，接收ssid、password两个参数
void wifiConfig()
{
  // 判断请求是否存在这两个参数以及是否未配置
  if (webServer.hasArg("ssid") && webServer.hasArg("password") && WiFi_State == "0")
  {
    int ssid_len = webServer.arg("ssid").length();
    int password_len = webServer.arg("password").length();
    // 判断ssid和密码长度是否合理
    if ((ssid_len > 0) && (ssid_len < 33) && (password_len > 7) && (password_len < 65))
    {
      // 把参数获得的String赋值到ssid
      String ssid_str = webServer.arg("ssid");
      String password_str = webServer.arg("password");
      const char *ssid = ssid_str.c_str();
      const char *password = password_str.c_str();
      Serial.print("SSID: ");
      Serial.println(ssid);
      Serial.print("Password: ");
      Serial.println(password);
      // 开始连接WiFi
      WiFi.begin(ssid, password);
      Serial.print("Connenting");
      //判断是否连接成功，8秒超时
      unsigned long millis_time = millis();
      while ((WiFi.status() != WL_CONNECTED) && (millis() - millis_time < 8000))
      {
        delay(500);
        Serial.print(".");
      }
      // 判断是否连接成功
      if (WiFi.status() == WL_CONNECTED)
      {
        // 连接成功，返回1
        digitalWrite(2, HIGH);
        Serial.println("");
        Serial.println("Connected successfully!");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
        Serial.print("http://");
        Serial.println(Hostname);
        webServer.send(200, "text/plain", "1");
        delay(300);
        WiFi_State = "1";
        // 记录状态到EEPROM：已配置
        EEPROM.write(WiFi_State_Addr, 1);
        EEPROM.commit();
        delay(50);
        //关闭热点
        WiFi.softAPdisconnect();
        delay(5000); ////////////////
        ESP.restart();
      }
      else
      {
        // 连接失败, 返回0
        Serial.println("Connenting failed!");
        webServer.send(200, "text/plain", "0");
      }
    }
    else
    {
      // WiFi密码格式错误
      Serial.println("Password format error");
      webServer.send(200, "text/plain", "Password format error");
    }
  }
  else
  {
    // 请求参数错误
    Serial.println("Request parameter error");
    webServer.send(200, "text/plain", "Request parameter error");
  }
}

//获取WiFi加密类型并返回
String wifi_type(int typecode)
{
  if (typecode == ENC_TYPE_NONE) return "Open";
  if (typecode == ENC_TYPE_WEP) return "WEP ";
  if (typecode == ENC_TYPE_TKIP) return "WPA ";
  if (typecode == ENC_TYPE_CCMP) return "WPA2";
  if (typecode == ENC_TYPE_AUTO) return "WPA*";
}

//扫描WiFi，返回json
void wifiScan()
{
  String req_json = "";
  Serial.println("Scan WiFi");
  int n = WiFi.scanNetworks();
  if (n > 0)
  {
    int m = 0;
    req_json = "{\"req\":[";
    for (int i = 0; i < n; i++)
    {
      if ((int)WiFi.RSSI(i) >= Signal_filtering)
      {
        m++;
        req_json += "{\"ssid\":\"" + (String)WiFi.SSID(i) + "\"," + "\"encryptionType\":\"" + wifi_type(WiFi.encryptionType(i)) + "\"," + "\"rssi\":" + (int)WiFi.RSSI(i) + "},";
      }
    }
    req_json.remove(req_json.length() - 1);
    req_json += "]}";
    webServer.send(200, "text/json;charset=UTF-8", req_json);
    Serial.print("Found ");
    Serial.print(m);
    Serial.print(" WiFi!  >");
    Serial.print(Signal_filtering);
    Serial.println("dB");
  }
}




void sb() {//////////////////////////////////////////////////////////////////////////
  String parm = webServer.arg("sb");
   if(parm=="sblx"){
    webServer.send(200, "text/plain", "rs");
    }
    if(parm=="rs"){
      ren=0;
      Serial.println("清空成功"); 
    webServer.send(200, "text/plain", "rs ok");
    }
    
}
void people() {
  String parm = webServer.arg("people");
  
   if(parm=="dqrs"){    //当前人数
webServer.send(200, "text/plain", String(ren));
 }
   if(parm=="dtrs"){   //当天人数
webServer.send(200, "text/plain", String(days));
 }
   if(parm=="dyrs"){   //当月人数
webServer.send(200, "text/plain", String(dy));
 }
   if(parm=="xtrs"){   //系统人数
webServer.send(200, "text/plain", String(zs));
 }
  if(parm=="jnrs"){   //今年人数
   int k =0;
 if(year()==2019){
  for(int a =0;a<12;a++) k=k+months19[a];
  }
  if(year()==2020){
  for(int a =0;a<12;a++) k=k+months20[a];
  }
  if(year()==2021){
  for(int a =0;a<12;a++) k=k+months21[a];
  }
  if(year()==2022){
  for(int a =0;a<12;a++) k=k+months22[a];
  }
  if(year()==2023){
  for(int a =0;a<12;a++) k=k+months23[a];
  }
  if(year()==2024){
  for(int a =0;a<12;a++) k=k+months24[a];
  }
  if(year()==2025){
  for(int a =0;a<12;a++) k=k+months25[a];
  }
    
webServer.send(200, "text/plain", String(k));
 }
   if(parm=="qnrs"){   //去年人数
    int y=year()-1;
    int k=0;
    if(y>2019){
   if(y==2019){
  for(int a =0;a<12;a++) k=k+months19[a];
  }
  if(y==2020){
  for(int a =0;a<12;a++) k=k+months20[a];
  }
  if(y==2021){
  for(int a =0;a<12;a++) k=k+months21[a];
  }
  if(y==2022){
  for(int a =0;a<12;a++) k=k+months22[a];
  }
  if(y==2023){
  for(int a =0;a<12;a++) k=k+months23[a];
  }
  if(y==2024){
  for(int a =0;a<12;a++) k=k+months24[a];
  }
  if(y==2025){
  for(int a =0;a<12;a++) k=k+months25[a];
  }
       webServer.send(200, "text/plain", String(k));
      }else
      webServer.send(200, "text/plain", "0");
 }
     
    if(parm=="1"){    //
    if(year()==2019){webServer.send(200, "text/plain", String(months19[0]));}
    if(year()==2020){webServer.send(200, "text/plain", String(months20[0]));}
    if(year()==2021){webServer.send(200, "text/plain", String(months21[0]));}
    if(year()==2022){webServer.send(200, "text/plain", String(months22[0]));}
    if(year()==2023){webServer.send(200, "text/plain", String(months23[0]));}
    if(year()==2024){webServer.send(200, "text/plain", String(months24[0]));}
    if(year()==2025){webServer.send(200, "text/plain", String(months25[0]));}  
 }
  if(parm=="2"){    //
    if(year()==2019){webServer.send(200, "text/plain", String(months19[1]));}
    if(year()==2020){webServer.send(200, "text/plain", String(months20[1]));}
    if(year()==2021){webServer.send(200, "text/plain", String(months21[1]));}
    if(year()==2022){webServer.send(200, "text/plain", String(months22[1]));}
    if(year()==2023){webServer.send(200, "text/plain", String(months23[1]));}
    if(year()==2024){webServer.send(200, "text/plain", String(months24[1]));}
    if(year()==2025){webServer.send(200, "text/plain", String(months25[1]));}  
 }  
  if(parm=="3"){    //
    if(year()==2019){webServer.send(200, "text/plain", String(months19[2]));}
    if(year()==2020){webServer.send(200, "text/plain", String(months20[2]));}
    if(year()==2021){webServer.send(200, "text/plain", String(months21[2]));}
    if(year()==2022){webServer.send(200, "text/plain", String(months22[2]));}
    if(year()==2023){webServer.send(200, "text/plain", String(months23[2]));}
    if(year()==2024){webServer.send(200, "text/plain", String(months24[2]));}
    if(year()==2025){webServer.send(200, "text/plain", String(months25[2]));}  
 }
  if(parm=="4"){    //
    if(year()==2019){webServer.send(200, "text/plain", String(months19[3]));}
    if(year()==2020){webServer.send(200, "text/plain", String(months20[3]));}
    if(year()==2021){webServer.send(200, "text/plain", String(months21[3]));}
    if(year()==2022){webServer.send(200, "text/plain", String(months22[3]));}
    if(year()==2023){webServer.send(200, "text/plain", String(months23[3]));}
    if(year()==2024){webServer.send(200, "text/plain", String(months24[3]));}
    if(year()==2025){webServer.send(200, "text/plain", String(months25[3]));}  
 }
  if(parm=="5"){    //
    if(year()==2019){webServer.send(200, "text/plain", String(months19[4]));}
    if(year()==2020){webServer.send(200, "text/plain", String(months20[4]));}
    if(year()==2021){webServer.send(200, "text/plain", String(months21[4]));}
    if(year()==2022){webServer.send(200, "text/plain", String(months22[4]));}
    if(year()==2023){webServer.send(200, "text/plain", String(months23[4]));}
    if(year()==2024){webServer.send(200, "text/plain", String(months24[4]));}
    if(year()==2025){webServer.send(200, "text/plain", String(months25[4]));}  
 }
  if(parm=="6"){    //
    if(year()==2019){webServer.send(200, "text/plain", String(months19[5]));}
    if(year()==2020){webServer.send(200, "text/plain", String(months20[5]));}
    if(year()==2021){webServer.send(200, "text/plain", String(months21[5]));}
    if(year()==2022){webServer.send(200, "text/plain", String(months22[5]));}
    if(year()==2023){webServer.send(200, "text/plain", String(months23[5]));}
    if(year()==2024){webServer.send(200, "text/plain", String(months24[5]));}
    if(year()==2025){webServer.send(200, "text/plain", String(months25[5]));}  
 }
  if(parm=="7"){    //
    if(year()==2019){webServer.send(200, "text/plain", String(months19[6]));}
    if(year()==2020){webServer.send(200, "text/plain", String(months20[6]));}
    if(year()==2021){webServer.send(200, "text/plain", String(months21[6]));}
    if(year()==2022){webServer.send(200, "text/plain", String(months22[6]));}
    if(year()==2023){webServer.send(200, "text/plain", String(months23[6]));}
    if(year()==2024){webServer.send(200, "text/plain", String(months24[6]));}
    if(year()==2025){webServer.send(200, "text/plain", String(months25[6]));}  
 }
  if(parm=="8"){    //
    if(year()==2019){webServer.send(200, "text/plain", String(months19[7]));}
    if(year()==2020){webServer.send(200, "text/plain", String(months20[7]));}
    if(year()==2021){webServer.send(200, "text/plain", String(months21[7]));}
    if(year()==2022){webServer.send(200, "text/plain", String(months22[7]));}
    if(year()==2023){webServer.send(200, "text/plain", String(months23[7]));}
    if(year()==2024){webServer.send(200, "text/plain", String(months24[7]));}
    if(year()==2025){webServer.send(200, "text/plain", String(months25[7]));}  
 }
  if(parm=="9"){    //
    if(year()==2019){webServer.send(200, "text/plain", String(months19[8]));}
    if(year()==2020){webServer.send(200, "text/plain", String(months20[8]));}
    if(year()==2021){webServer.send(200, "text/plain", String(months21[8]));}
    if(year()==2022){webServer.send(200, "text/plain", String(months22[8]));}
    if(year()==2023){webServer.send(200, "text/plain", String(months23[8]));}
    if(year()==2024){webServer.send(200, "text/plain", String(months24[8]));}
    if(year()==2025){webServer.send(200, "text/plain", String(months25[8]));}  
 }
  if(parm=="10"){    //
    if(year()==2019){webServer.send(200, "text/plain", String(months19[9]));}
    if(year()==2020){webServer.send(200, "text/plain", String(months20[9]));}
    if(year()==2021){webServer.send(200, "text/plain", String(months21[9]));}
    if(year()==2022){webServer.send(200, "text/plain", String(months22[9]));}
    if(year()==2023){webServer.send(200, "text/plain", String(months23[9]));}
    if(year()==2024){webServer.send(200, "text/plain", String(months24[9]));}
    if(year()==2025){webServer.send(200, "text/plain", String(months25[9]));}  
 }
  if(parm=="11"){    //
    if(year()==2019){webServer.send(200, "text/plain", String(months19[10]));}
    if(year()==2020){webServer.send(200, "text/plain", String(months20[10]));}
    if(year()==2021){webServer.send(200, "text/plain", String(months21[10]));}
    if(year()==2022){webServer.send(200, "text/plain", String(months22[10]));}
    if(year()==2023){webServer.send(200, "text/plain", String(months23[10]));}
    if(year()==2024){webServer.send(200, "text/plain", String(months24[10]));}
    if(year()==2025){webServer.send(200, "text/plain", String(months25[10]));}  
 }
  if(parm=="12"){    //
    if(year()==2019){webServer.send(200, "text/plain", String(months19[11]));}
    if(year()==2020){webServer.send(200, "text/plain", String(months20[11]));}
    if(year()==2021){webServer.send(200, "text/plain", String(months21[11]));}
    if(year()==2022){webServer.send(200, "text/plain", String(months22[11]));}
    if(year()==2023){webServer.send(200, "text/plain", String(months23[11]));}
    if(year()==2024){webServer.send(200, "text/plain", String(months24[11]));}
    if(year()==2025){webServer.send(200, "text/plain", String(months25[11]));}  
 } 


   
    
    }





void opera() {
  if(webServer.arg("opera") == "on"){
    
  }else if(webServer.arg("opera") == "off"){
    
  }
  webServer.send(200, "text/plain", "ok");
}

//中断重置
void blink() {
  Serial.println("Keep 3 seconds to reset!");
  bool res_state = true;
  unsigned int res_time = millis();
  //  等待三秒，如果D3还是下拉，则重置，否则取消
  while (millis() - res_time < 3000)
  {
    if (digitalRead(rs) != LOW)
    {
      res_state = false;
      Serial.println("Cancel reset!");
      break;
    }
  }
  if (res_state == true)
  {
    WiFi.disconnect(true);
    delay(100);
    EEPROM.write(WiFi_State_Addr, 0);
    EEPROM.commit();
    delay(300);
    Serial.println("Press the 'rst' button!");
    while (1) {
     
    }
     Serial.println("Reset!");
     ESP.restart();
  }
}
