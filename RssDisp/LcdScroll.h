#ifndef _lcd_scroll_h_
#define _lcd_scroll_h_

#define USE_FONTX 1

#include <Arduino.h>
#include "FCanvas.h"
#include <Humblesoft_ILI9341.h>


class LcdScroll : public Print {
 protected:
  Humblesoft_ILI9341 *m_pLcd;
  FCanvas1	   *m_pCanvas;
  String            m_sText;
  int               m_iRead;	// read index
  int	m_ix;
  int	m_w;
  int   m_ri;
  
 public:
  LcdScroll(Humblesoft_ILI9341 *pLcd, FCanvas1 *pCanva);
  
  size_t write(uint8_t c) override;
  bool scroll();
  uint16_t getPixel(int16_t x, int16_t);
  void setTextSize(uint8_t s) { m_pCanvas->setTextSize(s);}
};



#endif /* _lcd_scroll_h_ */
