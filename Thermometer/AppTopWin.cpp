#include "AppTopWin.h"
#include <time.h>

AppTopWin::AppTopWin() : LcdWin()
{
  m_x = 0;
  m_y = 0;
  m_w = 320;
  m_h = 240;
  m_pMenu = NULL;
  m_pDB = NULL;

  m_cDate    = Humblesoft_ILI9341::rgb("GREEN");
  m_cTime    = Humblesoft_ILI9341::rgb("WHITE");
  m_cAverage = Humblesoft_ILI9341::rgb("YELLOW");
}

void AppTopWin::activated(void)
{
  if(!m_root || !m_pLcd) return;

  m_pLcd->fillScreen(m_bg);
  m_graph.setLcd(m_pLcd);
  m_graph.setPos(0, 200, 170, 120);
  m_graph.draw(true);
}

void AppTopWin::deactivated()
{
  m_graph.cancelTasks();
}

void AppTopWin::tp_pressed(uint16_t x, uint16_t y)
{
  // Serial.printf("pressed(%u,%u)\n", x,y);
}

void AppTopWin::tp_drag(uint16_t x, uint16_t y)
{
  // Serial.printf("drag(%u,%u)\n", x,y);
}
void AppTopWin::tp_released(uint16_t x, uint16_t y)
{
  // Serial.printf("released(%u,%u)\n", x,y);

  if(m_pMenu && m_root)
    m_pMenu->popup(m_root, x, y);
}

  
void AppTopWin::update()
{
  static time_t t0;
  time_t t = time(NULL);
  float temp;
  char buf[10];

  if(t != t0){
    t0 = t;
    struct tm *tm = localtime(&t);
    const char *wd[7] = {"Sun","Mon","Tue","Wed","Thr","Fri","Sat"};
    m_pLcd->setTextColor(m_cDate, m_bg);
    m_pLcd->setTextSize(2);
    m_pLcd->alignPrintf(120,2,TA_CENTER,TA_TOP, "%04d/%02d/%02d(%s)",
			tm->tm_year+1900,tm->tm_mon+1,
			tm->tm_mday,wd[tm->tm_wday]);
    m_pLcd->setTextColor(m_cTime, m_bg);
    m_pLcd->setTextSize(4);
    m_pLcd->alignPrintf(120,30,TA_CENTER,TA_TOP,"%02d:%02d:%02d",
			tm->tm_hour, tm->tm_min, tm->tm_sec);
    if(m_pDB->getAverageValue(&temp)){
      m_pLcd->setTextColor(m_cAverage, m_bg);
      m_pLcd->setTextSize(5);
      m_pLcd->alignPrintf(120,100,TA_CENTER,TA_TOP,"%s'C",
			  dtostrf(temp, 4, 1, buf));
    }

    m_pLcd->setTextSize(2);
    for(int i=0; i < APP_CURRENT_VALUE_MAX; i++){
      if(m_pDB->getCurrentValue(i,&temp)){
	m_pLcd->setTextColor(m_graph.getDataColor(i), m_bg);
	m_pLcd->alignPrintf(239,200+(5-i)*12*2,TA_RIGHT,TA_TOP,"%s'C",
			    dtostrf(temp, 4, 1, buf));
      }
    }
    m_graph.update(t);
  }
}
