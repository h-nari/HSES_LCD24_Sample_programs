#include <Arduino.h>
#include <EEPROM.h>
#include "AppCalibWin.h"

extern LcdTouchscreen ts;
extern Params eeparam;

AppCalibWin::AppCalibWin() : LcdWin()
{
}

void AppCalibWin::activated()
{
  bool r;

  if(!m_root) return;
  LcdDrv *m_pLcd = m_root->getLcd();
  
  r = ts.calibrate(m_pLcd, m_fg, m_bg);
    
  m_pLcd->fillScreen(m_bg);
  m_pLcd->setTextColor(m_fg);
  m_pLcd->setTextSize(2);
  const char *rs = r ? "succeeded" : "failed";

  m_pLcd->alignPrintf(m_pLcd->width()/2,m_pLcd->height()/2 - 5,
		      TA_CENTER,TA_BOTTOM,
		      "calibration");
  
  m_pLcd->alignPrintf(m_pLcd->width()/2,m_pLcd->height()/2 + 5,
		      TA_CENTER,TA_TOP, "%s",rs);
  Serial.printf("calibration %s.\n", rs);
  delay(1000);
  if(r){
    ts.saveCalibrationData(&eeparam);
    if(eeparam.changed())
      eeparam.save();
  }
  m_root->popWin(this);
}
