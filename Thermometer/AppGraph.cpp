#include <Arduino.h>
#include "AppGraph.h"
#include "CsvScanner.h"
#include <math.h>

#define DAYSEC		(24 * 60 * 60)

static CsvMinMaxScanner sc;
static CsvDrawGraphScanner scanner;

AppGraph::AppGraph()
{
  m_bDrawn = false;
  m_pDB = NULL;
  m_pLcd = NULL;
  m_tNow = 0;
  m_cBackDay = 1;
  m_iStartHour = 0;
  m_iEndHour = 24;
  m_fTempMax = 25.0;
  m_fTempMin = 15.0;
  m_iTempRange = 5;
  m_bAutoRange = true;
  m_x0 = 0;
  m_y0 = 0;
  m_x1 = 320;
  m_y1 = 240;

  m_bGray = false;
  m_cBg 	= ILI9341_BLACK;
  m_cFrame 	= ILI9341_WHITE;
  m_cData[0] 	= ILI9341_WHITE;
  m_cData[1] 	= ILI9341_BLUE;
  m_cData[2] 	= ILI9341_GREEN;
  m_cData[3] 	= ILI9341_YELLOW;
  m_cData[4] 	= ILI9341_RED;
}

void AppGraph::setPos(int16_t x, int16_t y, int16_t w, int16_t h)
{
  m_x0 = x;
  m_x3 = x + w - 1;
  m_x1 = m_x0 + 20;
  m_x2 = m_x3 - 8;

  m_y0 = y;
  m_y3 = y + h - 1;
  m_y1 = m_y0;
  m_y2 = m_y3 - 10;
}


bool AppGraph::draw(bool async)
{
  if(!m_pDB || !m_pLcd || !m_tNow)
    return false;

  cancelTasks();
  m_bDrawn = true;
  if(!async){
    if(m_bAutoRange)
      autoRange(async);
    setScale();
    drawFrame();

    for(int8_t b = m_cBackDay; b >= 0; b--)
      drawDayGraph( m_tNow - b * DAYSEC, b > 0, async);
  } else {
    setScale();
    drawFrame();
    m_drawState = DS_AUTO_RANGE;
    m_iBackDay = m_cBackDay;
    queue.add(this);
  }
  return true;
}

void AppGraph::cancelTasks()
{
  queue.cancel(this);
  queue.cancel(&sc);
  queue.cancel(&scanner);
}

void AppGraph::run()
{
  switch(m_drawState){
  case DS_AUTO_RANGE:
    if(m_bAutoRange) {
      if(m_iBackDay >= 0){
	autoRange(true);
	m_iBackDay --;
	break;
      } else {
	m_fTempMax = ceil(sc.m_fMax)  + 1;
	m_fTempMin = floor(sc.m_fMin) - 1;

	if(m_fTempMax - m_fTempMin < m_iTempRange){
	  m_fTempMax = m_fTempMin + m_iTempRange / 2;
	  m_fTempMin = m_fTempMax - m_iTempRange;
	}
      }
    } 
    m_drawState = DS_SET_SCALE;
    queue.add(this);
    break;
    
  case DS_SET_SCALE:
    setScale();
    drawFrame();
    m_drawState = DS_DAY_GRAPH;
    m_iBackDay = m_cBackDay;
    queue.add(this);
    break;
    
  case DS_DAY_GRAPH:
    if(m_iBackDay >= 0){
      drawDayGraph(m_tNow - m_iBackDay * DAYSEC, m_iBackDay > 0, true);
      m_iBackDay--;
    } 
    break;
    
  default:
    Serial.printf("undefined drawState:%d\n", m_drawState);
    break;
  }
}

void AppGraph::getDayMinMax(time_t date, bool *pValidMinMax,
			    float *pMin, float *pMax)
{
}

void AppGraph::autoRange(bool async)
{
  char path[80];
  
  if(!async){
    for(int8_t b = m_cBackDay;b>=0; b--){
      time_t date = m_tNow - b * DAYSEC; 
      m_pDB->makeDataFileName(date, path, sizeof path);
      sc.getMinMax(path, async);
    }

    if(sc.m_bValidMinMax){
      m_fTempMax = ceil(sc.m_fMax)  + 1;
      m_fTempMin = floor(sc.m_fMin) - 1;

      if(m_fTempMax - m_fTempMin < m_iTempRange){
	m_fTempMax = m_fTempMin + m_iTempRange / 2;
	m_fTempMin = m_fTempMax - m_iTempRange;
      }
    }
  } else {
    sc.setNextTask(this);
    time_t date = m_tNow - m_iBackDay * DAYSEC; 
    m_pDB->makeDataFileName(date, path, sizeof path);
    if(!sc.getMinMax(path, true))
      queue.add(this);			/* file not found. next file */
  }
}

void AppGraph::drawDayGraph(time_t date, bool bGray, bool async)
{
  char path[80];
  m_pDB->makeDataFileName(date, path, sizeof path);
  m_bGray = bGray;
  scanner.drawGraph(path, this, async);
}


