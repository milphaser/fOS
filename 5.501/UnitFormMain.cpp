/////////////////////////////////////////////////////////////////////////////
//  fiberOS/CSP
//  ver. 5.501
//
//  ������������ ������� � �������������� "��� �����������" (non-preemptive)
//  �������� �� MICROS �� ��������� Win32/x86/Protected Mode
//
/////////////////////////////////////////////////////////////////////////////
//
//	��������������
//
//	����: ������ "������������ - ����������" �� ���� 1:1
//
//  - ���������� �� ������ (fibers)
//	- �������� �� ��������� � ��������� ���� stl::queue
//	- �������� �� ��������� �� ��������
//	- ���������� �� �������� ���� ��������� Suspend()/Resume()
//	- ������������ �� ���������� ���������
//
/////////////////////////////////////////////////////////////////////////////
//
//	������ ������: CSP ���������
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
TASK*			taskP;			// ������ "������������"
TASK*			taskQ;			// ������ "����������"

//  �������������� ������ ///////////////////////////////////////////////////
CHAN* 			chanC;			// ����� ����� P � Q

//  �������� ������ /////////////////////////////////////////////////////////
//
__fastcall TformMain::TformMain(TComponent* Owner)
	: TForm(Owner)
{
  pbarTask1->Max = LIMIT;
  pbarTask2->Max = LIMIT;

  buttonSuspend1->Enabled = true;
  buttonSuspend2->Enabled = true;
  buttonResume1->Enabled = false;
  buttonResume2->Enabled = false;

  sysContext = ConvertThreadToFiber(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TformMain::buttonStartClick(TObject *Sender)
{
  buttonStart->Enabled = false;

  sys_timer = 0;
  sys_tasks_num = 0;

  // ��������� �� �������� � ����������� �� � ���������
  taskP = CreateTask("Task P", Task1);
  taskQ = CreateTask("Task Q", Task2);

  // ��������� �� ��������
  chanC = CreateChan("c", taskP, taskQ);

  while(sys_tasks_num > 0)
  {
	// ������� ����� �� ��������� ������

	stSysTimer->Caption = sys_timer;
	stSysTasksNum->Caption = sys_tasks_num;
	stSysReadyTasksNum->Caption = sysReadyQ.size();

	Dispatcher();		// ������ ��� ���������

	Application->ProcessMessages();

	if(taskP->state == TS_STOP && taskQ->state == TS_STOP)
	  break;			// �����, ��� ������ ������ �� �����������
  }

  // ���������� �� �������� �� ���������
  delete chanC;

  // ���������� �� �������� �� ���������
  DeleteTask(taskP);
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
	Suspend(taskP);
	buttonSuspend1->Enabled = false;
	buttonResume1->Enabled = true;
  }
  else if(Sender == buttonSuspend2)
  {
	Suspend(taskQ);
	buttonSuspend2->Enabled = false;
	buttonResume2->Enabled = true;
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
	Resume(taskQ);
	buttonSuspend2->Enabled = true;
	buttonResume2->Enabled = false;
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

