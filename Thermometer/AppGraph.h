#ifndef _app_graph_h_
#define _app_graph_h_

#include <Arduino.h>
#include "LcdMenu.h"
#include "AppDB.h"
#include "Task.h"

#define GRAPH_LINE_MAX APP_CURRENT_VALUE_MAX

class AppGraph : public Task {
 protected:
  AppDB  	*m_pDB;
  LcdDrv 	*m_pLcd;
  time_t 	m_tNow;		// 現在時刻、日付の基準
  int8_t	m_cBackDay;    	// 何日前まで表示するか？
  int8_t	m_iBackDay;    	// 
  uint8_t	m_iStartHour;  	// 時間範囲
  uint8_t	m_iEndHour;
  uint8_t	m_iTempRange;	// 温度表示幅最小値
  float		m_fTempMax;	// 温度表示範囲
  float		m_fTempMin;
  bool		m_bAutoRange;
  bool		m_bDrawn;
  
  int16_t	m_ax;		// sx = x/a + b
  int16_t	m_bx;
  float		m_ay;		// sy = a*x + b
  int16_t	m_by;

  int16_t	m_x0;		// 描画位置
  int16_t	m_x1;
  int16_t	m_x2;	
  int16_t	m_x3;
  int16_t	m_y0;
  int16_t	m_y1;
  int16_t	m_y2;
  int16_t	m_y3;

  bool		m_bGray;
  uint16_t	m_cBg;
  uint16_t	m_cFrame;	// グラフ枠色
  uint16_t      m_cData[APP_CURRENT_VALUE_MAX];

  enum DrawState { DS_AUTO_RANGE, DS_SET_SCALE, DS_DAY_GRAPH};
  DrawState 	m_drawState;
  
 public:
  AppGraph();
  void setDB(AppDB *pDB) { m_pDB = pDB;}
  void setLcd(LcdDrv *pLcd) { m_pLcd = pLcd;}
  void setPos(int16_t x, int16_t y, int16_t w, int16_t h);
  bool draw(bool async);
  void drawFrame();
  void drawLine(int sid, time_t time, float value);
  inline int16_t sx(time_t t) { return t / m_ax + m_bx;};
  inline int16_t sy(float t) { return t * m_ay + m_by;} ;
  void update(time_t t);
  uint16_t getDataColor(int sid);
  void cancelTasks();
  
 protected:
  void getDayMinMax(time_t date, bool *pValidMinMax,
		    float *pMin, float *pMax);
  void autoRange(bool async);
  void setScale();
  void drawDayGraph(time_t date, bool bGray = false, bool async = false);
  void run();
};


#endif /* _app_graph_h_ */
