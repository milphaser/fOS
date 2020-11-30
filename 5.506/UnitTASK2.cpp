//---------------------------------------------------------------------------
// �������� ������ TASK2
//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "UnitTASK2.h"
#include "UnitFormMain.h"
#include "UnitCSP.h"
#include "Defs.h"
//---------------------------------------------------------------------------
extern CHAN*	chanPQ;				// ����� ����� P � Q
extern CHAN*	chanQC;				// ����� ����� Q � C
//---------------------------------------------------------------------------
void WINAPI Task2(PVOID pvParam)
{
  TASK_MSG msg_data;
  msg_data.id = 0;
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
	RECV(chanPQ, &msg);
	memcpy(&msg_data, &msg.data, msg.len);

	formMain->stTask2->Caption = msg_data.intCounter;
	formMain->pbarTask2->Position = msg_data.intCounter % formMain->pbarTask2->Max;

	// ������������ �� �����������
	SEND(chanQC, &msg);

	if(!strcmp(msg_data.cmd, CMD_STOP))
	{
	  STOP;				// ����������� �� ��������
	}

	Switch();			// ������������ �� ���������
  }
}
//---------------------------------------------------------------------------

