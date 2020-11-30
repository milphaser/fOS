//---------------------------------------------------------------------------
// �������� ������ TASK2
//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "UnitTASK2.h"
#include "UnitFormMain.h"
#include "UnitCSP.h"
//---------------------------------------------------------------------------
extern CHAN*	chanPQ1;			// ����� ����� P � Q1
//---------------------------------------------------------------------------
void WINAPI Task2(PVOID pvParam)
{
  int intCounter = 0;
  formMain->stTask2->Caption = intCounter;
  formMain->pbarTask2->Position = intCounter;

  CHAN_MSG msg;
  msg.len = sizeof(intCounter);

  while(true)
  {
	// ������� �����
	// ���������� �� ���������
	RECV(chanPQ1, &msg);
	memcpy(&intCounter, &msg.data, msg.len);

	formMain->stTask2->Caption = intCounter;
	formMain->pbarTask2->Position = intCounter;

	if(intCounter == LIMIT)
	{
	  STOP;				// ����������� �� ��������
	}

	Switch();			// ������������ �� ���������
  }
}
//---------------------------------------------------------------------------

