//---------------------------------------------------------------------------
// Приложна задача TASK1
//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "UnitTASK1.h"
#include "UnitFormMain.h"
#include "UnitCSP.h"
//---------------------------------------------------------------------------
extern CHAN*	chanPQ1;			// канал между P и Q1
extern CHAN*	chanPQ2;			// канал между P и Q2
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
	// Работен цикъл
	// Генериране на съобщение
	intCounter++;
	formMain->stTask1->Caption = intCounter;
	formMain->pbarTask1->Position = intCounter;

	memcpy(&msg.data, &intCounter, msg.len);
	SEND(chanPQ1, &msg);
	SEND(chanPQ2, &msg);

	Sleep(DELAY);		// изчакване за визуализация

	if(intCounter == LIMIT)
	{
	  STOP;				// терминиране на задачата
	}

	Switch();			// превключване на контекста
  }
}
//---------------------------------------------------------------------------

