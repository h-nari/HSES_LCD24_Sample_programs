#ifndef _lcd_calib_win_h_
#define _lcd_calib_win_h_

#include "LcdMenu.h"

class LcdCalibWin : public LcdWin {
 protected:
  int8_t m_state;	/* -1: error, 0,1,2,3 calibration point */ 
  uint32_t m_waitStart;
  LcdMesBox m_mesBox;
    
 public:
  LcdCalibWin();
  void redisplay() override;
  void activated() override;
  void reactivated() override;
  void tp_released(uint16_t x, uint16_t y) override;

 protected:
};


#endif /* _lcd_calib_win_h_ */
