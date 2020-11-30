/////////////////////////////////////////////////////////////////////////////
//  fiberOS/CSP
//  ver. 5.504
//
//  Многозадачна система с диспечеризация "без изтласкване" (non-preemptive)
//  Развитие на MICROS за платформа Win32/x86/Protected Mode
//
/////////////////////////////////////////////////////////////////////////////
//
//	ФУНКЦИОНАЛНОСТ
//
//	Тема: Изследване различни варианти на оператора за избор
//	Конфигурация "Производител - потребител" от типа 2:1 с недетерминизъм
//	Q - недетерминиран процес с 2 входни канала
//	ALT - оператор за недетерминиран избор (два входни канала)
//
//  - Използване на влакна (fibers)
//	- Опашката на диспечера е изпълнена чрез stl::queue
//	- Добавени са състояния на задачите
//	- Управление на задачите чрез функциите Suspend()/Resume()
//	- Визуализация на системното състояние
//	- Обработка чрез недетерминиран избор
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
TASK*			taskP1;			// задача "производител1"
TASK*			taskP2;			// задача "производител2"
TASK*			taskQ;			// задача "потребител"

//  КОМУНИКАЦИОННИ КАНАЛИ ///////////////////////////////////////////////////
CHAN* 			chanP1Q;		// канал между P1 и Q
CHAN* 			chanP2Q;		// канал между P2 и Q

//  СИСТЕМЕН ПРОЦЕС /////////////////////////////////////////////////////////
//
__fastcall TformMain::TformMain(TComponent* Owner)
	: TForm(Owner)
{
  pbarTask1->Max = LIMIT;
  pbarTask2->Max = LIMIT;
  pbarTask31->Max = LIMIT;
  pbarTask32->Max = LIMIT;

  buttonSuspend1->Enabled = true;
  buttonSuspend2->Enabled = true;
  buttonSuspend3->Enabled = true;
  buttonResume1->Enabled = false;
  buttonResume2->Enabled = false;
  buttonResume3->Enabled = false;

  Randomize();		// рандомизация, заради оператора ALT

  sysContext = ConvertThreadToFiber(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TformMain::buttonStartClick(TObject *Sender)
{
  buttonStart->Enabled = false;

  sys_timer = 0;
  sys_tasks_num = 0;

  // Създаване на задачите и включването им в системата
  taskP1 = CreateTask("Task P1", Task1);
  taskP2 = CreateTask("Task P2", Task2);
  taskQ  = CreateTask("Task Q", Task3);

  // Създаване на каналите
  chanP1Q = CreateChan("Chan P1Q", taskP1, taskQ);
  chanP2Q = CreateChan("Chan P2Q", taskP2, taskQ);

  while(sys_tasks_num > 0)
  {
	// РАБОТЕН ЦИКЪЛ НА СИСТЕМНИЯ ПРОЦЕС

	stSysTimer->Caption = sys_timer;
	stSysTasksNum->Caption = sys_tasks_num;
	stSysReadyTasksNum->Caption = sysReadyQ.size();

	Dispatcher();		// преход към диспечера

	Application->ProcessMessages();

	if(taskP1->state == TS_STOP &&
	   taskP2->state == TS_STOP &&
	   taskQ->state == TS_STOP)
	  break;			// изход, ако всички задачи са терминарани
  }

  // Премахване на каналите от системата
  delete chanP1Q;
  delete chanP2Q;

  // Премахване на задачите от системата
  DeleteTask(taskP1);
  DeleteTask(taskP2);
  DeleteTask(taskQ);

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
	Suspend(taskP1);
	buttonSuspend1->Enabled = false;
	buttonResume1->Enabled = true;
  }
  else if(Sender == buttonSuspend2)
  {
	Suspend(taskP2);
	buttonSuspend2->Enabled = false;
	buttonResume2->Enabled = true;
  }
  else if(Sender == buttonSuspend3)
  {
	Suspend(taskQ);
	buttonSuspend3->Enabled = false;
	buttonResume3->Enabled = true;
  }
}
//---------------------------------------------------------------------------
void __fastcall TformMain::buttonResume1Click(TObject *Sender)
{
  if(Sender == buttonResume1)
  {
	Resume(taskP1);
	buttonSuspend1->Enabled = true;
	buttonResume1->Enabled = false;
  }
  else if(Sender == buttonResume2)
  {
	Resume(taskP2);
	buttonSuspend2->Enabled = true;
	buttonResume2->Enabled = false;
  }
  else if(Sender == buttonResume3)
  {
	Resume(taskQ);
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

