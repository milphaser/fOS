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
extern CHAN*	chanC;			// ����� ����� P � Q
//---------------------------------------------------------------------------
void WINAPI Task1(PVOID pvParam)
{
  int intCounter1 = 0;
  formMain->stTask1->Caption = intCounter1;
  formMain->pbarTask1->Position = intCounter1;

  CHAN_MSG msg;
  msg.len = sizeof(intCounter1);

  while(true)
  {
	// ������� �����
	// ���������� �� ���������
	intCounter1++;
	formMain->stTask1->Caption = intCounter1;
	formMain->pbarTask1->Position = intCounter1;

	memcpy(&msg.data, &intCounter1, msg.len);
	SEND(chanC, &msg);

	Sleep(DELAY);		// ��������� �� ������������

	if(intCounter1 == LIMIT)
	{
	  STOP;				// ����������� �� ��������
	}

	Switch();			// ������������ �� ���������
  }
}
//---------------------------------------------------------------------------

