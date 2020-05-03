
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <WebSocketsServer.h>

#ifndef APSSID
#define APSSID "OBF_AP"
#define APPSK  "vacantgesture"
#endif
const char *APssid = APSSID;
const char *APpassword = APPSK;
int WifiTry =40;

ESP8266WebServer server;
uint8_t pin_led = 2;
String ssid = "WifiAP";
String password = "mypassword";
int t1wait = 0;
int t2wait = 0;
int t3wait = 0;
int t4wait = 0;
int t5wait = 0;
int t6wait = 0;

//Websockets section
String JSONtxt;
WebSocketsServer webSocket=WebSocketsServer(88);
boolean bEventtrig = false;
#define CONFIG_FILENAME  "\wconfig.json"



void setup()
{
  Serial.begin(115200);

  SPIFFS.begin();
  pinMode(pin_led, OUTPUT);

  loadConfig();


  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);


  while((WiFi.status()!=WL_CONNECTED) & (WifiTry>0))
  {
    WifiTry--;
    Serial.print(".");
    delay(500);
  };

  if (WifiTry ==0) {
      WiFi.mode(WIFI_AP);
      WiFi.softAP(APssid, APpassword);
      IPAddress myIP = WiFi.softAPIP();
      Serial.print("AP IP address: ");
      Serial.println(myIP);
  };

  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/",serveIndexFile);

//  server.on("/t1q",t1q);
//  server.on("/t2q",t2q);
//  server.on("/t3q",t3q);
//  server.on("/t4q",t4q);
//  server.on("/t5q",t5q);
//  server.on("/t6q",t6q);
  server.on("/json",whatsmyjsonsay);
  server.on("/save",saveConfig);
  
  server.begin();

//Start Websocket Server
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);



}

void loop()
{
  server.handleClient();
  webSocket.loop();
    
  JSONtxt="{\"ssid\":\""+ssid+"\","+              // JSON requires double quotes
             "\"password\":\""+password+"\","+
             "\"t1wait\":\""+(String)t1wait+"\","+
             "\"t2wait\":\""+(String)t2wait+"\","+
             "\"t3wait\":\""+(String)t3wait+"\","+
             "\"t4wait\":\""+(String)t4wait+"\","+
             "\"t5wait\":\""+(String)t5wait+"\","+
             "\"t6wait\":\""+(String)t6wait+"\","+
             "\"t6wait\":\""+String(t6wait)+"\"}";
   // if there has been a change send back my latest info          
   if (bEventtrig)  {webSocket.broadcastTXT(JSONtxt);bEventtrig=false; Serial.println("sending via websocket");};
   if (bEventtrig)  {Serial.println(JSONtxt);};
   if (bEventtrig)  {Serial.println("bEventtrig = true");};

}

void serveIndexFile()
{
  File file = SPIFFS.open("/index.html","r");
  server.streamFile(file, "text/html");
  file.close();
  bEventtrig = true;
}

void whatsmyjsonsay()
{
  File file = SPIFFS.open("/wconfig.json","r");
  server.streamFile(file, "text/html");
  file.close();
}



void t1q()
{
  server.send(200,"text/plain", String(t1wait));
  Serial.println("t1wait set to "+ String(t1wait));
}

void t2q()
{
  server.send(200,"text/plain", String(t2wait));
  Serial.println("t2wait set to "+ String(t2wait));
}

void t3q()
{
  server.send(200,"text/plain", String(t3wait));
  Serial.println("t3wait set to "+ String(t3wait));
}

void t4q()
{
  server.send(200,"text/plain", String(t4wait));
  Serial.println("t4wait set to "+ String(t4wait));
}

void t5q()
{
  server.send(200,"text/plain", String(t5wait));
  Serial.println("t5wait set to "+ String(t5wait));
}

void t6q()
{
  server.send(200,"text/plain", String(t6wait));
  Serial.println("t6wait set to "+ String(t6wait));
}

