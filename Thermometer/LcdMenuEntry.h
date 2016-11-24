#ifndef _lcd_menu_entry_h_
#define _lcd_menu_entry_h_

#include "LcdWin.h"

class LcdMenu;
class LcdMenuEntry {
  friend class LcdMenu;
 public:
  LcdMenuEntry(const char *title=NULL, LcdWin *pWin = NULL, bool bPush = true);
  virtual void activate(LcdRoot *root);
  
 protected:
  const char *m_sTitle;
  bool    m_bPush;
  LcdWin *m_pWin;
  class LcdMenuEntry *m_pNext;
};

#endif /* _lcd_menu_entry_h_ */
