#ifndef _csv_scanner_h_
#define _csv_scanner_h_

#include <SD.h>
#include "AppGraph.h"
#include "Task.h"

class CsvScanner : public Task {
 protected:
  File m_file;
 public:
  CsvScanner();
  void run() override;
  
 protected:
  bool scan(const char *path, bool async = true);
  bool scanBlock();
  virtual void onCol(int nCol,const char *str){};
  virtual void onEol(){};
};

class CsvMinMaxScanner : public CsvScanner {
 public:
  bool  m_bValidMinMax;
  float m_fMin;
  float m_fMax;
 protected:
  Task  *m_pNextTask;
  
 public:
  CsvMinMaxScanner();
  bool getMinMax(const char *path,bool async);
  void setNextTask(Task *pTask) { m_pNextTask = pTask;}
  
 protected:
  void run() override;
  void onCol(int nCol,const char *str) override;
  void onEol() override;
};

class CsvDrawGraphScanner : public CsvScanner {
 protected:
  AppGraph *m_pGraph;
  int 	  m_sid;
  time_t  m_time;
  bool	  m_vSid;
  bool    m_vTime;
  
 public:
  CsvDrawGraphScanner();
  bool drawGraph(const char *path,AppGraph *pGraph, bool async);
  
 protected:
  void run() override;
  void onCol(int nCol,const char *str) override;
  void onEol() override;
};


#endif /* _csv_scanner_h_ */

