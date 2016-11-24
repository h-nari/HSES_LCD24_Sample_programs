#ifndef _lcd_mes_box_h_
#define _lcd_mes_box_h_

#include "LcdMenu.h"

class LcdMesBox : public LcdWin {
 public:
  enum MesType { MT_OK, MT_YESNO, MT_YESNOCANCEL };
  enum ResultType { R_NONE, R_OK, R_YES,R_NO, R_CANCEL };
 protected:
  MesType m_type;
  ResultType m_result;
  const char * m_sMes;
  LcdButton m_btn0,m_btn1,m_btn2;
  
 public:
  LcdMesBox();
  void dialog(LcdRoot *root, MesType t, const char *mes);
  void ok(LcdRoot *root, const char *mes) { dialog(root, MT_OK, mes);}
  void yesNo(LcdRoot *root, const char *mes) { dialog(root, MT_YESNO, mes);}
  void yesNoCancel(LcdRoot *root, const char *mes) {
    dialog(root, MT_YESNOCANCEL, mes);}
  bool isOk() { return m_result == R_OK; }
  bool isYes() { return m_result == R_YES; }
  bool isNo() { return m_result == R_NO; }
  bool isCancel() { return m_result == R_CANCEL; }
  virtual void redisplay();
  virtual void tp_pressed(uint16_t x, uint16_t y);
  virtual void tp_drag(uint16_t x, uint16_t y);
  virtual void tp_released(uint16_t x, uint16_t y);
};

#endif /* _lcd_mes_box_h_ */
