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
extern CHAN*	chanPQ;			// канал между P и Q
extern CHAN*	chanCP;			// канал между C и P
//---------------------------------------------------------------------------
void WINAPI Task1(PVOID pvParam)
{
  TASK_MSG msg_data;
  msg_data.id = 1;					// Task1 Id
  msg_data.intCounter = 0;
  strcpy(msg_data.cmd, CMD_EMPTY);

  formMain->stTask1->Caption = msg_data.intCounter;
  formMain->pbarTask1->Position = msg_data.intCounter;

  CHAN_MSG msg;
  msg.len = sizeof(msg_data);

  // Генериране на началното съобщение
  msg_data.intCounter++;
  if(formMain->boolTask1Stop)
	strcpy(msg_data.cmd, CMD_STOP);

  formMain->stTask1->Caption = msg_data.intCounter;
  formMain->pbarTask1->Position = msg_data.intCounter % formMain->pbarTask1->Max;

  memcpy(&msg.data, &msg_data, msg.len);
  SEND(chanPQ, &msg);

  while(true)
  {
	// РАБОТЕН ЦИКЪЛ НА ЗАДАЧАТА

	// Получаване на съобщението
	RECV(chanCP, &msg);
	memcpy(&msg_data, &msg.data, msg.len);

	formMain->stTask1->Caption = msg_data.intCounter;
	formMain->pbarTask1->Position = msg_data.intCounter % formMain->pbarTask1->Max;

	Sleep(DELAY);		// изчакване за визуализация

	// Корекция и препредаване на съобщението
	msg_data.id = 1;				// Task1 Id
	msg_data.intCounter++;
	if(formMain->boolTask1Stop)
	  strcpy(msg_data.cmd, CMD_STOP);
	memcpy(&msg.data, &msg_data, msg.len);
	SEND(chanPQ, &msg);

	if(!strcmp(msg_data.cmd, CMD_STOP))
	{
	  STOP;				// терминиране на задачата
	}

	Switch();			// превключване на контекста
  }
}
//---------------------------------------------------------------------------

