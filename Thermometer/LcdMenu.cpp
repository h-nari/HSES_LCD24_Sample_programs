#include "LcdMenu.h"

LcdMenu::LcdMenu() : LcdWin()
{
  m_lEntry = NULL;
  m_fg        = Humblesoft_ILI9341::rgb("DARKGREEN");
  m_bg	      = Humblesoft_ILI9341::rgb("WHITE");
  m_textColor = Humblesoft_ILI9341::rgb("BLUE");
  m_x = 5;
  m_y = 5;
  m_w = 0;
  m_rowHeight = 0;
  m_vSize = false;
  m_textSize = 3;
  m_margin = 10;
}

void LcdMenu::add(LcdMenuEntry *pEntry)
{
  LcdMenuEntry *p=m_lEntry,*p0=NULL;

  while(p) p0=p, p=p->m_pNext;
  if(p0) p0->m_pNext = pEntry;
  else   m_lEntry = pEntry;
  m_vSize = false;
}

void LcdMenu::popup(LcdRoot *rootWin, uint16_t x, uint16_t y)
{
  rootWin->pushWin(this);
}

void LcdMenu::adjustSize()
{
  uint16_t min_x=0,max_x=0,min_y=0,max_y=0;
  int n = 0,ww,hh;
  
  m_pLcd->setTextSize(m_textSize);
  for(LcdMenuEntry *p=m_lEntry;p;p=p->m_pNext){
    int16_t x, y;
    uint16_t w, h;
    m_pLcd->getTextBounds((char *)p->m_sTitle, 0, 0, &x, &y, &w, &h);
    if(x < min_x) min_x = x;
    if(y < min_y) min_y = y;
    if(x + w > max_x) max_x = x + w;
    if(y + h > max_y) max_y = y + h;
    n++;
  }
  // m_ox = m_margin - min_x;
  // m_oy = m_margin - min_y;
  ww = max_x - min_x + m_margin * 2;
  hh = max_y - min_y + m_margin * 2;
  if(m_w < ww) m_w = ww;
  if(m_rowHeight < hh) m_rowHeight = hh;
  m_h = m_rowHeight * n;
  m_x = (m_pLcd->width() - m_w)/2;
  m_y = (m_pLcd->height() - m_h)/2;
}

int8_t LcdMenu::findSelectedRow(int16_t x, int16_t y)
{
  if(x < 0 || x > m_x + m_w || y < 0 || y > m_y + m_h)
    return -1;
  return (y - m_y) / m_rowHeight;
}

void LcdMenu::drawAll()
{
  if(!m_vSize) adjustSize();
  uint16_t y = m_y;
  Serial.printf("%s:%d m_y:%u\n",__FUNCTION__,__LINE__,m_y);
  
  for(LcdMenuEntry *p=m_lEntry; p; p=p->m_pNext){
    drawRow(p, m_x, y);
    y += m_rowHeight;
  }
}

void LcdMenu::drawRow(LcdMenuEntry *p,uint16_t x, uint16_t y, bool selected)
{
  m_pLcd->setTextSize(m_textSize);
  
  m_pLcd->fillRect(x, y, m_w, m_rowHeight, selected ? m_fg : m_bg);
  if(!selected)
    m_pLcd->drawRect(x, y, m_w, m_rowHeight, m_fg);
    
  if(p->m_sTitle){
    m_pLcd->setTextColor(selected ? m_bg : m_textColor);
#if 0
    m_pLcd->setCursor(x+m_ox, y+m_oy);
    m_pLcd->print(p->m_sTitle);
#else
    m_pLcd->alignPrintf(x + m_w/2, y + m_rowHeight/2, TA_CENTER, TA_CENTER,
			"%s", p->m_sTitle);
#endif
  }
}

void LcdMenu::drawChanged(uint16_t cx, uint16_t cy)
{
  int8_t r = findSelectedRow(cx,cy);
  int i=0;
  uint16_t y = m_y;
  if(r != m_selectedRow){
    for(LcdMenuEntry *p = m_lEntry; p; p=p->m_pNext,i++){
      if(i == m_selectedRow)
	drawRow(p, m_x, y, false);
      if(i == r)
	drawRow(p, m_x, y, true);
      y += m_rowHeight;
    }
    m_selectedRow = r;
  }
}

void LcdMenu::activated()
{
  drawAll();
}

void LcdMenu::reactivated()
{
  m_root->popWin(this);
}

void LcdMenu::tp_pressed(uint16_t x, uint16_t y)
{
  m_selectedRow = -1;
  drawChanged(x,y);
}

void LcdMenu::tp_drag(uint16_t x, uint16_t y)
{
  drawChanged(x,y);
}
void LcdMenu::tp_released(uint16_t x, uint16_t y)
{
  Serial.printf("LcdMenu::%s:%d sel:%d\n",__FUNCTION__,__LINE__,m_selectedRow);
  LcdMenuEntry *p = m_lEntry;
  if(m_selectedRow >= 0){
    for(int i=m_selectedRow; i > 0 && p; i--)
      p = p->m_pNext;
    if(p){
      p->activate(m_root);
      return;
    }
  }
  Serial.printf("LcdMenu::%s:%d\n",__FUNCTION__,__LINE__);
  m_root->popWin(this);
}
