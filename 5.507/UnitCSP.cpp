/////////////////////////////////////////////////////////////////////////////
//  fiberOS
//  ver. 5.5
//
// Copyright (c) 2014-2020 by Milen Loukantchevshy.
/////////////////////////////////////////////////////////////////////////////
//
//	CSP поддръжка:
//		CHAN_MSH	- канално съобщение
//		CHAN		- дескриптор на комуникационен канал
//		SEND(CHAN* chanOut, CHAN_MSG* src)	- изпращане на съобщение
//		RECV(CHAN* chanIn, CHAN_MSG* dst)	- получаване на съобщение
//		STOP		- терминиране на задачата
//		ALT			- алтернативен (недетерминиран) избор/2 входни канала
//		TODO: PAR
//
/////////////////////////////////////////////////////////////////////////////

#include <vcl.h>
#pragma hdrstop

#include "UnitCSP.h"
//---------------------------------------------------------------------------
extern TASK*	runtask;  		// активна задача
//---------------------------------------------------------------------------
//  CreateChan()
//  Създаване на еднопосочен двуточков канал (от типа 1:1)
//
//  Входни аргументи:
//		name	- име на канала
//		in		- идентификатор на задачата на входа на канала
//		out		- идентификатор на задачата на изходе на канала
//  Връщана стойност: идентификатор на създадения канал
//
CHAN* CreateChan(char* name, TASK* in, TASK* out)
{
  CHAN* chan = new CHAN;

  if(chan)
  {
	strncat(chan->tag, name, CHAN_TAG_LEN-1);
	chan->in = in;
	chan->out = out;
	chan->block = NULL;
  }

  return chan;
}
//---------------------------------------------------------------------------
//  SEND()
//  Изпращане на съобщение към канал
//	Синхорнизация: "рандеву"
//
//  Входни аргументи:
//		chanOut	- идентификатор на изходния канал
//		src		- източник на съобщението
//  Връщана стойност: признак за успешно изпълнение
//
bool SEND(CHAN* chanOut, CHAN_MSG* src)
{
  bool boolResult = false;

  if(runtask == chanOut->in)
  {
	memcpy(&chanOut->msg, src, sizeof(chanOut->msg));

	if(chanOut->block)
	{
	  // задачата-приемник е изпреварила задачата-предавател
	  // разблокиране на задачата-приемник
	  Resume(chanOut->block);
	  chanOut->block = NULL;
	}
	else
	{
	  // задачата-предавател е изпреварила задачата-приемник
	  // блокиране на задачата-предавател
	  chanOut->block = runtask;
	  Suspend();
	}

	boolResult = true;
  }

  return boolResult;
}
//---------------------------------------------------------------------------
//  RECV()
//  Получаване на съобщение от канал
//	Синхорнизация: "рандеву"
//
//  Входни аргументи:
//		chanIn	- идентификатор на входния канал
//		dst		- приемник на съобщението
//  Връщана стойност: признак за успешно изпълнение
//
bool RECV(CHAN* chanIn, CHAN_MSG* dst)
{
  bool boolResult = false;

  if(runtask == chanIn->out)
  {
	if(chanIn->block)
	{
	  // задачата-предавател е изпреварила задачата-приемник
	  // разблокиране на задачата-предавател
	  Resume(chanIn->block);
	  chanIn->block = NULL;
	}
	else
	{
	  // задачата-приемник е изпреварила задачата-предавател
	  // блокиране на задачата-приемник
	  chanIn->block = runtask;
	  Suspend();
	}

	memcpy(dst, &chanIn->msg, sizeof(chanIn->msg));
	boolResult = true;
  }

  return boolResult;
}
//---------------------------------------------------------------------------
//  ALT()
//  Алтернативен (недетерминиран) избор/двуканален
//
//  Входни аргументи:
//		chanIn1	- идентификатор на входен канал 1
//		chanIn2	- идентификатор на входен канал 2
//		dst		- приемник на съобщението
//  Връщана стойност: номер на избрания канал
//		-1		- не е избран канал, поради липса на съобщения
//		 0		- избран е канал chanIn1
//		 1		- избран е канал chanIn2
//
int ALT(CHAN* chanIn1, CHAN* chanIn2, CHAN_MSG* dst)
{
  int chan_selector = -1;

  if(chanIn1->block && !chanIn2->block)
	chan_selector = 0;
  else if(!chanIn1->block && chanIn2->block)
	chan_selector = 1;
  else if(chanIn1->block && chanIn2->block)
	chan_selector = Random(2);

  if(chan_selector == 0)
  {
	RECV(chanIn1, dst);
  }
  else if(chan_selector == 1)
  {
	RECV(chanIn2, dst);
  }

  return chan_selector;
}
//---------------------------------------------------------------------------

