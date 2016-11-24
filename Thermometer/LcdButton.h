#ifndef _lcd_button_h_
#define _lcd_button_h_


class LcdButton {
  friend class LcdWin;
  friend class LcdMesBox;
 protected:
  LcdButton *m_pNext;
  int16_t m_x, m_y;		// box position (absolute position)
  uint16_t m_w, m_h;
  int8_t m_ox, m_oy;		// text position (relative to button);
  const char *m_text;
  bool m_selected;
  bool m_selected0;
  int m_value;

 public:
  LcdButton();
  LcdButton(const char *text, int value);
  void init();
  void adjustSize(LcdDrv *pLcd);
  void draw(LcdDrv *pLcd, uint16_t cFg, uint16_t cBg, uint16_t cText);
  void drawChanged(LcdDrv *pLcd, uint16_t cFg, uint16_t cBg, uint16_t cText);
  void setSelected(uint16_t x, uint16_t y);
  void setTextAndValue(const char *text, int value);
  void setPos(int16_t x, int16_t y);
  void getSize(uint16_t *pw, uint16_t *ph);
};

#endif /* _lcd_button_h_ */
