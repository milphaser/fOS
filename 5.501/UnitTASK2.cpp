//---------------------------------------------------------------------------
// �������� ������ TASK3
//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "UnitTASK2.h"
#include "UnitFormMain.h"
#include "UnitCSP.h"
//---------------------------------------------------------------------------
extern CHAN*	chanC;			// ����� ����� P � Q
//---------------------------------------------------------------------------
void WINAPI Task2(PVOID pvParam)
{
  int intCounter2 = 0;
  formMain->stTask2->Caption = intCounter2;
  formMain->pbarTask2->Position = intCounter2;

  CHAN_MSG msg;
  msg.len = sizeof(intCounter2);

  while(true)
  {
	// ������� �����
    // ���������� �� ���������
	RECV(chanC, &msg);
	memcpy(&intCounter2, &msg.data, msg.len);

	formMain->stTask2->Caption = intCounter2;
	formMain->pbarTask2->Position = intCounter2;

	if(intCounter2 == LIMIT)
	{
	  STOP;				// ����������� �� ��������
	}

	Switch();			// ������������ �� ���������
  }
}
//---------------------------------------------------------------------------

