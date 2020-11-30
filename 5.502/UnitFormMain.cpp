/////////////////////////////////////////////////////////////////////////////
//  fiberOS/CSP
//  ver. 5.502
//
//  Многозадачна система с диспечеризация "без изтласкване" (non-preemptive)
//  Развитие на MICROS за платформа Win32/x86/Protected Mode
//
/////////////////////////////////////////////////////////////////////////////
//
//	ФУНКЦИОНАЛНОСТ
//
//	Тема: Задача "Производител - потребител" от типа 1:2
//
//  - Използване на влакна (fibers)
//	- Опашката на диспечера е изпълнена чрез stl::queue
//	- Добавени са състояния на задачите
//	- Управление на задачите чрез функциите Suspend()/Resume()
//	- Визуализация на системното състояние
//
/////////////////////////////////////////////////////////////////////////////
//
//	ТЕКУЩА РАБОТА: CSP поддръжка
//
/////////////////////////////////////////////////////////////////////////////
//  Състояния на задачите:
//  Луканчевски, М. Системно програмиране за едночипови микрокомпютри.
//  София, Техника, 1993, ISBN 954-03-0319-2.
//  (стр. 102-103, фиг. 2.9)
/////////////////////////////////////////////////////////////////////////////

#include <vcl.h>
#pragma hdrstop

#include "UnitFormMain.h"
#include "UnitCSP.h"
//---------------------------------------------------------------------------
#include "UnitTASK1.h"
#include "UnitTASK2.h"
#include "UnitTASK3.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TformMain *formMain;
//---------------------------------------------------------------------------
//  СИСТЕМЕН ДЕСКРИПТОР /////////////////////////////////////////////////////
TASK*			runtask;  		// активна задача
queue < TASK* > sysReadyQ;		// опашка на готовите задачи (на диспечера)
PTASKCONTEXT	sysContext;		// системен контекст
unsigned		sys_timer;		// системен таймер
unsigned		sys_tasks_num;	// общ брой на задачите в системата

//  ПРИЛОЖНИ ЗАДАЧИ /////////////////////////////////////////////////////////
TASK*			taskP;			// задача "производител"
TASK*			taskQ1;			// задача "потребител1"
TASK*			taskQ2;			// задача "потребител2"

//  КОМУНИКАЦИОННИ КАНАЛИ ///////////////////////////////////////////////////
CHAN* 			chanPQ1;		// канал между P и Q1
CHAN* 			chanPQ2;		// канал между P и Q2

