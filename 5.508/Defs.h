//---------------------------------------------------------------------------

#ifndef DefsH
#define DefsH
//---------------------------------------------------------------------------
// Application Definitions
//

// Команди на приложния комуникационен протокол
#define CMD_EMPTY	"CMD_EMPTY"
#define CMD_STOP	"CMD_STOP"

const int TASK_MSG_CMD_LEN	= 16;		// макс.дължина на TASK_MSG.cmd

typedef struct
{
  int id;						// task id
  int intCounter;				// counter
  char cmd[TASK_MSG_CMD_LEN];	// command
} TASK_MSG;						// СТРУКТУРИРАНО СЪОБЩЕНИЕ //////////////////
//---------------------------------------------------------------------------
#endif
