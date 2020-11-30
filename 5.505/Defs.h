//---------------------------------------------------------------------------

#ifndef DefsH
#define DefsH
//---------------------------------------------------------------------------
// Application Definitions
//

// ������� �� ��������� �������������� ��������
#define CMD_EMPTY	"CMD_EMPTY"
#define CMD_STOP	"CMD_STOP"

const int TASK_MSG_CMD_LEN	= 16;		// ����.������� �� TASK_MSG.cmd

typedef struct
{
  int id;						// task id
  int intCounter;				// counter
  char cmd[TASK_MSG_CMD_LEN];	// command
} TASK_MSG;						// ������������� ��������� //////////////////
//---------------------------------------------------------------------------
#endif