void AppGraph::setScale()
{
  float ax,bx,ay,by;

  ax = (m_x2 - m_x1 - 2) / ((m_iEndHour - m_iStartHour) * 3600.0);
  bx = m_x1 - m_iStartHour * 3600 * ax + 1;
  ay = (m_y1 - m_y2 + 2) / (m_fTempMax - m_fTempMin);
  by = m_y2 - m_fTempMin * ay + 1;

  m_ax = (int16_t)(1.0 / ax);
  m_bx = (int16_t)bx;
  m_ay = ay;
  m_by = (int16_t)by;
}

void AppGraph::drawLine(int sid, time_t time, float value)
{
  int16_t x = sx(time % DAYSEC);
  int16_t y = sy(value);
  uint16_t color;

  if(m_bGray || sid < 0 || sid >= APP_CURRENT_VALUE_MAX)
    color = ILI9341_DARKGREY;
  else
    color = m_cData[sid];
  if(y > m_y0 && y < m_y3 && x > m_x0 && x < m_x3)
    m_pLcd->drawPixel(x, y, color);
}

void AppGraph::drawFrame()
{
  uint16_t tw,th,tw2,th2;
  int16_t  tx,ty;
  char buf[10];
  
  m_pLcd->getTextBounds("12", 0, 0, &tx, &ty, &tw2, &th2);
  m_pLcd->fillRect(m_x0, m_y0, m_x3 - m_x0 + 1, m_y3 - m_y0 + 1, m_cBg);
  
  // x-tics

  const static uint8_t xpitch[] = {1,3,6};
  uint16_t aColor[3];
  
  aColor[0] = m_pLcd->colorRGB(80,80,80);
  aColor[1] = m_pLcd->colorRGB(150,150,150);
  aColor[2] = m_pLcd->colorRGB(200,200,200);

  m_pLcd->setTextSize(1);
  m_pLcd->setTextColor(ILI9341_WHITE);

  for(int i=0;i<3;i++){
    int w = xpitch[i] * 3600 / m_ax;
    for(int h=0;h<24;h+=xpitch[i]){
      if(h > m_iStartHour && h < m_iEndHour){
	int x = sx(h * 3600);
	if(w > 3)
	  m_pLcd->drawFastVLine(x,m_y1,m_y2-m_y1, aColor[i]);
      }
    }
  }

  for(int i=0;i<3;i++){
    int w = xpitch[i] * 3600 / m_ax;
    if(w > tw2+2 ){
      for(int h=0;h<=24;h+=xpitch[i]){
	if(h >= m_iStartHour && h <= m_iEndHour){
	  int x = sx(h * 3600);
	  m_pLcd->alignPrintf(x, m_y2+2, TA_CENTER, TA_TOP,"%d",h);
	}
      }
      break;
    }
  }

  // y-tics

  const static uint8_t ypitch[] = {1,5,10};
  for(int i=0;i<3;i++){
    int h =  -m_ay * ypitch[i];
    if(h > 3){
      int t = (int)m_fTempMin / ypitch[i] * ypitch[i];
      if(t < (int)m_fTempMin) t += ypitch[i];
      for(;t<(int)m_fTempMax; t += ypitch[i]){
	int y = sy(t);
	m_pLcd->drawFastHLine(m_x1,y,m_x2-m_x1, aColor[i]);
      }
    }
  }
  for(int i=0;i<3;i++){
    int h = -m_ay * ypitch[i];
    if(h > th2 + 2){
      int t = (int)m_fTempMin / ypitch[i] * ypitch[i];
      if(t < (int)m_fTempMin) t += ypitch[i];
      for(;t<=(int)(m_fTempMax+0.1); t += ypitch[i]){
	int y = sy(t);
	m_pLcd->alignPrintf(m_x1 - 2, y, TA_RIGHT, TA_CENTER,"%d",t);
      }
      break;
    }
  }

  // frame
  m_pLcd->drawRect(m_x1, m_y1, m_x2 - m_x1 + 1, m_y2 - m_y1 + 1, m_cFrame);
}

void AppGraph::update(time_t t)
{
  if(!m_bDrawn || m_tNow / DAYSEC != t/DAYSEC){
    m_tNow = t;
    draw(true);
  }
  if(m_bDrawn){
    for(int i=0; i<APP_CURRENT_VALUE_MAX; i++){
      float temp;
      bool  bDrawn;
      if(m_pDB->getCurrentValue(i, &temp, &bDrawn) && !bDrawn && m_ax){
	if(temp > m_fTempMax || temp < m_fTempMin)
	  m_bDrawn = false;
	drawLine(i, t, temp);
	m_pDB->setCurrentValueDrawn(i);
      }
    }
  }
}

uint16_t AppGraph::getDataColor(int sid)
{
  uint16_t color;
  if(sid < 0 || sid >= APP_CURRENT_VALUE_MAX)
    color = ILI9341_DARKGREY;
  else
    color = m_cData[sid];
  return color;
}
  
