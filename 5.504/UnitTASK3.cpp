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
  formMain->stTask3->Caption = msg_data.intCounter;
  formMain->pbarTask31->Position = msg_data.intCounter;
  formMain->pbarTask32->Position = msg_data.intCounter;

  CHAN_MSG msg;
  msg.len = sizeof(msg_data);

  bool boolTask1Stop = false, boolTask2Stop = false;

  while(true)
  {
	// Работен цикъл
	// Получаване на съобщение чрез оператор за избор

	// TODO: ALT
	#if (__ALT__ == 1) 		//---------------------------------------------1-
	if(RECV(chanP1Q, &msg))
	{
	  memcpy(&msg_data, &msg.data, msg.len);
	}
	else if(RECV(chanP2Q, &msg))
	{
	  memcpy(&msg_data, &msg.data, msg.len);
	}
	#elif (__ALT__ == 2)	//---------------------------------------------2-
	if(RECV(chanP2Q, &msg))
	{
	  memcpy(&msg_data, &msg.data, msg.len);
	}
	else if(RECV(chanP1Q, &msg))
	{
	  memcpy(&msg_data, &msg.data, msg.len);
	}
	#elif (__ALT__ == 3) 	//---------------------------------------------3-
	int chan_selector = -1;

	if(chanP1Q->block && !chanP2Q->block)
	  chan_selector = 1;
	else if(!chanP1Q->block && chanP2Q->block)
	  chan_selector = 2;
	else if(chanP1Q->block && chanP2Q->block)
	  chan_selector = Random(2);

	if(chan_selector == 1)
	{
	  RECV(chanP1Q, &msg);
	  memcpy(&msg_data, &msg.data, msg.len);
	}
	else if(chan_selector == 2)
	{
	  RECV(chanP2Q, &msg);
	  memcpy(&msg_data, &msg.data, msg.len);
	}
	#elif (__ALT__ == 4) 	//---------------------------------------------4-
	ALT(chanP1Q, chanP2Q, &msg);
	memcpy(&msg_data, &msg.data, msg.len);
	#else 					//-----------------------------------------------
	  #error НЕКОРЕКТНА ВЕРСИЯ
	#endif //----------------------------------------------------------------

	formMain->stTask3->Caption = msg_data.intCounter;
	if(msg_data.id == 1)		// Task1 Id
	{
	  formMain->pbarTask31->Position = msg_data.intCounter;
	  if(msg_data.intCounter == LIMIT)
		boolTask1Stop = true;
	}
	else if(msg_data.id == 2)	// Task2 Id
	{
	  formMain->pbarTask32->Position = msg_data.intCounter;
	  if(msg_data.intCounter == LIMIT)
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

