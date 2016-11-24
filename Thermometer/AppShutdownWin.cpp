#include <Arduino.h>
#include <EEPROM.h>
#include "AppShutdownWin.h"
#include "AppDB.h"

extern AppDB db;

AppShutdownWin::AppShutdownWin() : LcdWin(), m_btnCancel("Cancel",1)
{
  addButton(&m_btnCancel);
}

void AppShutdownWin::activated()
{
  db.blockDataSave();
  redisplay();
}

void AppShutdownWin::redisplay()
{
  if(!m_root) return;
  LcdDrv *pLcd = m_root->getLcd();

  uint16_t sw = pLcd->width();
  uint16_t sh = pLcd->height();
  int16_t mx = sw/2;
  int16_t y;
  int16_t my0 = sh/3 - 30;
  int16_t my1 = sh*2/3 + 20;

  pLcd->fillScreen(m_bg);
  adjustButtonSize();

  pLcd->alignPrintf(mx, my0, TA_CENTER, TA_TOP,
		    "stop data write\n\n");
  y = pLcd->getCursorY() + 5;
  pLcd->alignPrintf(mx, y, TA_CENTER, TA_TOP,
		    "you can reset or\n");
  y = pLcd->getCursorY() + 5;
  pLcd->alignPrintf(mx, y, TA_CENTER, TA_TOP,
		    "turn off safely.\n");

  uint16_t bw,bh;
  
  m_btnCancel.getSize(&bw, &bh);
  m_btnCancel.setPos((sw-bw)/2, my1 - bh/2);

  resetButtonSelected();
  drawButton();
}

void AppShutdownWin::tp_pressed(uint16_t x, uint16_t y)
{
  resetButtonSelected();
  setButtonSelected(x, y);
}

void AppShutdownWin::tp_drag(uint16_t x, uint16_t y)
{
  setButtonSelected(x, y);
  drawChangedButton();
}

void AppShutdownWin::tp_released(uint16_t x, uint16_t y)
{
  int v;
  
  setButtonSelected(x, y);
  drawChangedButton();

  if(getButtonValue(&v) && v == 1){
    db.blockDataSave(false);
    m_root->popWin(this);
  }
}

  