bool loadConfig() {
  File configFile = SPIFFS.open("/wconfig.json", "r");
  if (!configFile) {
    Serial.println("Failed to open config file");
    return false;
  }

  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Config file size is too large");
    return false;
  }

  // Allocate a buffer to store contents of the file.
  std::unique_ptr<char[]> buf(new char[size]);

  // We don't use String here because ArduinoJson library requires the input
  // buffer to be mutable. If you don't use ArduinoJson, you may as well
  // use configFile.readString instead.
  configFile.readBytes(buf.get(), size);

  StaticJsonDocument<200> doc;
  auto error = deserializeJson(doc, buf.get());
  if (error) {
    Serial.println("Failed to parse config file");
    return false;
  }

  const char* CfgSsid = doc["CfgSsid"];
  const char* CfgPassword = doc["CfgPassword"];
  const int CfgT1 = doc["CfgT1"];
  const int CfgT2 = doc["CfgT2"];
  const int CfgT3 = doc["CfgT3"];
  const int CfgT4 = doc["CfgT4"];
  const int CfgT5 = doc["CfgT5"];
  const int CfgT6 = doc["CfgT6"];

  Serial.println();
  Serial.print("Loaded AP name: ");
  Serial.println(CfgSsid);
  Serial.print("Loaded Password: ");
  Serial.println(CfgPassword);
  Serial.print("Loaded Timer1: ");
  Serial.println(CfgT1);
  Serial.print("Loaded Timer2: ");
  Serial.println(CfgT2);
  Serial.print("Loaded Timer3: ");
  Serial.println(CfgT3);
  Serial.print("Loaded Timer4: ");
  Serial.println(CfgT4);
  Serial.print("Loaded Timer5: ");
  Serial.println(CfgT5);
  Serial.print("Loaded Timer6: ");
  Serial.println(CfgT6);
  ssid = String(CfgSsid);
  password = String(CfgPassword);
  t1wait = CfgT1;
  t2wait = CfgT2;
  t3wait = CfgT3;
  t4wait = CfgT4;
  t5wait = CfgT5;
  t6wait = CfgT6;
  return true;
}

bool saveConfig() {
  StaticJsonDocument<200> doc;

  
  
  doc["CfgSsid"] = ssid;
  doc["CfgPassword"] = password;
  doc["CfgT1"] = t1wait;
  doc["CfgT2"] = t2wait;
  doc["CfgT3"] = t3wait;
  doc["CfgT4"] = t4wait;
  doc["CfgT5"] = t5wait;
  doc["CfgT6"] = t6wait;

  Serial.print("Saved AP name: ");
  Serial.println(ssid);
  Serial.print("Saved Password: ");
  Serial.println(password);
  Serial.print("Saved Timer1: ");
  Serial.println(t1wait);
  Serial.print("Saved Timer2: ");
  Serial.println(t2wait);
  Serial.print("Saved Timer3: ");
  Serial.println(t3wait);
  Serial.print("Saved Timer4: ");
  Serial.println(t4wait);
  Serial.print("Saved Timer5: ");
  Serial.println(t5wait);
  Serial.print("Saved Timer6: ");
  Serial.println(t6wait);

  File configFile = SPIFFS.open("/wconfig.json", "w");
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return false;
  }

  serializeJson(doc, configFile);
  server.send(200,"text/plain", "Success");
  Serial.println("Saved config file");
  return true;
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t wslength)
{
  String payloadString=(const char *)payload;
  Serial.println("payload: '"+payloadString+"', channel: "+(String)num);
  if(type==WStype_TEXT){
    byte separator=payloadString.indexOf('=');
    String var=payloadString.substring(0,separator);
    String val=payloadString.substring(separator+1);
    if(var=="ssid"){
      ssid = val;
    }else if(var=="password"){
      password = val;
    }else if(var=="t1wait"){
      t1wait = val.toInt();
    }else if(var=="t2wait"){
      t2wait = val.toInt();
    }else if(var=="t3wait"){
      t3wait = val.toInt();
    }else if(var=="t4wait"){
      t4wait = val.toInt();
    }else if(var=="t5wait"){
      t5wait = val.toInt();
    }else if(var=="t6wait"){
      t6wait = val.toInt();
    }else if(var=="t6wait"){
      t6wait = val.toInt();
    }
    bEventtrig = true;
  }
}
