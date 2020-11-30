//---------------------------------------------------------------------------
// �������� ������ TASK1/P1
//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "UnitTASK1.h"
#include "UnitFormMain.h"
#include "UnitCSP.h"
#include "Defs.h"
//---------------------------------------------------------------------------
extern CHAN* 	chanP1Q;			// ����� ����� P1 � Q
//---------------------------------------------------------------------------
void WINAPI Task1(PVOID pvParam)
{
  TASK_MSG msg_data;
  msg_data.id = 1;					// Task1 Id
  msg_data.intCounter = 0;
  strcpy(msg_data.cmd, CMD_EMPTY);

  formMain->stTask1->Caption = msg_data.intCounter;
  formMain->pbarTask1->Position = msg_data.intCounter;

  CHAN_MSG msg;
  msg.len = sizeof(msg_data);

  while(true)
  {
	// ������� ����� �� ��������

	// ���������� �� ���������
	msg_data.intCounter++;
	if(formMain->boolTask1Stop)
	  strcpy(msg_data.cmd, CMD_STOP);

	formMain->stTask1->Caption = msg_data.intCounter;
	formMain->pbarTask1->Position = msg_data.intCounter % formMain->pbarTask1->Max;

	memcpy(&msg.data, &msg_data, msg.len);
	SEND(chanP1Q, &msg);

	Sleep(DELAY);		// ��������� �� ������������

	if(!strcmp(msg_data.cmd, CMD_STOP))
	{
	  STOP;				// ����������� �� ��������
	}

	Switch();			// ������������ �� ���������
  }
}
//---------------------------------------------------------------------------

