/*
 * This sketch is customized ESP8266WiFi/WiFiScan for HSES-LCD24
 */

#define USE_KANJI 1

#include <Adafruit_GFX.h>		// https://github.com/adafruit/Adafruit-GFX-Library
#include <Adafruit_ILI9341.h>		// https://github.com/adafruit/Adafruit_ILI9341
#include <Fontx.h>							// https://github.com/h-nari/Fontx
#include <Humblesoft_GFX.h>			// https://github.com/h-nari/Humblesoft_GFX
#include <Humblesoft_ILI9341.h>	// https://github.com/h-nari/Humblesoft_ILI9341
#include <ESP8266WiFi.h>

#define WiFiMax	10
#define Y1	32
#define LH	24
#define X_SIG 	260
#define X_STAT	240
#define Y_STAT0  2
#define Y_STAT  18

#if USE_KANJI
#include <fontx/ILGH16XB.h>
#include <fontx/ILGZ16XB.h>
RomFontx fontx(ILGH16XB,ILGZ16XB);
#endif

#define CS 	 2
#define DC 	15
#define RST	-1
Humblesoft_ILI9341 tft = Humblesoft_ILI9341(CS,DC,RST);
bool serial_out = false;

uint16_t bg    = ILI9341_BLACK;
uint16_t bg2   = ILI9341_DARKGREY;
uint16_t white = ILI9341_WHITE;
uint16_t black = ILI9341_BLACK;
uint16_t red   = ILI9341_RED;
uint16_t green = ILI9341_GREEN;
uint16_t blue  = ILI9341_BLUE;
uint16_t yellow = ILI9341_YELLOW;
uint16_t cyan  = ILI9341_CYAN;

void setup() {
  Serial.begin(115200);
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(bg);
  
  tft.fillRect(0,0, tft.width(), Y1, bg2);
  tft.setTextSize(3);
  tft.setTextColor(black);
  tft.setCursor(5,5);
  tft.print("WiFiScan");
  tft.setTextColor(yellow);
  tft.setCursor(3,3);
  tft.print("WiFiScan");
		   
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.println("\n:Reset:Setup done");
}

void loop() {
  tft.setCursor(X_STAT, Y_STAT0);
  tft.setTextSize(1);
  tft.setTextColor(white, bg2);
  tft.print("Scan START");
  
  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  tft.setCursor(X_STAT, Y_STAT0);
  tft.print("SCAN DONE ");

  if (n == 0 && serial_out)
    Serial.println("no networks found");

  tft.setCursor(X_STAT, Y_STAT);
  tft.setTextSize(1);
  tft.setTextColor(red, bg2);
  tft.printf("%d Found.   ", n);
  
  uint16_t sort_tbl[WiFiMax];
  uint32_t rssi[WiFiMax];
  int i,j=0;
  for(i=0; i < n && j < WiFiMax;i++){
    if(!WiFi.isHidden(i)){
      rssi[j] = WiFi.RSSI(i);
      sort_tbl[j++] = i;
    }
  }

  n = j;
  for(i=0; i<n-1; i++){
    for(j=n-1; j>i; j--){
      int j0 = sort_tbl[j-1];
      int j1 = sort_tbl[j];
      if(rssi[j0] < rssi[j1]){
	sort_tbl[j-1] = j1;
	sort_tbl[j]   = j0;
      }
    }
  }

  int16_t y = Y1 + 10;

  i = 0;
  while(y + LH < tft.height() && i < n){
    j = sort_tbl[i++];

    if(serial_out)
      Serial.printf("%s [%ddBm]\n", WiFi.SSID(j).c_str(), WiFi.RSSI(j));
    
    tft.setCursor(0, y);
#if USE_KANJI
		tft.setFont(&fontx);
    tft.setTextSize(1);
#else
    tft.setTextSize(2);
#endif

    tft.setTextColor(green, bg);
    tft.print(WiFi.SSID(j));

    int16_t x = tft.getCursorX();
    if(x < X_SIG)
      tft.fillRect(x, y, X_SIG - x, LH, bg);

    tft.setCursor(X_SIG, y);
    tft.setTextColor(cyan,bg);
    tft.printf("%3d", WiFi.RSSI(j));
#if USE_KANJI
    tft.setFont();
#endif
    tft.setTextSize(1);
    tft.setCursor(tft.getCursorX()+1, y + 8);
    tft.setTextColor(blue,bg);
    tft.print("dBm");

    y += LH;
  }
  if(serial_out) Serial.println();
  tft.fillRect(0, y, tft.width(), tft.height() - y, bg);

  
  // Wait a bit before scanning again
  delay(5000);
}

/*** Local variables: ***/
/*** tab-width:2 ***/
/*** End: ***/
