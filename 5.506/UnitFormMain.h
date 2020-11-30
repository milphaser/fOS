//---------------------------------------------------------------------------

#ifndef UnitFormMainH
#define UnitFormMainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
//---------------------------------------------------------------------------
#include <queue>
using namespace std;
//---------------------------------------------------------------------------
const int DELAY 	= 125;	 		// ���������� � ms
const int LIMIT 	= 100;	 		//

const int TAGSIZE 	= 128;	 		// ����.������� �� ����� �� ��������

typedef PVOID	PTASKCONTEXT;		// �������� �� ������ (��������)

typedef enum
{
  TS_IDLE = 0, TS_READY, TS_RUN, TS_BLOCK, TS_STOP
} TASKSTATE;		   	 			// ��������� �� ��������

typedef struct
{
  char tag[TAGSIZE];   				// ��� �� ��������
  TASKSTATE state;	   				// ������ ��������� �� ��������
  PTASKCONTEXT context;				// �������� �� ��������
} TASK;				   				// ���������� �� ������ /////////////////

//  �������� ��������� //////////////////////////////////////////////////////
TASK* CreateTask(char* name,
				PFIBER_START_ROUTINE pfMain,
				PVOID pvParam = NULL,
				DWORD dwStackSize = 0);
void DeleteTask(TASK* task);
char* GetTaskName(void);
void Switch(void);
void Dispatcher(void);

void Terminate(void);
void Suspend(void);
void Suspend(TASK* task);
void Resume(TASK* task);

//  �������� �������  ///////////////////////////////////////////////////////
void InsertInReadyQ(TASK* task);
TASK* RemoveFirstFromReadyQ(void);
void RemoveTaskFromReadyQ(TASK* task);

//---------------------------------------------------------------------------
class TformMain : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *gboxTask1;
	TGroupBox *gboxTask2;
	TProgressBar *pbarTask1;
	TProgressBar *pbarTask2;
	TButton *buttonSuspend1;
	TButton *buttonResume1;
	TStaticText *stTask1;
	TStaticText *stTask2;
	TButton *buttonStart;
	TButton *buttonSuspend2;
	TButton *buttonResume2;
	TGroupBox *gboxSysStatus;
	TLabel *labelSysTimer;
	TStaticText *stSysTimer;
	TLabel *labelSysTasksNum;
	TStaticText *stSysTasksNum;
	TLabel *labelSysReadyTasksNum;
	TStaticText *stSysReadyTasksNum;
	TGroupBox *gboxTask3;
	TProgressBar *pbarTask3;
	TStaticText *stTask3;
	TButton *buttonSuspend3;
	TButton *buttonResume3;
	TButton *buttonStop1;
	void __fastcall buttonStartClick(TObject *Sender);
	void __fastcall buttonSuspend1Click(TObject *Sender);
	void __fastcall buttonResume1Click(TObject *Sender);
	void __fastcall buttonStop1Click(TObject *Sender);

private:

public:
	__fastcall TformMain(TComponent* Owner);

	bool boolTask1Stop;		// ���� �� �������� �� ������� �� Task1
};
//---------------------------------------------------------------------------
extern PACKAGE TformMain *formMain;
//---------------------------------------------------------------------------
#endif
