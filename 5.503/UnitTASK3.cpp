//---------------------------------------------------------------------------
// �������� ������ TASK3
//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "UnitTASK3.h"
#include "UnitFormMain.h"
#include "UnitCSP.h"
//---------------------------------------------------------------------------
extern CHAN*	chanQC;			// ����� ����� Q � C
//---------------------------------------------------------------------------
void WINAPI Task3(PVOID pvParam)
{
  int intCounter = 0;
  formMain->stTask3->Caption = intCounter;
  formMain->pbarTask3->Position = intCounter;

  CHAN_MSG msg;
  msg.len = sizeof(intCounter);

  while(true)
  {
	// ������� �����
	// ���������� �� ���������
	RECV(chanQC, &msg);
	memcpy(&intCounter, &msg.data, msg.len);

	formMain->stTask3->Caption = intCounter;
	formMain->pbarTask3->Position = intCounter;

	if(intCounter == LIMIT)
	{
	  STOP;				// ����������� �� ��������
	}

	Switch();			// ������������ �� ���������
  }
}
//---------------------------------------------------------------------------

