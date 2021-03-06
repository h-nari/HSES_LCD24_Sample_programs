#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <time.h>

#include <Adafruit_GFX.h>		// https://github.com/adafruit/Adafruit-GFX-Library
#include <Fontx.h>							// https://github.com/h-nari/Fontx
#include <Humblesoft_GFX.h>			// https://github.com/h-nari/Humblesoft_GFX
#include <Humblesoft_ILI9341.h>	// https://github.com/h-nari/Humblesoft_ILI9341

#include "rssParser.h"
#include "rssBuf.h"

#include "conf.h"

#define KANJI 1

const char *site[] = {
  "http://numbers2007.blog123.fc2.com/?xml",
  "http://feeds.feedburner.com/make_jp",
  "http://www.jiji.com/rss/ranking.rdf",
  "http://rss.rssad.jp/rss/impresswatch/pcwatch.rdf",
  "http://headlines.yahoo.co.jp/rss/natiogeog-c_int.xml",
  "http://headlines.yahoo.co.jp/rss/jct-bus.xml",
  "http://rss.asahi.com/rss/asahi/newsheadlines.rdf",
  "http://headlines.yahoo.co.jp/rss/it_eetimes-bus.xml",
  "http://headlines.yahoo.co.jp/rss/it_monoist-bus.xml",
  NULL};

#if KANJI
#include <fontx/ILGH16XB.h>
#include <fontx/ILGZ16XB.h>
#endif

enum AppState {
  AS_INIT,
  AS_READ,
  AS_DISP,
};

AppState gState;
int httpCode;

uint16_t titleFg,titleBg,subTitleFg,subTitleBg;
uint16_t itemFg,itemBg,timeFg,timeBg;
const uint16_t hTitle = 36;
const uint16_t hSubTitle = 22;

RssBuf rssBuf;
const char **pSite = site;
Humblesoft_ILI9341 tft = Humblesoft_ILI9341();
RomFontx fontx(ILGH16XB,ILGZ16XB);
RssParser rss;

void setup()
{
  Serial.begin(115200);
  delay(100);
  Serial.println("\n\nReset:");

  tft.begin();

	titleFg    = tft.rgb("YELLOW");
	titleBg    = tft.rgb("RED");
	subTitleFg = tft.rgb("BLACK");
	subTitleBg = tft.rgb(173,255,47); // greenyellow
	itemFg     = tft.rgb("WHITE");
	itemBg     = tft.rgb("BLACK");
	timeFg     = tft.rgb("WHITE");
	timeBg     = tft.rgb("BLUE");

	
  tft.setRotation(0);
  tft.setVerticalScrollArea(hTitle + hSubTitle,21);
  int w = tft.width();
  tft.tfa.fillRect(0,0, w, hTitle, titleBg);
  tft.tfa.fillRect(0,hTitle,w, hSubTitle, subTitleBg);
  tft.vsa.fillScreen(itemBg);
  tft.vsa.setTextColor(itemFg);
  tft.setScrollBg(itemBg);
  tft.bfa.fillScreen(timeBg);

  tft.tfa.setTextWrap(false);
  tft.bfa.setTextWrap(false);
  
#if KANJI
  tft.tfa.setFont(&fontx);
  tft.vsa.setFont(&fontx);
  tft.bfa.setFont(&fontx);
#endif
  gState = AS_INIT;
}

void draw_time()
{
  static time_t t0;
  time_t t = time(NULL);

  if(t != t0){
    struct tm *tm = localtime(&t);
    const char *wd[7] = {"日","月","火","水","木","金","土"};
    tft.bfa.setTextColor(timeFg,timeBg);
    tft.bfa.setTextSize(1);
    tft.bfa.setCursor(4,3);
    tft.bfa.printf("%04d年%02d月%02d(%s) %02d:%02d:%02d",
		   tm->tm_year + 1900, tm->tm_mon+1, tm->tm_mday,
		   wd[tm->tm_wday],
		   tm->tm_hour, tm->tm_min, tm->tm_sec);
    
    t0 = t;
  }
}

void loop()
{
  Serial.printf("gState:%d\n", gState);
  if(gState == AS_INIT){
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while(WiFi.status() != WL_CONNECTED) {
      Serial.print('.');
      delay(500);
    }
    Serial.println();
    Serial.printf("Connected, IP address: ");
    Serial.println(WiFi.localIP());

    configTime( 9 * 3600, 0,
		"ntp.nict.jp", "ntp.jst.mfeed.ad.jp", NULL);
    
    gState = AS_READ;
  }
  else if(gState == AS_READ){
    rssBuf.clear();
    
    HTTPClient http;
    Serial.printf("[HTTP]begin..\n");
    
    http.begin(*pSite);
    Serial.printf("[HTTP]GET %s\n",*pSite);

    httpCode = http.GET();
    if(httpCode > 0){
      Serial.printf("[HTTP] GET ... code: %d\n",httpCode);
      if(httpCode == HTTP_CODE_OK) {
	int len = http.getSize();
	uint8_t buff[128] = {0};
	WiFiClient *stream = http.getStreamPtr();
	while(http.connected() && (len > 0 || len == -1)){
	  size_t size = stream->available();
	  if(size){
	    if(size > sizeof buff) size = sizeof buff;
	    int c = stream->readBytes(buff,size);
	    for(int i=0; i<c; i++)
	      rss.put(buff[i]);
	    if(len > 0) len -= c;
	  }
	  delay(1);
	}
	Serial.print("[HTTP] connection closed or file end.\n");
      }
      gState = AS_DISP;
    } else {
      Serial.printf("[HTTP] GET ... failed, error: %s\n",
		    http.errorToString(httpCode).c_str());
      if(*++pSite == NULL) pSite = site;
      gState = AS_READ;
    }
    http.end();
    
  }
  else if(gState == AS_DISP) {
    int w = tft.width();
    tft.tfa.fillRect(0, 0, w, hTitle,titleBg);
    tft.bfa.fillRect(0, hTitle, w, hSubTitle, subTitleBg);
    tft.tfa.setCursor(5,2);
    tft.tfa.setTextSize(2);
    tft.tfa.setTextColor(titleFg);
    tft.tfa.print(rssBuf.getTitle());

    int itemCount = rssBuf.getItemCount();
    Serial.printf("ItemCount:%d\n", itemCount);

    if(itemCount > 0){
      for(int i=0; i< itemCount; i++){
	tft.vsa.println();
	tft.vsa.println();
	tft.vsa.print(rssBuf.getItem(i));
	tft.tfa.setTextSize(1);
	tft.tfa.setCursor(2,hTitle+3);
	tft.tfa.setTextColor(subTitleFg,subTitleBg);
	tft.tfa.printf("[%2d/%2d] %s",i+1,rssBuf.getItemCount(), *pSite);
	for(int j=0; j < 50; j++){
	  draw_time();
	  delay(100);
	}
      }
    } else {
      tft.vsa.printf("No Item\n%s", *pSite);
    }
      
    tft.vsa.setTextColor("YELLOW");
    tft.vsa.print("\n\n--------------------------");
    tft.vsa.setTextColor("WHITE");
    if(*++pSite == NULL) pSite = site;
    gState = AS_READ;
  }
}

/*** Local variables: ***/
/*** tab-width:2 ***/
/*** End: ***/
