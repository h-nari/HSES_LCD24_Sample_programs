#ifndef _lcd_root_h_
#define _lcd_root_h_

#include <Humblesoft_ILI9341.h>
typedef Humblesoft_ILI9341 LcdDrv;

class LcdWin;
class LcdTouchscreen;

class LcdRoot {
 protected:
  LcdDrv *m_pLcd;
  LcdTouchscreen *m_pTp;
  LcdWin *m_lWin;
  bool    m_bPressed0;
  int16_t m_x0;
  int16_t m_y0;
  uint16_t m_bg;
  bool 	m_vSize;	/* valid size */

 public:
  LcdRoot(LcdDrv *lcd, LcdTouchscreen *tp);
  void begin();
  void update();
  void bindWin(LcdWin *pWin);
  void pushWin(LcdWin *pWin);
  void popWin(LcdWin *pWin = NULL);
  void popWinAll();
  LcdDrv *getLcd() {return m_pLcd;}
  void redisplay();
};

#endif /* _lcd_root_h */
