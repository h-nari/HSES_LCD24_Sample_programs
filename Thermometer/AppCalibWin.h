#ifndef _app_calib_win_h_
#define _app_calib_win_h_

#include <LcdTouchScreen.h>
#include "LcdWin.h"

class AppCalibWin : public LcdWin {
 protected:
 public:
  AppCalibWin();
  
 protected:
  void activated() override;
};

#endif /* _app_calib_win_h */
