#include "LcdCalibWin.h"

LcdCalibWin::LcdCalibWin()
{
  m_state = 0;
}

void LcdCalibWin::activated()
{
  m_state = 0;
  redisplay();
}

void LcdCalibWin::tp_released(uint16_t x, uint16_t y)
{
  if(++m_state < 4)
    redisplay();
  else 
    m_mesBox.yesNo(m_root,"Calibration succedded.\nSave result?");
}

void LcdCalibWin::reactivated()
{
  if(m_state == 4){
    if(m_mesBox.isYes()) {
      m_state = 5;
      m_mesBox.ok(m_root,"Calibration data saved.");
    }
    else 
      m_root->popWin(this);
  }
  else 
    m_root->popWin(this);
}
    

void LcdCalibWin::redisplay()
{
  m_pLcd->fillScreen(m_bg);
  m_pLcd->setTextSize(3);
  m_pLcd->setCursor(0,20);
  m_pLcd->setTextColor(m_fg);
  m_pLcd->printf("Calibration\nstate:%d\n",m_state);
  if(m_state >= 4)
    m_pLcd->print("\ndone.\n");
}


