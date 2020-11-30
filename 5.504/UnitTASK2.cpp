//---------------------------------------------------------------------------
// Приложна задача TASK2
//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "UnitTASK2.h"
#include "UnitFormMain.h"
#include "UnitCSP.h"
#include "Defs.h"
//---------------------------------------------------------------------------
extern CHAN*	chanP2Q;			// канал между P2 и Q
//---------------------------------------------------------------------------
void WINAPI Task2(PVOID pvParam)
{
  TASK_MSG msg_data;
  msg_data.id = 2;					// Task2 Id
  msg_data.intCounter = 0;
  formMain->stTask2->Caption = msg_data.intCounter;
  formMain->pbarTask2->Position = msg_data.intCounter;

  CHAN_MSG msg;
  msg.len = sizeof(msg_data);

  while(true)
  {
	// Работен цикъл
	// Генериране на съобщение
	msg_data.intCounter++;
	formMain->stTask2->Caption = msg_data.intCounter;
	formMain->pbarTask2->Position = msg_data.intCounter;

	memcpy(&msg.data, &msg_data, msg.len);
	SEND(chanP2Q, &msg);

	Sleep(DELAY);		// изчакване за визуализация

	if(msg_data.intCounter == LIMIT)
	{
	  STOP;				// терминиране на задачата
	}

	Switch();			// превключване на контекста
  }
}
//---------------------------------------------------------------------------

