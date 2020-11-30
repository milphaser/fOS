/////////////////////////////////////////////////////////////////////////////
//  fiberOS
//  ver. 5.5
//
// Copyright (c) 2014-2020 by Milen Loukantchevshy.
/////////////////////////////////////////////////////////////////////////////
//
//	CSP ���������:
//		CHAN_MSH	- ������� ���������
//		CHAN		- ���������� �� �������������� �����
//		SEND(CHAN* chanOut, CHAN_MSG* src)	- ��������� �� ���������
//		RECV(CHAN* chanIn, CHAN_MSG* dst)	- ���������� �� ���������
//		STOP		- ����������� �� ��������
//		ALT			- ������������ (��������������) �����/2 ������ ������
//		TODO: PAR
//
/////////////////////////////////////////////////////////////////////////////

#include <vcl.h>
#pragma hdrstop

#include "UnitCSP.h"
//---------------------------------------------------------------------------
extern TASK*	runtask;  		// ������� ������
//---------------------------------------------------------------------------
//  CreateChan()
//  ��������� �� ����������� ��������� ����� (�� ���� 1:1)
//
//  ������ ���������:
//		name	- ��� �� ������
//		in		- ������������� �� �������� �� ����� �� ������
//		out		- ������������� �� �������� �� ������ �� ������
//  ������� ��������: ������������� �� ���������� �����
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
//  ��������� �� ��������� ��� �����
//	�������������: "�������"
//
//  ������ ���������:
//		chanOut	- ������������� �� �������� �����
//		src		- �������� �� �����������
//  ������� ��������: ������� �� ������� ����������
//
bool SEND(CHAN* chanOut, CHAN_MSG* src)
{
  bool boolResult = false;

  if(runtask == chanOut->in)
  {
	memcpy(&chanOut->msg, src, sizeof(chanOut->msg));

	if(chanOut->block)
	{
	  // ��������-�������� � ����������� ��������-����������
	  // ������������ �� ��������-��������
	  Resume(chanOut->block);
	  chanOut->block = NULL;
	}
	else
	{
	  // ��������-���������� � ����������� ��������-��������
	  // ��������� �� ��������-����������
	  chanOut->block = runtask;
	  Suspend();
	}

	boolResult = true;
  }

  return boolResult;
}
//---------------------------------------------------------------------------
//  RECV()
//  ���������� �� ��������� �� �����
//	�������������: "�������"
//
//  ������ ���������:
//		chanIn	- ������������� �� ������� �����
//		dst		- �������� �� �����������
//  ������� ��������: ������� �� ������� ����������
//
bool RECV(CHAN* chanIn, CHAN_MSG* dst)
{
  bool boolResult = false;

  if(runtask == chanIn->out)
  {
	if(chanIn->block)
	{
	  // ��������-���������� � ����������� ��������-��������
	  // ������������ �� ��������-����������
	  Resume(chanIn->block);
	  chanIn->block = NULL;
	}
	else
	{
	  // ��������-�������� � ����������� ��������-����������
	  // ��������� �� ��������-��������
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
//  ������������ (��������������) �����/����������
//
//  ������ ���������:
//		chanIn1	- ������������� �� ������ ����� 1
//		chanIn2	- ������������� �� ������ ����� 2
//		dst		- �������� �� �����������
//  ������� ��������: ����� �� �������� �����
//		-1		- �� � ������ �����, ������ ����� �� ���������
//		 0		- ������ � ����� chanIn1
//		 1		- ������ � ����� chanIn2
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

