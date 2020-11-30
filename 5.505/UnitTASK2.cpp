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
extern CHAN*	chanP2Q;			// ����� ����� P2 � Q
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

	// ���������� �� ���������
	msg_data.intCounter++;
	if(formMain->boolTask2Stop)
	  strcpy(msg_data.cmd, CMD_STOP);

	formMain->stTask2->Caption = msg_data.intCounter;
	formMain->pbarTask2->Position = msg_data.intCounter % formMain->pbarTask2->Max;

	memcpy(&msg.data, &msg_data, msg.len);
	SEND(chanP2Q, &msg);

	Sleep(DELAY);		// ��������� �� ������������

	if(!strcmp(msg_data.cmd, CMD_STOP))
	{
	  STOP;				// ����������� �� ��������
	}

	Switch();			// ������������ �� ���������
  }
}
//---------------------------------------------------------------------------

