#include "LcdMenu.h"

void LcdButton::init()
{
  m_ox = 10;
  m_oy = 10;
  m_selected = m_selected0 = false;
  m_pNext = NULL;
}

LcdButton::LcdButton()
{
  init();
}

LcdButton::LcdButton(const char *text, int value)
{
  init();
  m_text = text;
  m_value = value;
}


void LcdButton::setTextAndValue(const char *text, int value)
{
  m_text = text;
  m_value = value;
}

void LcdButton::adjustSize(LcdDrv *pLcd)
{
  if(!pLcd)
    return;
  if(m_text){
    int16_t x, y;
    uint16_t w, h;
    pLcd->getTextBounds((char *)m_text, 0, 0, &x, &y, &w, &h);
    m_w = w + m_ox * 2;
    m_h = h + m_oy * 2;
  }
  else {
    m_w = 16;
    m_h = 16;
  }
}

void LcdButton::draw(LcdDrv *pLcd, uint16_t cFg, uint16_t cBg, uint16_t cText)
{
  pLcd->fillRect(m_x, m_y, m_w, m_h, !m_selected ? cBg : cFg);
  pLcd->drawRect(m_x, m_y, m_w, m_h, !m_selected ? cFg : cBg);
  if(m_text){
    pLcd->setCursor(m_x + m_ox, m_y+m_oy);
    pLcd->setTextColor(!m_selected ? cText : cBg);
    pLcd->print(m_text);
  }
}

void LcdButton::drawChanged(LcdDrv *pLcd, uint16_t cFg, uint16_t cBg,
			    uint16_t cText)
{
  if(m_selected != m_selected0){
      draw(pLcd, cFg, cBg, cText);
      m_selected0 = m_selected;
  }
}

void LcdButton::setSelected(uint16_t x, uint16_t y)
{
  m_selected = 
    (x >= m_x && x < m_x + m_w && y >= m_y && y < m_y + m_h);
}

void LcdButton::setPos(int16_t x, int16_t y)
{
  m_x = x;
  m_y = y;
}

void LcdButton::getSize(uint16_t *pw, uint16_t *ph)
{
  if(pw) *pw = m_w;
  if(ph) *ph = m_h;
}
