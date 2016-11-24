#ifndef _app_top_win_h_
#define _app_top_win_h_

#include "LcdMenu.h"
#include "AppDB.h"
#include "AppGraph.h"

class AppTopWin : public LcdWin {
 public:
  AppTopWin();
  void activated(void) override;
  void deactivated(void) override;
  void tp_pressed(uint16_t x, uint16_t y) override;
  void tp_drag(uint16_t x, uint16_t y) override;
  void tp_released(uint16_t x, uint16_t y) override;
  void setMenu(LcdMenu *menu)  { m_pMenu = menu;  }
  void update() override;
  void setDB(AppDB *pDB) {m_pDB = pDB; m_graph.setDB(pDB);}
  
 protected:
  // LcdMenu *m_pMenu;
  AppDB *m_pDB;
  AppGraph m_graph;
  // colors
  uint16_t m_cDate;
  uint16_t m_cTime;
  uint16_t m_cAverage;
};


#endif /* _app_top_win_h */
