//---------------------------------------------------------------------------
// �������� ������ TASK4/Q
//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "UnitTASK4.h"
#include "UnitFormMain.h"
#include "UnitCSP.h"
#include "Defs.h"
//---------------------------------------------------------------------------
extern CHAN* 	chanP1Q;			// ����� ����� P1 � Q
extern CHAN* 	chanP2Q;			// ����� ����� P2 � Q
extern CHAN* 	chanP3Q;			// ����� ����� P3 � Q
//---------------------------------------------------------------------------
void WINAPI Task4(PVOID pvParam)
{
  TASK_MSG msg_data;
  msg_data.id = 0;
  msg_data.intCounter = 0;
  strcpy(msg_data.cmd, "");

  formMain->stTask41->Caption = msg_data.intCounter;
  formMain->stTask42->Caption = msg_data.intCounter;
  formMain->stTask43->Caption = msg_data.intCounter;
  formMain->pbarTask41->Position = msg_data.intCounter;
  formMain->pbarTask42->Position = msg_data.intCounter;
  formMain->pbarTask43->Position = msg_data.intCounter;

  CHAN_MSG msg;
  msg.len = sizeof(msg_data);

  bool boolTask1Stop = false, boolTask2Stop = false, boolTask3Stop = false;

  // ��������� ������� �� �������� ������
  vector <CHAN *> vChanIn;
  vChanIn.push_back(chanP1Q);
  vChanIn.push_back(chanP2Q);
  vChanIn.push_back(chanP3Q);

  while(true)
  {
	// ������� ����� �� ��������

	// ���������� �� ��������� ���� N-������� �������� �� ����� ALT
	while(ALT(vChanIn, &msg) < 0)
	{
	  Switch();			// ������������ �� ��������� � �������� �� ���������
	}
	memcpy(&msg_data, &msg.data, msg.len);

	if(msg_data.id == 1)		// Task1 Id
	{
	  formMain->stTask41->Caption = msg_data.intCounter;
	  formMain->pbarTask41->Position = msg_data.intCounter % formMain->pbarTask41->Max;
	  if(!strcmp(msg_data.cmd, CMD_STOP))
		boolTask1Stop = true;
	}
	else if(msg_data.id == 2)		// Task2 Id
	{
	  formMain->stTask42->Caption = msg_data.intCounter;
	  formMain->pbarTask42->Position = msg_data.intCounter % formMain->pbarTask42->Max;
	  if(!strcmp(msg_data.cmd, CMD_STOP))
		boolTask2Stop = true;
	}
	else if(msg_data.id == 3)		// Task3 Id
	{
	  formMain->stTask43->Caption = msg_data.intCounter;
	  formMain->pbarTask43->Position = msg_data.intCounter % formMain->pbarTask43->Max;
	  if(!strcmp(msg_data.cmd, CMD_STOP))
		boolTask3Stop = true;
	}

	if(boolTask1Stop && boolTask2Stop && boolTask3Stop)
	{
	  STOP;				// ����������� �� ��������
	}

	Switch();			// ������������ �� ���������
  }
}
//---------------------------------------------------------------------------

