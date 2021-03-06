//---------------------------------------------------------------------------
// �������� ������ TASK2/Q1
//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "UnitTASK2.h"
#include "UnitFormMain.h"
#include "UnitCSP.h"
#include "Defs.h"
//---------------------------------------------------------------------------
extern CHAN* 	chanPQ1;			// ����� ����� P � Q1
extern CHAN* 	chanQ1C;			// ����� ����� Q1 � C
//---------------------------------------------------------------------------
void WINAPI Task2(PVOID pvParam)
{
  TASK_MSG msg_data;
  msg_data.id = 2;					// Task2 Id
  msg_data.intCounter = 0;
  strcpy(msg_data.cmd, CMD_EMPTY);

  formMain->stTask2->Caption = msg_data.intCounter;
  formMain->pbarTask2->Position = msg_data.intCounter;

  CHAN_MSG msg;
  msg.len = sizeof(msg_data);

  while(true)
  {
	// ������� ����� �� ��������

	// ���������� �� �����������
	RECV(chanPQ1, &msg);
	memcpy(&msg_data, &msg.data, msg.len);

	formMain->stTask2->Caption = msg_data.intCounter;
	formMain->pbarTask2->Position = msg_data.intCounter % formMain->pbarTask2->Max;

	// �������� � ������������ �� �����������
	msg_data.id = 2;				// Task2 Id
	memcpy(&msg.data, &msg_data, msg.len);
	SEND(chanQ1C, &msg);

	if(!strcmp(msg_data.cmd, CMD_STOP))
	{
	  STOP;				// ����������� �� ��������
	}

	Switch();			// ������������ �� ���������
  }
}
//---------------------------------------------------------------------------

