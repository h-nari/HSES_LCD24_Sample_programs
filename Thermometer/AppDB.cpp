#include <Arduino.h>
#include <SD.h>
#include <time.h>
#include "AppDB.h"
#include "nscanf.h"

AppDB::AppDB()
{
  m_vSD = false;
  m_bBlockDataSave = false;
  for(int i=0; i<APP_CURRENT_VALUE_MAX; i++)
    m_vValue[i] = false;
}

bool AppDB::dirCheck(const char *path)
{
  char buf[60];
  char *p = strrchr(path,'/');
  if(!p) return false;
  int len = p - path;
  if(len > sizeof buf - 1) return false;
  strncpy(buf, path, len);
  buf[len] = 0;
  return SD.mkdir(buf);
}

void AppDB::dataSave(const char *sid, const char *time,
			    const char *value)
{
  int yy,mm,dd;
  char path[60];

  if(m_bBlockDataSave) {
    Serial.printf("%s:%d blocked\n",__FUNCTION__,__LINE__);
    return;
  }
  
  if(nscanf(time,"%d/%d/%d", &yy,&mm,&dd)==3){
    makeDataFileName(yy,mm,dd,path,sizeof path);
    if(SD.exists(path) || dirCheck(path)){
      File file = SD.open(path, FILE_WRITE);
      if(file)
	file.printf("%s,%s,%s\n",sid,time,value);
      else
	Serial.printf("open %s failed.\n",path);
      file.close();
    } else {
      Serial.printf("mkdir for %s failed.\n",path);
    }
  }
  else Serial.printf("parse_date failed.\n");
}


void AppDB::dataPut(const char *sid_str, const char *time_str,
		    const char *value_str)
{
  int sid;
  float value;
  Serial.printf("%s sid:%s %s\n",time_str, sid_str, value_str);

  if(nscanf(sid_str,"%d\f",&sid)!=2 || nscanf(value_str,"%f\f",&value)!=2){
    Serial.printf("bad data format.\n");
    return;
  }
  
  if(m_vSD)
    dataSave(sid_str, time_str, value_str);

  double d = strtod("10.4", NULL);
  setCurrentValue(sid, value);
}

void AppDB::setCurrentValue(int sid, float value)
{
  if(sid >= 0 && sid < APP_CURRENT_VALUE_MAX){
    m_vValue[sid] = true;
    m_bDrawn[sid] = false;
    m_fValue[sid] = value;
  }
}

bool AppDB::getAverageValue(float *pValue)
{
  int cnt = 0;
  float sum = 0.0;
  for(int i=0; i<APP_CURRENT_VALUE_MAX;i++){
    if(m_vValue[i]){
      cnt++;
      sum += m_fValue[i];
    }
  }
  if(cnt == 0)
    return false;
  *pValue = sum / cnt;
  return true;
}

bool AppDB::getCurrentValue(int sid, float *pValue, bool *pDrawn)
{
  if(sid >= 0 && sid < APP_CURRENT_VALUE_MAX && m_vValue[sid]){
    if(pValue) *pValue = m_fValue[sid];
    if(pDrawn) *pDrawn = m_bDrawn[sid];
    return true;
  }
  return false;
}

void AppDB::makeDataFileName(int yy,int mm,int dd,char *buf, size_t buflen)
{
  snprintf(buf,buflen, "/tdata/%04d/%02d/td%02d%02d%02d.csv",
	   yy ,mm,yy % 100,mm,dd);
}

void AppDB::makeDataFileName(time_t date,char *buf, size_t buflen)
{
  struct tm *tm = localtime(&date);
  makeDataFileName(tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday, buf, buflen);
}
