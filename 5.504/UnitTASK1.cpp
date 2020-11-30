//---------------------------------------------------------------------------
// Приложна задача TASK1
//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "UnitTASK1.h"
#include "UnitFormMain.h"
#include "UnitCSP.h"
#include "Defs.h"
//---------------------------------------------------------------------------
extern CHAN*	chanP1Q;			// канал между P1 и Q
//---------------------------------------------------------------------------
void WINAPI Task1(PVOID pvParam)
{
  TASK_MSG msg_data;
  msg_data.id = 1;					// Task1 Id
  msg_data.intCounter = 0;
  formMain->stTask1->Caption = msg_data.intCounter;
  formMain->pbarTask1->Position = msg_data.intCounter;

  CHAN_MSG msg;
  msg.len = sizeof(msg_data);

  while(true)
  {
	// Работен цикъл
	// Генериране на съобщение
	msg_data.intCounter++;
	formMain->stTask1->Caption = msg_data.intCounter;
	formMain->pbarTask1->Position = msg_data.intCounter;

	memcpy(&msg.data, &msg_data, msg.len);
	SEND(chanP1Q, &msg);

	Sleep(DELAY);		// изчакване за визуализация

	if(msg_data.intCounter == LIMIT)
	{
	  STOP;				// терминиране на задачата
	}

	Switch();			// превключване на контекста
  }
}
//---------------------------------------------------------------------------

