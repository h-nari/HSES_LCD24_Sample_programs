#include <Arduino.h>
#include <SD.h>
#include "CsvScanner.h"
#include "nscanf.h"

// #define CSV_READ_BUF_SIZE 512
#define CSV_READ_BUF_SIZE (512*4)
#define CSV_COL_BUF_SIZE 40

static char buf[CSV_READ_BUF_SIZE];

CsvScanner::CsvScanner() : Task()
{
}

bool CsvScanner::scan(const char *path, bool async)
{
  m_file = SD.open(path);
  if(!m_file) return false;
  
  if(async)
    queue.add(this);
  else {
    while(scanBlock())
      yield();
    m_file.close();
  }
  return true;
}

void CsvScanner::run()
{
  if(scanBlock())
    queue.add(this);
  else if(m_file)
    m_file.close();
}

bool CsvScanner::scanBlock()
{
  uint8_t iCol = 0;
  uint16_t ri,sz,wi;
  char sCol[CSV_COL_BUF_SIZE];
  
  if(!m_file) return false;
  ri = sz = wi = 0;
  sz = m_file.read(buf, sizeof buf);
  if(sz == 0) return false;

  while(ri < sz){
    char c = buf[ri++];

    if(c == '\r') continue;
    else if(c == ',' || c == '\n'){
      sCol[wi] = 0;
      onCol(iCol, sCol);
      if(c == ',')
	iCol++;
      else {
	onEol();
	iCol = 0;
      }
      wi = 0;
    }
    else if(wi < sizeof sCol - 1)
      sCol[wi++] = c;
  }
  return true;
}

CsvMinMaxScanner::CsvMinMaxScanner() : CsvScanner()
{
  m_bValidMinMax = false;
}

bool CsvMinMaxScanner::getMinMax(const char *path, bool async)
{
  bool r = scan(path, async);
  if(!r)
    Serial.printf("scan:%s not found.\n",path);
  return r;
}

void CsvMinMaxScanner::run()
{
  if(scanBlock()){
    queue.add(this);
  } else {
    if(m_file)
      m_file.close();
    if(m_pNextTask)
      queue.add(m_pNextTask);
  }
}

void CsvMinMaxScanner::onCol(int nCol,const char *str)
{
  if(nCol == 2){
    float val;
    if(nscanf(str,"%f", &val) != 1)
      Serial.printf("Bad float string:%s\n",str);
    else {
      if(m_bValidMinMax){
	if(val < m_fMin) m_fMin = val;
	if(val > m_fMax) m_fMax = val;
      } else {
	m_fMin = m_fMax = val;
	m_bValidMinMax = true;
      }
    }
  }
}

void CsvMinMaxScanner::onEol()
{
#if 0
  static int cnt;
  Serial.printf("%s:%d minMax:%d\n",__FUNCTION__,__LINE__,cnt++);
#endif
}


CsvDrawGraphScanner::CsvDrawGraphScanner() : CsvScanner()
{
  m_vSid = m_vTime = false;
};

bool CsvDrawGraphScanner::drawGraph(const char *path,AppGraph *pGraph,
				    bool async)
{
  m_pGraph = pGraph;
  return scan(path, async);
}

void CsvDrawGraphScanner::run()
{
  if(scanBlock())
    queue.add(this);
  else {
    if(m_file)
      m_file.close();
    queue.add(m_pGraph);
  }
}

void CsvDrawGraphScanner::onCol(int nCol, const char *str)
{
  if(nCol == 0){ 	/* sid */
    if(nscanf(str,"%d", &m_sid) == 1)
      m_vSid = true;
  }
  else if(nCol == 1){ 	/* date time */
    const char *p = strchr(str,' ');
    int hh,mm,ss;
    if(p && nscanf(p+1,"%d:%d:%d",&hh,&mm,&ss)==3){
      m_time = hh * 3600 + mm * 60 + ss;
      m_vTime = true;
    }
  }
  else if(nCol == 2){ 	/* value */
    float value;
    if(m_vTime && m_vSid && nscanf(str,"%f",&value)==1){
      m_pGraph->drawLine(m_sid, m_time, value);
    }
  }
}

void CsvDrawGraphScanner::onEol()
{
  m_vSid = m_vTime = false;
}
