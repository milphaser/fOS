//---------------------------------------------------------------------------

#ifndef UnitCSPH
#define UnitCSPH
//---------------------------------------------------------------------------
#include "UnitFormMain.h"
//---------------------------------------------------------------------------
const int CHAN_MSG_LEN	= 128;			// ����.������� �� CHAN_MSG.data
const int CHAN_TAG_LEN 	= 128;			// ����.������� �� ����� �� ������

typedef struct
{
  int len;								// ������� �� ���� �����
  BYTE data[CHAN_MSG_LEN];				// ���� �����
} CHAN_MSG;								// ������� ��������� ////////////////

typedef struct
{
  char tag[CHAN_TAG_LEN];				// ��� �� ������
  TASK* in;								// ������ �� ����� �� ������
  TASK* out;							// ������ �� ������ �� �����
  TASK* block;							// ������, ��������� �� ������
  CHAN_MSG msg;							// ��������� � ������
} CHAN;									// ���������� �� ����� //////////////
//---------------------------------------------------------------------------
CHAN* CreateChan(char* name, TASK* in, TASK* out);
bool SEND(CHAN* chanOut, CHAN_MSG* src);
bool RECV(CHAN* chanIn, CHAN_MSG* dst);
#define STOP	Terminate();
//---------------------------------------------------------------------------
#endif
