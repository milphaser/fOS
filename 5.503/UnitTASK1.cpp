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
extern CHAN*	chanPQ;			// ����� ����� P � Q
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
	SEND(chanPQ, &msg);

	Sleep(DELAY);		// ��������� �� ������������

	if(intCounter == LIMIT)
	{
	  STOP;				// ����������� �� ��������
	}

	Switch();			// ������������ �� ���������
  }
}
//---------------------------------------------------------------------------

