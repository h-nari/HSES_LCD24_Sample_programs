#include <Arduino.h>
#include "Task.h"

Task::Task()
{
  m_bInQueue = false;
}

TaskQueue::TaskQueue()
{
  m_pHead = m_pTail = NULL;
}

void TaskQueue::add(Task *pTask)
{
  if(pTask->m_bInQueue)
    Serial.println("Task already in queue.");
  else {
    if(m_pTail == NULL)
      m_pHead = pTask;
    else 
      m_pTail->m_pNext = pTask;
    m_pTail = pTask;
    pTask->m_bInQueue = true;
    pTask->m_pNext = NULL;
  }
}

void TaskQueue::cancel(Task *pTask)
{
  Task *p,*p0=NULL;

  for(p=m_pHead; p!=NULL; p0=p,p=p->m_pNext){
    if(p == pTask){
      if(p0) p0->m_pNext = p->m_pNext;
      else {
	m_pHead = p->m_pNext;
	if(m_pHead == NULL)
	  m_pTail = NULL;
      }
      p->m_bInQueue = false;
    }
  }
}

void TaskQueue::update()
{
  if(m_pHead){
    Task *task = m_pHead;
    m_pHead = m_pHead->m_pNext;
    if(!m_pHead) m_pTail = NULL;
    task->m_bInQueue = false;
    task->run();
  }
}
