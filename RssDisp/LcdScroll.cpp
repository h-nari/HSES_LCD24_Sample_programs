#include "LcdScroll.h"

LcdScroll::LcdScroll(Humblesoft_ILI9341 *pLcd, FCanvas1 *pCanvas)
{
  m_pLcd = pLcd;
  m_pCanvas = pCanvas;
  m_w = m_ix = m_ri = 0;
  m_pLcd->setScrollStep(-1);
}

size_t LcdScroll::write(uint8_t c)
{
  m_sText += (char)c;
  return 0;
}

bool LcdScroll::scroll()
{
  if(m_ix >= m_w){ 		// bitmap_empty
    if(m_ri >= m_sText.length())
      return false;

    // 1文字描画
    int i = 0;
    char buf[8];

    do {
      if(i+1 >= sizeof buf)
	return false;
      buf[i++] = m_sText[m_ri++];
    } while(m_ri < m_sText.length() && (m_sText[m_ri] & 0xc0) == 0x80);
    buf[i] = 0;

    int16_t x1,y1;
    uint16_t w, h;
    m_pCanvas->getTextBounds(buf, 0, 0, &x1, &y1, &w, &h);
    m_pCanvas->fillScreen(0);
    m_pCanvas->setCursor(0,0);
    m_pCanvas->print(buf);

    m_w = w + 1;
    m_ix = 0;
  }
  m_pLcd->scroll(false);

  for(int y=0; y < m_pLcd->height() && y < m_pCanvas->height(); y++)
    m_pLcd->scrollDrawPixel(0, y, getPixel(m_ix, y));
  m_ix++;
  return true;
}

uint16_t LcdScroll::getPixel(int16_t x, int16_t y)
{
  uint8_t *buf = m_pCanvas->getBuffer(),d;
  uint16_t w = m_pCanvas->width();
  uint16_t h = m_pCanvas->height();
  uint16_t pixel = ILI9341_BLACK;

  if(0 <= x && x  < w && 0 <= y && y < h){
    d = buf[x/8 + y * ((w + 7)/8)];
    pixel = (d & (0x80 >>(x & 7))) ? ILI9341_WHITE : ILI9341_BLACK;
  }
  return pixel;
}

