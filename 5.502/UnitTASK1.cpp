//---------------------------------------------------------------------------
// �������� ������ TASK1
//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "UnitTASK1.h"
#include "UnitFormMain.h"
#include "UnitCSP.h"
//---------------------------------------------------------------------------
extern CHAN*	chanPQ1;			// ����� ����� P � Q1
extern CHAN*	chanPQ2;			// ����� ����� P � Q2
//---------------------------------------------------------------------------
void WINAPI Task1(PVOID pvParam)
{
  int intCounter = 0;
  formMain->stTask1->Caption = intCounter;
  formMain->pbarTask1->Position = intCounter;

  CHAN_MSG msg;
  msg.len = sizeof(intCounter);

  while(true)
  {
	// ������� �����
	// ���������� �� ���������
	intCounter++;
	formMain->stTask1->Caption = intCounter;
	formMain->pbarTask1->Position = intCounter;

	memcpy(&msg.data, &intCounter, msg.len);
	SEND(chanPQ1, &msg);
	SEND(chanPQ2, &msg);

	Sleep(DELAY);		// ��������� �� ������������

	if(intCounter == LIMIT)
	{
	  STOP;				// ����������� �� ��������
	}

	Switch();			// ������������ �� ���������
  }
}
//---------------------------------------------------------------------------

