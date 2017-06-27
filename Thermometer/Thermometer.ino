#define USE_STATIC_IP 	1
#define USE_OTA 	0

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <stdlib.h>
#include <time.h>

#include <Adafruit_GFX.h>				// https://github.com/adafruit/Adafruit-GFX-Library
#include <Fontx.h>							// https://github.com/h-nari/Fontx
#include <Humblesoft_GFX.h>			// https://github.com/h-nari/Humblesoft_GFX
#include <Humblesoft_ILI9341.h>	// https://github.com/h-nari/Humblesoft_ILI9341

#include <SD.h>
#include "sdHandler.h"
#include "LcdRoot.h"
#include "LcdMenu.h"
#include "LcdCalibWin.h"
#include <XPT2046_Touchscreen.h>
#include <LcdTouchscreen.h>
#include <Params.h>
#include <EEPROM.h>
#include "AppTopWin.h"
#include "AppDB.h"
#include "AppGraph.h"
#include "AppCalibWin.h"
#include "AppShutdownWin.h"
#include "Task.h"

#if USE_OTA
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#endif

#include "wifi-conf.h"

#if USE_STATIC_IP
IPAddress staticIP(192,168,0,203);
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0);
#endif

#define TFT_CS	 2
#define TFT_DC  15
#define TFT_RST -1
#define SD_CS	 0
#define TP_CS   16

ESP8266WebServer server(80);
Humblesoft_ILI9341 tft = Humblesoft_ILI9341(TFT_CS,TFT_DC,TFT_RST);

LcdTouchscreen ts(TP_CS);
LcdRoot root(&tft, &ts);
AppTopWin topWin;
AppDB db;
AppCalibWin calibWin;
AppShutdownWin shutdownWin;
LcdMenu topMenu;
TaskQueue queue;

ParamEntry paramTable[] = {
  { PARAM_CALIBRATION, ""},
};
static uint16_t c1 = Humblesoft_ILI9341::rgb("GREEN");
static uint16_t c2 = Humblesoft_ILI9341::rgb("YELLOW");
static uint16_t bg = Humblesoft_ILI9341::rgb("BLACK");

#if USE_OTA
static int8_t ota_col;
#endif

Params eeparam( paramTable, sizeof(paramTable)/sizeof(paramTable[0]));

void handleRoot() {
  server.send(200, "text/plain", "hello from esp8266/Thermometer!");
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void makeTopMenu(LcdMenu *pMenu)
{
  static LcdMenuEntry me1("Calibration", &calibWin);
  static LcdMenuEntry me2("Shutdown", &shutdownWin);
  static LcdMenuEntry me3("Graph");
  pMenu->add(&me1);
  pMenu->add(&me2);
  pMenu->add(&me3);
}

void handlePut(void)
{
  server.send(200, "text/plain", "OK");
  
  String sid = server.arg("sid");
  String value = server.arg("v");
  String time  = server.arg("time");

  db.dataPut(sid.c_str(), time.c_str(), value.c_str());
}

void setup(void){
  bool r;
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
#if USE_STATIC_IP
  WiFi.config(staticIP, gateway, subnet);
#endif

  Serial.println("\n\nReset:");
  
  // initialize SPI bus

  pinMode(TFT_CS,OUTPUT);
  digitalWrite(TFT_CS, HIGH);
  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);
  pinMode(TP_CS, OUTPUT);
  digitalWrite(TP_CS, HIGH);
  
  // SDcard must be initilized first in SPI devices
  
  if(SD.begin(SD_CS)) {
    Serial.printf("initialize SD Card done.\n");
    db.setSDValid();
  } else
    Serial.printf("initialise SD Card failed.\n");

  tft.begin();
  tft.setRotation(0);
  tft.fillScreen("BLACK");
  tft.setTextWrap(false);
  
  // touchscreen calibration 

  ts.begin();
  eeparam.load();
  if(ts.touched() || !ts.loadCalibrationData(&eeparam)){
    do {
      r = ts.calibrate(&tft, c1, bg);
    
      tft.fillScreen(bg);
      tft.setTextColor(c2);
      tft.setCursor(0,0);
      tft.printf("calibration %s.", r ? "succeeded" : "failed");
      Serial.printf("calibration %s.", r ? "succeeded" : "failed");
      delay(1000);
    } while(!r);
    ts.saveCalibrationData(&eeparam);
    if(eeparam.changed())
      eeparam.save();
  }

  // Wait for connection

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  tft.tfa.print("IP: "); 
  tft.tfa.println(WiFi.localIP());
  
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }
  configTime( 9 * 3600, 0,
	      "ntp.nict.jp", "ntp.jst.mfeed.ad.jp", NULL);
  
  server.on("/", handleRoot);
  server.on("/put", handlePut);
  server.onNotFound(handleNotFound);
  server.addHandler(new SdHandler("/","/sd"));
  server.begin();
  Serial.println("HTTP server started");

#if USE_OTA
#ifdef OTA_PORT
  ArduinoOTA.setPort(OTA_PORT);
#endif
  ArduinoOTA.setHostname(OTA_HOSTNAME);
  ArduinoOTA.setPassword(OTA_PASSWORD);
  ArduinoOTA.onStart([](){
      Serial.println("start");
      ota_col = 0;
    });
  ArduinoOTA.onEnd([](){
      Serial.println("\nEnd");
    });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total){
      // Serial.printf("Progress: %u%%\n", (progress / (total / 100)));
      Serial.print('.');
      if(++ota_col >= 50){
	Serial.println();
	ota_col = 0;
      }
    });
  ArduinoOTA.onError([](ota_error_t error){
      Serial.printf("Error[%u]: ", error);
      if(error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if(error == OTA_BEGIN_ERROR) Serial.println("Connect Failed");
      else if(error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if(error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if(error == OTA_END_ERROR) Serial.println("End Failed");
    });
  ArduinoOTA.begin();
#endif

  topWin.setDB(&db);

  makeTopMenu(&topMenu);
  topWin.setMenu(&topMenu);
  root.pushWin(&topWin);
}

void loop(void){
#if USE_OTA
  ArduinoOTA.handle();
#endif

  server.handleClient();
  
  root.update();
  queue.update();
}

/*** Local variables: ***/
/*** tab-width:2 ***/
/*** End: ***/
