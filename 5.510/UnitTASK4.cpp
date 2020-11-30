//---------------------------------------------------------------------------
// Приложна задача TASK4/Q
//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "UnitTASK4.h"
#include "UnitFormMain.h"
#include "UnitCSP.h"
#include "Defs.h"
//---------------------------------------------------------------------------
extern CHAN* 	chanP1Q;			// канал между P1 и Q
extern CHAN* 	chanP2Q;			// канал между P2 и Q
extern CHAN* 	chanP3Q;			// канал между P3 и Q
//---------------------------------------------------------------------------
void WINAPI Task4(PVOID pvParam)
{
  TASK_MSG msg_data;
  msg_data.id = 0;
  msg_data.intCounter = 0;
  strcpy(msg_data.cmd, "");

  formMain->stTask41->Caption = msg_data.intCounter;
  formMain->stTask42->Caption = msg_data.intCounter;
  formMain->stTask43->Caption = msg_data.intCounter;
  formMain->pbarTask41->Position = msg_data.intCounter;
  formMain->pbarTask42->Position = msg_data.intCounter;
  formMain->pbarTask43->Position = msg_data.intCounter;

  CHAN_MSG msg;
  msg.len = sizeof(msg_data);

  bool boolTask1Stop = false, boolTask2Stop = false, boolTask3Stop = false;

  // ФОРМИРАНЕ СПИСЪКА НА ВХОДНИТЕ КАНАЛИ
  vector <CHAN *> vChanIn;
  vChanIn.push_back(chanP1Q);
  vChanIn.push_back(chanP2Q);
  vChanIn.push_back(chanP3Q);

  while(true)
  {
	// РАБОТЕН ЦИКЪЛ НА ЗАДАЧАТА

	// Получаване на съобщение чрез N-канален оператор за избор ALT
	while(ALT(vChanIn, &msg) < 0)
	{
	  Switch();			// превключване на контекста в очакване на съобщение
	}
	memcpy(&msg_data, &msg.data, msg.len);

	if(msg_data.id == 1)		// Task1 Id
	{
	  formMain->stTask41->Caption = msg_data.intCounter;
	  formMain->pbarTask41->Position = msg_data.intCounter % formMain->pbarTask41->Max;
	  if(!strcmp(msg_data.cmd, CMD_STOP))
		boolTask1Stop = true;
	}
	else if(msg_data.id == 2)		// Task2 Id
	{
	  formMain->stTask42->Caption = msg_data.intCounter;
	  formMain->pbarTask42->Position = msg_data.intCounter % formMain->pbarTask42->Max;
	  if(!strcmp(msg_data.cmd, CMD_STOP))
		boolTask2Stop = true;
	}
	else if(msg_data.id == 3)		// Task3 Id
	{
	  formMain->stTask43->Caption = msg_data.intCounter;
	  formMain->pbarTask43->Position = msg_data.intCounter % formMain->pbarTask43->Max;
	  if(!strcmp(msg_data.cmd, CMD_STOP))
		boolTask3Stop = true;
	}

	if(boolTask1Stop && boolTask2Stop && boolTask3Stop)
	{
	  STOP;				// терминиране на задачата
	}

	Switch();			// превключване на контекста
  }
}
//---------------------------------------------------------------------------