//  СИСТЕМЕН ПРОЦЕС /////////////////////////////////////////////////////////
//
__fastcall TformMain::TformMain(TComponent* Owner)
	: TForm(Owner)
{
  pbarTask1->Max = LIMIT;
  pbarTask2->Max = LIMIT;
  pbarTask3->Max = LIMIT;

  buttonSuspend1->Enabled = true;
  buttonSuspend2->Enabled = true;
  buttonSuspend3->Enabled = true;
  buttonResume1->Enabled = false;
  buttonResume2->Enabled = false;
  buttonResume3->Enabled = false;

  sysContext = ConvertThreadToFiber(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TformMain::buttonStartClick(TObject *Sender)
{
  buttonStart->Enabled = false;

  sys_timer = 0;
  sys_tasks_num = 0;

  // Създаване на задачите и включването им в системата
  taskP = CreateTask("Task P", Task1);
  taskQ1 = CreateTask("Task Q1", Task2);
  taskQ2 = CreateTask("Task Q2", Task3);

  // Създаване на каналите
  chanPQ1 = CreateChan("Chan PQ1", taskP, taskQ1);
  chanPQ2 = CreateChan("Chan PQ2", taskP, taskQ2);

  while(sys_tasks_num > 0)
  {
	// РАБОТЕН ЦИКЪЛ НА СИСТЕМНИЯ ПРОЦЕС

	stSysTimer->Caption = sys_timer;
	stSysTasksNum->Caption = sys_tasks_num;
	stSysReadyTasksNum->Caption = sysReadyQ.size();

	Dispatcher();		// преход към диспечера

	Application->ProcessMessages();

	if(taskP->state == TS_STOP &&
	   taskQ1->state == TS_STOP &&
	   taskQ2->state == TS_STOP)
	  break;			// изход, ако всички задачи са терминарани
  }

  // Премахване на каналите от системата
  delete chanPQ1;
  delete chanPQ2;

  // Премахване на задачите от системата
  DeleteTask(taskP);
  DeleteTask(taskQ1);
  DeleteTask(taskQ2);

  stSysTimer->Caption = sys_timer;
  stSysTasksNum->Caption = sys_tasks_num;
  stSysReadyTasksNum->Caption = sysReadyQ.size();

  buttonStart->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TformMain::buttonSuspend1Click(TObject *Sender)
{
  if(Sender == buttonSuspend1)
  {
	Suspend(taskP);
	buttonSuspend1->Enabled = false;
	buttonResume1->Enabled = true;
  }
  else if(Sender == buttonSuspend2)
  {
	Suspend(taskQ1);
	buttonSuspend2->Enabled = false;
	buttonResume2->Enabled = true;
  }
  else if(Sender == buttonSuspend3)
  {
	Suspend(taskQ2);
	buttonSuspend3->Enabled = false;
	buttonResume3->Enabled = true;
  }
}
//---------------------------------------------------------------------------
void __fastcall TformMain::buttonResume1Click(TObject *Sender)
{
  if(Sender == buttonResume1)
  {
	Resume(taskP);
	buttonSuspend1->Enabled = true;
	buttonResume1->Enabled = false;
  }
  else if(Sender == buttonResume2)
  {
	Resume(taskQ1);
	buttonSuspend2->Enabled = true;
	buttonResume2->Enabled = false;
  }
  else if(Sender == buttonResume3)
  {
	Resume(taskQ2);
	buttonSuspend3->Enabled = true;
	buttonResume3->Enabled = false;
  }
}
/////////////////////////////////////////////////////////////////////////////
//  СИСТЕМНИ ПРИМИТИВИ
/////////////////////////////////////////////////////////////////////////////
//  CreateTask()
//  Създаване на приложна задача от тип влакно (fiber)
//
//  Входни аргументи:
//		name 		- име на задачата
//		pfMain		- главна функция на задачата
//		pvPar		- параметри на главната функция
//		dwStackSize - размер на стека (по подразбиране е 1 MB)
//  Връщана стойност: идентификатор на създадената задача
//
TASK* CreateTask(char* name,
				 PFIBER_START_ROUTINE pfMain, PVOID pvParam, DWORD dwStackSize)
{
  TASK* task = new TASK;

  if(task)
  {
	sys_tasks_num++;

	strncat(task->tag, name, TAGSIZE-1);
	task->state = TS_READY;
	task->context = CreateFiber(dwStackSize, pfMain, pvParam);
	InsertInReadyQ(task);
  }

  return task;
}
//---------------------------------------------------------------------------
//  DeleteTask()
//  Отстраняване на приложна задача
//
//  Входни аргументи:
//		task 		- идентификатор на задачата
//
void DeleteTask(TASK* task)
{
  if(sys_tasks_num > 0)
	sys_tasks_num--;

  RemoveTaskFromReadyQ(task);
  DeleteFiber(task->context);
  delete task;
}
//---------------------------------------------------------------------------
//  Switch()
//  Превключване от контекста на активната задача към системния контекст
//
void Switch(void)
{
  if(runtask)
  {
	if(runtask->state == TS_RUN)
	{	// вмъкване на активната задача в опашката на диспечера
	  runtask->state = TS_READY;
	  InsertInReadyQ(runtask);
	}
  }

  // превключване към системния контекст
  SwitchToFiber(sysContext);
}
//---------------------------------------------------------------------------
//  Dispatcher()
//  Диспечер на задачите
//
void Dispatcher(void)
{
  sys_timer++;

  // извличане на следващата готова задача от опашката на диспечера
  runtask = RemoveFirstFromReadyQ();

  if(runtask)
  { // превключване към контекста на задачата
	runtask->state = TS_RUN;			// switch to RUN state
	SwitchToFiber(runtask->context);	// switch to task context
  }
}
//---------------------------------------------------------------------------
//	Terminate()
//	Терминиране на текущата задача
//
void Terminate(void)
{
  runtask->state = TS_STOP;
  Switch();
}
//---------------------------------------------------------------------------
//	Suspend()
//	Блокиране на текущата задача
//
void Suspend(void)
{
  runtask->state = TS_BLOCK;
  Switch();
}
//---------------------------------------------------------------------------
//	Suspend()
//	Блокиране на текущата задача
//
//  Входни аргументи:
//		task 		- идентификатор на задачата
//
void Suspend(TASK* task)
{
  if(task->state == TS_RUN || task->state == TS_READY)
  {
	task->state = TS_BLOCK;
  }
}
//---------------------------------------------------------------------------
//	Resume()
//	Разблокиране на задача и включването и в опашката на диспечера
//
//  Входни аргументи:
//		task 		- идентификатор на задачата
//
void Resume(TASK* task)
{
  if(task->state == TS_BLOCK)
  {
	task->state = TS_READY;
	InsertInReadyQ(task);
  }
}
//---------------------------------------------------------------------------
//  InsertInReadyQ()
//  Включва задачата task в края на опашката на диспечера
//
//  Входен аргумент: task - идентификатор на задачата
//
void InsertInReadyQ(TASK* task)
{
  if(task->state == TS_READY)
  {
	sysReadyQ.push(task);
  }
}
//---------------------------------------------------------------------------
//  RemoveFirstFromReadyQ()
//  Изключва първата задача от началото на опашката на диспечера
//
//  Връщана стойност: идентификатор на задачата
//
TASK* RemoveFirstFromReadyQ(void)
{
  TASK* temp = NULL;

  while(!sysReadyQ.empty())
  {
	TASK* qi = sysReadyQ.front();
	sysReadyQ.pop();
	if(qi->state == TS_READY)
	{
	  temp = qi;
	  break;
	}
  }

  return temp;
}
//---------------------------------------------------------------------------
//  RemoveTaskFromReadyQ()
//  Изключва задача от опашката на диспечера
//
//  Входен аргумент: task - идентификатор на задачата
//
void RemoveTaskFromReadyQ(TASK* task)
{
  queue < TASK* > bufQ;				// буфер на системната опашка на диспечера

  // трасиране на системната опашка на диспечера
  // за изключване от нея на задачата task
  while(!sysReadyQ.empty())
  {
	TASK* qi = sysReadyQ.front();
	sysReadyQ.pop();

	if(qi != task)
	  bufQ.push(qi);
  }

  // възстановяване на системната опашка на диспечера
  // без задачата task
  while(!bufQ.empty())
  {
	TASK* qi = bufQ.front();
	bufQ.pop();
	sysReadyQ.push(qi);
  }
}
//---------------------------------------------------------------------------

