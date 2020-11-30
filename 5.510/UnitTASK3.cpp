//---------------------------------------------------------------------------
// Приложна задача TASK3/P3
//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "UnitTASK3.h"
#include "UnitFormMain.h"
#include "UnitCSP.h"
#include "Defs.h"
//---------------------------------------------------------------------------
extern CHAN* 	chanP3Q;			// канал между P3 и Q
//---------------------------------------------------------------------------
void WINAPI Task3(PVOID pvParam)
{
  TASK_MSG msg_data;
  msg_data.id = 3;					// Task3 Id
  msg_data.intCounter = 0;
  strcpy(msg_data.cmd, CMD_EMPTY);

  formMain->stTask3->Caption = msg_data.intCounter;
  formMain->pbarTask3->Position = msg_data.intCounter;

  CHAN_MSG msg;
  msg.len = sizeof(msg_data);

  while(true)
  {
	// РАБОТЕН ЦИКЪЛ НА ЗАДАЧАТА

	// Генериране на съобщение
	msg_data.intCounter++;
	if(formMain->boolTask3Stop)
	  strcpy(msg_data.cmd, CMD_STOP);

	formMain->stTask3->Caption = msg_data.intCounter;
	formMain->pbarTask3->Position = msg_data.intCounter % formMain->pbarTask3->Max;

	memcpy(&msg.data, &msg_data, msg.len);
	SEND(chanP3Q, &msg);

	Sleep(DELAY);		// изчакване за визуализация

	if(!strcmp(msg_data.cmd, CMD_STOP))
	{
	  STOP;				// терминиране на задачата
	}

	Switch();			// превключване на контекста
  }
}
//---------------------------------------------------------------------------

