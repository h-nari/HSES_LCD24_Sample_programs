#include <Arduino.h>
#include "LcdMenu.h"
#include <LcdTouchscreen.h>

LcdRoot::LcdRoot(LcdDrv *lcd, LcdTouchscreen *tp)
{
  m_pLcd = lcd;
  m_pTp  = tp;
  m_lWin = NULL;
  m_bPressed0 = false;
  m_bg = ILI9341_BLACK;
}

void LcdRoot::begin(void)
{
#if 0
  Serial.begin(115200);
  delay(100);
  Serial.println("\n\nReset:");
  
  m_pLcd->begin();
  m_pLcd->setRotation(3);
  m_pLcd->fillScreen(ILI9341_BLACK);
  m_pLcd->setTextSize(2);
  m_pLcd->println("MenuTest.");
  m_pTp->begin();
#endif
  
  if(m_lWin) m_lWin->activated();
}

void LcdRoot::update(void)
{
  int16_t x, y;
  bool bPressed;

  bPressed =  m_pTp->getPoint(&x, &y);

  if(bPressed){
    if(!m_bPressed0){
      if(m_lWin)
	m_lWin->tp_pressed(x, y);
    }
    else if(x != m_x0 || y != m_y0){
      if(m_lWin)
	m_lWin->tp_drag(x,y);
    }
  }
  else {
    if(m_bPressed0){
      if(m_lWin)
	m_lWin->tp_released(m_x0,m_y0);
    }
  }

  m_bPressed0 = bPressed;
  m_x0 = x;
  m_y0 = y;

#if 1
  m_lWin->update();
#else
  for(LcdWin *p = m_lWin; p; p=p->m_pNext)
    p->update();
#endif
}

void LcdRoot::bindWin(LcdWin *pWin)
{
  pWin->m_root = this;
  pWin->m_pLcd = m_pLcd;
}

void LcdRoot::pushWin(LcdWin *pWin)
{
  pWin->m_root = this;
  pWin->m_pNext = m_lWin;
  pWin->m_pLcd = m_pLcd;
  if(m_lWin)
    m_lWin->deactivated();
  m_lWin = pWin;

  pWin->activated();
}

void LcdRoot::popWin(LcdWin *pWin)
{
  if(m_lWin == pWin || !pWin){
    if(!pWin) pWin = m_lWin;
    m_lWin = pWin->m_pNext;
    pWin->deactivated();
    if(m_lWin)
      m_lWin->reactivated();
#if 0
    else
      redisplay();
#endif
  }
  else 
    Serial.printf("popWin Error, bad win\n");
}

void LcdRoot::popWinAll()
{
  while(m_lWin){
    m_lWin->deactivated();
    m_lWin = m_lWin->m_pNext;
  }
}

void LcdRoot::redisplay(void)
{
  m_pLcd->fillScreen(m_bg);
}


