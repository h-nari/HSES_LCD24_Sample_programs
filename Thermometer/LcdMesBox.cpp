#include "LcdMenu.h"

LcdMesBox::LcdMesBox() : LcdWin()
{
  m_x = 40;
  m_y = 20;
  m_w = 320 - m_x * 2;
  m_h = 240 - m_y * 2;
  m_sMes = NULL;
}

void LcdMesBox::dialog(LcdRoot *root, MesType t, const char *mes)
{
  m_type = t;

  removeButton();
  if(t == MT_OK){
    m_btn0.setTextAndValue("OK", R_OK);
    addButton(&m_btn0);
  }
  else if(t == MT_YESNO){
    m_btn0.setTextAndValue("Yes", R_YES);
    addButton(&m_btn0);
    m_btn1.setTextAndValue("No", R_NO);
    addButton(&m_btn1);
  }
  else if(t == MT_YESNOCANCEL){
    m_btn0.setTextAndValue("Yes", R_YES);
    addButton(&m_btn0);
    m_btn1.setTextAndValue("No", R_NO);
    addButton(&m_btn1);
    m_btn2.setTextAndValue("Cancel", R_CANCEL);
    addButton(&m_btn2);
  }
  root->bindWin(this);		// fill m_pLcd
  adjustButtonSize();
  setButtonPosition();
  
  m_sMes = mes;
  m_result = R_NONE;
  root->pushWin(this);	
}


void LcdMesBox::redisplay()
{
  m_pLcd->fillRect(m_x, m_y, m_w, m_h, m_bg);
  m_pLcd->drawRect(m_x, m_y, m_w, m_h, m_fg);
  m_pLcd->setCursor(m_x + 2, m_y + 2);
  m_pLcd->setTextColor(m_fg);
  m_pLcd->setTextSize(m_textSize);
  m_pLcd->print(m_sMes);

  resetButtonSelected();
  drawButton();
}

void LcdMesBox::tp_pressed(uint16_t x, uint16_t y)
{
  setButtonSelected(x, y);
  drawChangedButton();
}

void LcdMesBox::tp_drag(uint16_t x, uint16_t y)
{
  setButtonSelected(x, y);
  drawChangedButton();
}

void LcdMesBox::tp_released(uint16_t x, uint16_t y)
{
  int v;
  if(getButtonValue(&v)){
    m_result = (ResultType)v;
    m_root->popWin(this);
  }
}

