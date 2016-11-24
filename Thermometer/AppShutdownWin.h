#ifndef _app_shutdown_win_h_
#define _app_shutdown_win_h_

#include <LcdTouchScreen.h>
#include "LcdWin.h"

class AppShutdownWin : public LcdWin {
 protected:
  LcdButton m_btnCancel;
  
 public:
  AppShutdownWin();
  
 protected:
  void activated() override;
  void redisplay() override;
  void tp_pressed(uint16_t x, uint16_t y) override;
  void tp_drag(uint16_t x, uint16_t y) override;
  void tp_released(uint16_t x, uint16_t y) override;
};

#endif /* _app_shutdown_win_h */
