#include "LcdMenu.h"

LcdWin::LcdWin()
{
  m_x = m_y = 0;
  m_w = 320;
  m_h = 240;

  m_fg = Humblesoft_ILI9341::rgb("WHITE");
  m_bg = Humblesoft_ILI9341::rgb("BLACK");

  m_pNext = NULL;
  m_lButton = NULL;

  m_textSize = 2;

  m_pMenu = NULL;
}

bool LcdWin::isHit(uint16_t x, uint16_t y)
{
  return x >= m_x && x < m_x + m_w && y >= m_y && y < m_y + m_h;
}

void LcdWin::addButton(LcdButton *btn)
{
  LcdButton *p,*p0=NULL;
  
  for(p=m_lButton; p; p0=p, p=p->m_pNext);
  if(p0) p0->m_pNext = btn;
  else    m_lButton = btn;
  btn->m_pNext = NULL;
}

void LcdWin::removeButton()
{
  m_lButton = NULL;
}

void LcdWin::drawButton()
{
  m_pLcd->setTextSize(m_textSize);
  for(LcdButton *p = m_lButton; p; p=p->m_pNext)
    p->draw(m_pLcd, m_fg, m_bg, m_fg);
}


void LcdWin::drawChangedButton()
{
  m_pLcd->setTextSize(m_textSize);
  for(LcdButton *p = m_lButton; p; p=p->m_pNext)
    p->drawChanged(m_pLcd, m_fg, m_bg, m_fg);
}

void LcdWin::resetButtonSelected()
{
  for(LcdButton *p = m_lButton; p; p=p->m_pNext)
    p->m_selected = false;
}

void LcdWin::setButtonSelected(uint16_t x, uint16_t y)
{
  for(LcdButton *p = m_lButton; p; p=p->m_pNext)
    p->setSelected(x,y);
}

void LcdWin::adjustButtonSize()
{
  m_pLcd->setTextSize(m_textSize);
  for(LcdButton *p = m_lButton;p; p=p->m_pNext){
    p->adjustSize(m_pLcd);
  }
}

void LcdWin::setButtonPosition()
{
  int w = 0,n=0,s,x,y;
  LcdButton *p;

  if(!m_lButton) return;
  for(p = m_lButton; p; p=p->m_pNext){
    w += p->m_w;
    n++;
  }
  s = (m_w - w)/(n+1);
  x = m_x + s;
  y = m_y + m_h - m_lButton->m_h * 3 / 2;
  for(p = m_lButton; p; p=p->m_pNext){
    p->m_x = x;
    p->m_y = y;
    x += p->m_w + s;
  }
}

bool LcdWin::getButtonValue(int *pVal)
{
  for(LcdButton *p = m_lButton; p; p=p->m_pNext){
    if(p->m_selected) {
      if(pVal) *pVal = p->m_value;
      return true;
    }
  }
  return false;
}

void LcdWin::tp_released(uint16_t x, uint16_t y)
{
  if(m_pMenu && m_root){
    m_pMenu->popup(m_root, x, y);
  }
}
