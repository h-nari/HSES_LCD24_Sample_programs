#ifndef _app_db_h_
#define _app_db_h_

#define APP_CURRENT_VALUE_MAX 5

class AppDB {
 protected:
  bool m_vSD;
  bool m_bBlockDataSave;
  bool  m_vValue[APP_CURRENT_VALUE_MAX];
  bool  m_bDrawn[APP_CURRENT_VALUE_MAX];
  float m_fValue[APP_CURRENT_VALUE_MAX];

 public:
  AppDB();
  void dataPut(const char *sid, const char *time, const char *value);
  void setSDValid(bool b = true) { m_vSD = b;}
  void setCurrentValue(int sid, float value);
  bool getCurrentValue(int sid, float *pValue, bool *pDrawn = NULL);
  bool getAverageValue(float *pValue);
  void makeDataFileName(int yy,int mm,int dd,char *buf, size_t buflen);
  void makeDataFileName(time_t date,char *buf, size_t buflen);
  void setCurrentValueDrawn(int sid, bool bDrawn = true){
    if(sid>0&&sid<APP_CURRENT_VALUE_MAX)m_bDrawn[sid] = bDrawn;
  }
  void blockDataSave(bool b=true){ m_bBlockDataSave = b;}

 protected:
  bool dirCheck(const char *path);
  void dataSave(const char *sid, const char *time, const char *value);

};


#endif /* _app_db_h_ */
