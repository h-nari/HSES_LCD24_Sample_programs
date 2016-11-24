#ifndef _lcd_win_h_
#define _lcd_win_h_

#include <Arduino.h>
#include "LcdRoot.h"
#include "LcdButton.h"

class LcdMenu;

class LcdWin {
  friend class LcdRoot;
  
 protected:
  LcdRoot *m_root;
  LcdDrv  *m_pLcd;
  uint16_t m_x;
  uint16_t m_y;
  uint16_t m_w;
  uint16_t m_h;
  LcdWin *m_pNext;
  LcdButton *m_lButton;
  uint16_t m_fg, m_bg;
  uint8_t m_textSize;
  LcdMenu *m_pMenu;
  
 public:
  LcdWin();
  void setPos(uint16_t x, uint16_t y) { m_x = x; m_y = y;}
  void setSize(uint16_t w, uint16_t h) { m_w = w; m_h = h;}
  virtual void redisplay(){ if(m_pLcd) m_pLcd->fillScreen(m_bg);};
  virtual void activated(){redisplay();};
  virtual void reactivated(){activated();};
  virtual void deactivated(){};
  virtual void tp_pressed(uint16_t x, uint16_t y){}
  virtual void tp_drag(uint16_t x, uint16_t y){};
  virtual void tp_released(uint16_t x, uint16_t y);
  virtual void update(){};
  bool isHit(uint16_t x, uint16_t y);
  void addButton(LcdButton *btn);
  void removeButton();
  void drawButton();
  void drawChangedButton();
  void resetButtonSelected();
  void setButtonSelected(uint16_t x, uint16_t y);
  void adjustButtonSize();
  void setButtonPosition();
  bool getButtonValue(int *pVal);
  void setMenu(LcdMenu *menu)  { m_pMenu = menu;  }
};


#endif /* _lcd_win_h_ */
