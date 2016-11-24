#include "LcdMenu.h"

LcdMenuEntry::LcdMenuEntry(const char *title, LcdWin *pWin, bool bPush)
{
  m_sTitle = title;
  m_pWin   = pWin;
  m_bPush  = bPush;
  m_pNext = NULL;
}

void LcdMenuEntry::activate(LcdRoot *root)
{
  Serial.printf("%s:%d push:%d title:%s\n",__FUNCTION__,__LINE__,
		m_bPush,m_sTitle);
  
  if(m_pWin){
    if(!m_bPush)
      root->popWinAll();
    root->pushWin(m_pWin);
  } else {
    root->popWin();
  }
}

