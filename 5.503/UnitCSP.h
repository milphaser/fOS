//---------------------------------------------------------------------------

#ifndef UnitCSPH
#define UnitCSPH
//---------------------------------------------------------------------------
#include "UnitFormMain.h"
//---------------------------------------------------------------------------
const int CHAN_MSG_LEN	= 128;			// макс.дължина на CHAN_MSG.data
const int CHAN_TAG_LEN 	= 128;			// макс.дължина на името на канала

typedef struct
{
  int len;								// дължина на поле данни
  BYTE data[CHAN_MSG_LEN];				// поле данни
} CHAN_MSG;								// КАНАЛНО СЪОБЩЕНИЕ ////////////////

typedef struct
{
  char tag[CHAN_TAG_LEN];				// име на канала
  TASK* in;								// задача на входа на канала
  TASK* out;							// задача на изхода на канла
  TASK* block;							// задача, блокирана на канала
  CHAN_MSG msg;							// съобщение в канала
} CHAN;									// ДЕСКРИПТОР НА КАНАЛ //////////////
//---------------------------------------------------------------------------
CHAN* CreateChan(char* name, TASK* in, TASK* out);
bool SEND(CHAN* chanOut, CHAN_MSG* src);
bool RECV(CHAN* chanIn, CHAN_MSG* dst);
#define STOP	Terminate();
//---------------------------------------------------------------------------
#endif
