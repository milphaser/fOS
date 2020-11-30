//---------------------------------------------------------------------------
// Приложна задача TASK3
//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "UnitTASK3.h"
#include "UnitFormMain.h"
#include "UnitCSP.h"
#include "Defs.h"
//---------------------------------------------------------------------------
extern CHAN*	chanP1Q;			// канал между P1 и Q
extern CHAN*	chanP2Q;			// канал между P2 и Q
//---------------------------------------------------------------------------
void WINAPI Task3(PVOID pvParam)
{
  TASK_MSG msg_data;
  msg_data.id = 0;
  msg_data.intCounter = 0;
  strcpy(msg_data.cmd, "");

  formMain->stTask31->Caption = msg_data.intCounter;
  formMain->stTask32->Caption = msg_data.intCounter;
  formMain->pbarTask31->Position = msg_data.intCounter;
  formMain->pbarTask32->Position = msg_data.intCounter;

  CHAN_MSG msg;
  msg.len = sizeof(msg_data);

  bool boolTask1Stop = false, boolTask2Stop = false;

  while(true)
  {
	// РАБОТЕН ЦИКЪЛ НА ЗАДАЧАТА

	// Получаване на съобщение чрез оператор за избор
	while(ALT(chanP1Q, chanP2Q, &msg) < 0)
	{
	  Switch();			// превключване на контекста в очакване на съобщение
	}
	memcpy(&msg_data, &msg.data, msg.len);

	if(msg_data.id == 1)		// Task1 Id
	{
	  formMain->stTask31->Caption = msg_data.intCounter;
	  formMain->pbarTask31->Position = msg_data.intCounter % formMain->pbarTask31->Max;
	  if(!strcmp(msg_data.cmd, CMD_STOP))
		boolTask1Stop = true;
	}
	else if(msg_data.id == 2)	// Task2 Id
	{
	  formMain->stTask32->Caption = msg_data.intCounter;
	  formMain->pbarTask32->Position = msg_data.intCounter % formMain->pbarTask32->Max;
	  if(!strcmp(msg_data.cmd, CMD_STOP))
		boolTask2Stop = true;
	}

	if(boolTask1Stop && boolTask2Stop)
	{
	  STOP;				// терминиране на задачата
	}

	Switch();			// превключване на контекста
  }
}
//---------------------------------------------------------------------------

