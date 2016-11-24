#ifndef _task_h_
#define _task_h_

class TaskQueue;
class Task {
  friend class TaskQueue;
 protected:
  bool m_bInQueue;
  class Task *m_pNext;

 public:
  Task();
  
 protected:
  virtual void run() = 0;
};

class TaskQueue {
 protected:
  Task *m_pHead;
  Task *m_pTail;
  
 public:
  TaskQueue();
  void add(Task *pTask);
  void cancel(Task *pTask);
  void update();
};

extern TaskQueue queue;


#endif /* _task_h_ */
