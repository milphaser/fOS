//---------------------------------------------------------------------------
// �������� ������ TASK3/Q2
//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "UnitTASK3.h"
#include "UnitFormMain.h"
#include "UnitCSP.h"
#include "Defs.h"
//---------------------------------------------------------------------------
extern CHAN* 	chanPQ2;			// ����� ����� P � Q2
extern CHAN*	chanQ2C;			// ����� ����� Q2 � C
//---------------------------------------------------------------------------
void WINAPI Task3(PVOID pvParam)
{
  TASK_MSG msg_data;
  msg_data.id = 3;					// Task3 Id
  msg_data.intCounter = 0;
  strcpy(msg_data.cmd, CMD_EMPTY);

  formMain->stTask3->Caption = msg_data.intCounter;
  formMain->pbarTask3->Position = msg_data.intCounter;

  CHAN_MSG msg;
  msg.len = sizeof(msg_data);

  while(true)
  {
	// ������� ����� �� ��������

	// ���������� �� �����������
	RECV(chanPQ2, &msg);
	memcpy(&msg_data, &msg.data, msg.len);

	formMain->stTask3->Caption = msg_data.intCounter;
	formMain->pbarTask3->Position = msg_data.intCounter % formMain->pbarTask3->Max;

	// �������� � ������������ �� �����������
	msg_data.id = 3;				// Task3 Id
	memcpy(&msg.data, &msg_data, msg.len);
	SEND(chanQ2C, &msg);

	if(!strcmp(msg_data.cmd, CMD_STOP))
	{
	  STOP;				// ����������� �� ��������
	}

	Switch();			// ������������ �� ���������
  }
}
//---------------------------------------------------------------------------

