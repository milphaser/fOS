//---------------------------------------------------------------------------
// Приложна задача TASK4/C
//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "UnitTASK4.h"
#include "UnitFormMain.h"
#include "UnitCSP.h"
#include "Defs.h"
//---------------------------------------------------------------------------
extern CHAN* 	chanQ1C;			// канал между Q1 и C
extern CHAN*	chanQ2C;			// канал между Q2 и C
//---------------------------------------------------------------------------
void WINAPI Task4(PVOID pvParam)
{
  TASK_MSG msg_data;
  msg_data.id = 0;
  msg_data.intCounter = 0;
  strcpy(msg_data.cmd, "");

  formMain->stTask41->Caption = msg_data.intCounter;
  formMain->stTask42->Caption = msg_data.intCounter;
  formMain->pbarTask41->Position = msg_data.intCounter;
  formMain->pbarTask42->Position = msg_data.intCounter;

  CHAN_MSG msg;
  msg.len = sizeof(msg_data);

  bool boolTask2Stop = false, boolTask3Stop = false;

  while(true)
  {
	// РАБОТЕН ЦИКЪЛ НА ЗАДАЧАТА

	// Получаване на съобщение чрез оператор за избор
	while(ALT(chanQ1C, chanQ2C, &msg) < 0)
	{
	  Switch();			// превключване на контекста в очакване на съобщение
	}
	memcpy(&msg_data, &msg.data, msg.len);

	if(msg_data.id == 2)		// Task2 Id
	{
	  formMain->stTask41->Caption = msg_data.intCounter;
	  formMain->pbarTask41->Position = msg_data.intCounter % formMain->pbarTask41->Max;
	  if(!strcmp(msg_data.cmd, CMD_STOP))
		boolTask2Stop = true;
	}
	else if(msg_data.id == 3)	// Task3 Id
	{
	  formMain->stTask42->Caption = msg_data.intCounter;
	  formMain->pbarTask42->Position = msg_data.intCounter % formMain->pbarTask42->Max;
	  if(!strcmp(msg_data.cmd, CMD_STOP))
		boolTask3Stop = true;
	}

	if(boolTask2Stop && boolTask3Stop)
	{
	  STOP;				// терминиране на задачата
	}

	Switch();			// превключване на контекста
  }
}
//---------------------------------------------------------------------------

