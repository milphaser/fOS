/////////////////////////////////////////////////////////////////////////////
//  fiberOS/CSP
//  ver. 5.504
//
//  ������������ ������� � �������������� "��� �����������" (non-preemptive)
//  �������� �� MICROS �� ��������� Win32/x86/Protected Mode
//
/////////////////////////////////////////////////////////////////////////////
//
//	��������������
//
//	����: ���������� �������� �������� �� ��������� �� �����
//	������������ "������������ - ����������" �� ���� 2:1 � ��������������
//	Q - �������������� ������ � 2 ������ ������
//	ALT - �������� �� �������������� ����� (��� ������ ������)
//
//  - ���������� �� ������ (fibers)
//	- �������� �� ��������� � ��������� ���� stl::queue
//	- �������� �� ��������� �� ��������
//	- ���������� �� �������� ���� ��������� Suspend()/Resume()
//	- ������������ �� ���������� ���������
//	- ��������� ���� �������������� �����
//
/////////////////////////////////////////////////////////////////////////////
//  ��������� �� ��������:
//  �����������, �. �������� ������������ �� ���������� �������������.
//  �����, �������, 1993, ISBN 954-03-0319-2.
//  (���. 102-103, ���. 2.9)
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
//  �������� ���������� /////////////////////////////////////////////////////
TASK*			runtask;  		// ������� ������
queue < TASK* > sysReadyQ;		// ������ �� �������� ������ (�� ���������)
PTASKCONTEXT	sysContext;		// �������� ��������
unsigned		sys_timer;		// �������� ������
unsigned		sys_tasks_num;	// ��� ���� �� �������� � ���������

//  �������� ������ /////////////////////////////////////////////////////////
TASK*			taskP1;			// ������ "������������1"
TASK*			taskP2;			// ������ "������������2"
TASK*			taskQ;			// ������ "����������"

//  �������������� ������ ///////////////////////////////////////////////////
CHAN* 			chanP1Q;		// ����� ����� P1 � Q
CHAN* 			chanP2Q;		// ����� ����� P2 � Q

//  �������� ������ /////////////////////////////////////////////////////////
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

  Randomize();		// ������������, ������ ��������� ALT

  sysContext = ConvertThreadToFiber(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TformMain::buttonStartClick(TObject *Sender)
{
  buttonStart->Enabled = false;

  sys_timer = 0;
  sys_tasks_num = 0;

  // ��������� �� �������� � ����������� �� � ���������
  taskP1 = CreateTask("Task P1", Task1);
  taskP2 = CreateTask("Task P2", Task2);
  taskQ  = CreateTask("Task Q", Task3);

  // ��������� �� ��������
  chanP1Q = CreateChan("Chan P1Q", taskP1, taskQ);
  chanP2Q = CreateChan("Chan P2Q", taskP2, taskQ);

  while(sys_tasks_num > 0)
  {
	// ������� ����� �� ��������� ������

	stSysTimer->Caption = sys_timer;
	stSysTasksNum->Caption = sys_tasks_num;
	stSysReadyTasksNum->Caption = sysReadyQ.size();

	Dispatcher();		// ������ ��� ���������

	Application->ProcessMessages();

	if(taskP1->state == TS_STOP &&
	   taskP2->state == TS_STOP &&
	   taskQ->state == TS_STOP)
	  break;			// �����, ��� ������ ������ �� �����������
  }

  // ���������� �� �������� �� ���������
  delete chanP1Q;
  delete chanP2Q;

  // ���������� �� �������� �� ���������
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
//  �������� ���������
/////////////////////////////////////////////////////////////////////////////
//  CreateTask()
//  ��������� �� �������� ������ �� ��� ������ (fiber)
//
//  ������ ���������:
//		name 		- ��� �� ��������
//		pfMain		- ������ ������� �� ��������
//		pvPar		- ��������� �� �������� �������
//		dwStackSize - ������ �� ����� (�� ������������ � 1 MB)
//  ������� ��������: ������������� �� ����������� ������
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
//  ������������ �� �������� ������
//
//  ������ ���������:
//		task 		- ������������� �� ��������
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
//  ������������ �� ��������� �� ��������� ������ ��� ��������� ��������
//
void Switch(void)
{
  if(runtask)
  {
	if(runtask->state == TS_RUN)
	{	// �������� �� ��������� ������ � �������� �� ���������
	  runtask->state = TS_READY;
	  InsertInReadyQ(runtask);
	}
  }

  // ������������ ��� ��������� ��������
  SwitchToFiber(sysContext);
}
//---------------------------------------------------------------------------
//  Dispatcher()
//  �������� �� ��������
//
void Dispatcher(void)
{
  sys_timer++;

  // ��������� �� ���������� ������ ������ �� �������� �� ���������
  runtask = RemoveFirstFromReadyQ();

  if(runtask)
  { // ������������ ��� ��������� �� ��������
	runtask->state = TS_RUN;			// switch to RUN state
	SwitchToFiber(runtask->context);	// switch to task context
  }
}
//---------------------------------------------------------------------------
//	Terminate()
//	����������� �� �������� ������
//
void Terminate(void)
{
  runtask->state = TS_STOP;
  Switch();
}
//---------------------------------------------------------------------------
//	Suspend()
//	��������� �� �������� ������
//
void Suspend(void)
{
  runtask->state = TS_BLOCK;
  Switch();
}
//---------------------------------------------------------------------------
//	Suspend()
//	��������� �� �������� ������
//
//  ������ ���������:
//		task 		- ������������� �� ��������
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
//	������������ �� ������ � ����������� � � �������� �� ���������
//
//  ������ ���������:
//		task 		- ������������� �� ��������
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
//  ������� �������� task � ���� �� �������� �� ���������
//
//  ������ ��������: task - ������������� �� ��������
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
//  �������� ������� ������ �� �������� �� �������� �� ���������
//
//  ������� ��������: ������������� �� ��������
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
//  �������� ������ �� �������� �� ���������
//
//  ������ ��������: task - ������������� �� ��������
//
void RemoveTaskFromReadyQ(TASK* task)
{
  queue < TASK* > bufQ;				// ����� �� ���������� ������ �� ���������

  // ��������� �� ���������� ������ �� ���������
  // �� ���������� �� ��� �� �������� task
  while(!sysReadyQ.empty())
  {
	TASK* qi = sysReadyQ.front();
	sysReadyQ.pop();

	if(qi != task)
	  bufQ.push(qi);
  }

  // �������������� �� ���������� ������ �� ���������
  // ��� �������� task
  while(!bufQ.empty())
  {
	TASK* qi = bufQ.front();
	bufQ.pop();
	sysReadyQ.push(qi);
  }
}
//---------------------------------------------------------------------------

