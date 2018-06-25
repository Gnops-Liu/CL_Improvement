#pragma once
#ifdef CL_IMPROVE_EXPORT
#define SHARING_PORT_API __declspec(dllexport)
#define PRIVATE_PORT_API __declspec(dllimport)
#else
#define SHARING_PORT_API __declspec(dllimport)
#define PRIVATE_PORT_API __declspec(dllexport)
#endif
/***************CL MACRO**********************/
#undef _EXTERN
#define _EXTERN( name, type )			\
	_EXTERN_DECLARATION(name, type *)

#define  MAKE_THREADFUNC(name)	\
	DWORD WINAPI name##(LPVOID	lpParameter)

#define pTHREADATA	pThreadData
#define POINT_TO_THREADATA()	\
	THREAD_DATA* pTHREADATA = (THREAD_DATA*)lpParameter;

//ABILITY
#define Max_Dut			128
#define Max_TB_Num		3000
#define Max_gross_die	1000000

#define Py_Datalog_Merge_File "\\Datalog_Merge.py"
/*************************************************************/

typedef enum {
	ForceVMeasureI,
	ForceIMeasureV
} IVMeasType;
enum e_log_type { LOG_I, LOG_V, LOG_T };
static IntArray FailCnt, FailCntHi;

//Sharing User Variable
SHARING_PORT_API extern CStringArray	tblk_list;
SHARING_PORT_API EXTERN_CSTRING_VARIABLE(Datalog_File);
SHARING_PORT_API EXTERN_INT_VARIABLE(MODE);
SHARING_PORT_API EXTERN_ONEOF_VARIABLE(Mode_Selection);
SHARING_PORT_API EXTERN_INT_VARIABLE(CL_dlog);
SHARING_PORT_API EXTERN_ONEOF_VARIABLE(dlog_verbosity);
SHARING_PORT_API EXTERN_CSTRING_VARIABLE(CASE_FOLDER);
SHARING_PORT_API EXTERN_BOOL_VARIABLE(UI_CREATE_DATALOG_FILE_FLAG);
SHARING_PORT_API EXTERN_BOOL_VARIABLE(UI_DATALOG_HOST); 
SHARING_PORT_API EXTERN_BOOL_VARIABLE(UI_DATALOG_SITE);
SHARING_PORT_API EXTERN_INT_VARIABLE(Max_Site);

/*?*/SHARING_PORT_API EXTERN_INT_VARIABLE(HOST_CSV_INDEX);
/*?*/SHARING_PORT_API EXTERN_VOID_VARIABLE(HOST_FILEOUT);
/*?*/SHARING_PORT_API EXTERN_INT_VARIABLE(pipe_index);
/*?*/SHARING_PORT_API EXTERN_CSTRING_VARIABLE(Value_Pipe);
//datalog
SHARING_PORT_API extern void set_DatalogMode(int); 
SHARING_PORT_API extern void Show_All_Dut_Info(void);
SHARING_PORT_API extern void Show_All_Site_Info(void);
//func
SHARING_PORT_API extern void	SMutex(void);
SHARING_PORT_API extern int		folder_existed(CString);
SHARING_PORT_API extern void	CC_Path(CString, bool = TRUE);
#define CString2Char(str) str.GetBuffer(str.GetLength())
SHARING_PORT_API extern CString	Int2CString(int);
SHARING_PORT_API extern CString	Double2CString(double);
//Treasure Box 2.0
class SHARING_PORT_API Treasure_save
{
public:
	class Values
	{
	public:
		CArray<CString, CString>   Treasure_box;
		hash_map<const char*, int> Treasure_index;
		CString Ruby;
		CString tmp;
	} MyValue;

	class TreasureBox :public Values
	{
		void resize(int depth);
		void destroy(void);
		void build(int depth);
		bool dig(char *path);
	} MyTreasureBox;

	class SaveTreasure :public Values
	{
		void _int(char* label, int coin);
		void _string(char* label, char* coin);
		void _PF(char* label, bool coin);
		void _hex(char* label, long coin);
		void _double(char* label, double coin);
	} ToSave;
};
//COF ToolBox
SHARING_PORT_API extern hash_map<TestBlock *, CString> fail_index;
SHARING_PORT_API extern hash_map<TestBlock *, CString> pass_index;
SHARING_PORT_API EXTERN_TEST_BIN(cof_bin);
// several block
SHARING_PORT_API EXTERN_AFTER_TESTING_BLOCK(tb_CL_Improve_end);
SHARING_PORT_API EXTERN_BEFORE_TESTING_BLOCK(tb_CL_Improve_init);
SHARING_PORT_API extern void CL_SiteBeginBlock(void);

//Test Block Control Group
SHARING_PORT_API extern SiteMask cl_site_mask;
SHARING_PORT_API extern UINT64 active_dut_mask;
SHARING_PORT_API extern IntArray r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14, r15;	// To store local test results
SHARING_PORT_API extern IntArray r16, r17, r18, r19, r20, r21, r22, r23, r24, r25;	// To store local test results
SHARING_PORT_API extern TEST_TIMER TB_Timer;
SHARING_PORT_API extern TEST_TIMER FlowTimer;
//classes
typedef struct __THREAD_DATA
{
	int nMaxNum = 1;
	CString strThreadName;
	__THREAD_DATA() :nMaxNum(0)
	{
		strThreadName.ReleaseBuffer();
	}
} THREAD_DATA;


class SHARING_PORT_API  tbVarGroup
{
public:
	TestBlock *tb;
	CStringArray tbv_label;
	CStringArray tbv_value;
	void set_value(CString label, CString var);
	void RemoveAll();
};
class SHARING_PORT_API  CL_CSV
{
public:
	void init();
	void save_title(CString label, int column_index);
	void save_value(CString label, int row_index, CString content);
	void data_merge(void);
	int get_MaxRow(void);
	int get_MaxCol(void);
	int makeCSV(CString Path);
	//for programmer debug
	void print_down(void);
	bool If_TitleExisted(CString label);
	CStringArray CSV_Map_buffer;

private:
	CArray<CStringArray*, CStringArray*> CSV_Map;
	CStringArray buffer[Max_gross_die];
};
class SHARING_PORT_API  TBCG
{
public:
	CString							Lot_ID;
	CString							Slot_ID;
	IntArray						XArray, YArray;
	CL_CSV							TBCG_CSV;
	IntArray						FailCnt, FailCntHi;
	DutNumArray						last_active_duts;
	tbVarGroup						CLTB[Max_TB_Num];
	CStringArray					TBNameList;
	hash_map<char *, tbVarGroup*>	tblk2Var;

	UINT64							curr_dut_mask;
	DutNumArray						last_active_dut_mask;
	void	initial(void);
	void	fresh(void);
	int		header(void);
	int		end(bool flag = 1);
	void	set_WaferInfo(CString lot, CString slot);
	void	set_Coordinate(int x, int y, int dut_num);
	void	display_test_results(bool);


	void value(CString label, int*		Var);
	void value(CString label, double*	Var);
	void value(CString label, char**	Var);
	void value(CString label, UINT64*	Var);
	void value(CString label, bool*		Var);
	
	//dartalog
	CString SwitchU(double value, e_log_type log_type);
	void	Pdatalog(PinList *pPinList, IVMeasType meas_type);
	void	Fdatalog(PinList *pPinList);
	// for programmer debug
	void	Show_linking(void);
	void	Fileout(CString Path);
private:
	CString		demo_path = "";
	CString		FileName = "";
	CString		FileName_site = "";
	void		TBNameStore();
	bool		TBFirstAppear(void);
	int			GetTBNameLocation(void);
};
class SHARING_PORT_API	CL_THREAD
{
public:
	HANDLE Mutex = NULL;
	void  createMutex();
	void  createEmptyThread(HANDLE th, LPTHREAD_START_ROUTINE func_address);
	void  createStdThread(HANDLE th, LPTHREAD_START_ROUTINE func_address, LPVOID Var_address);
	void  closeThread(HANDLE th);
};
class SHARING_PORT_API CL_Timer
{
public:
	void startTimer(char* = "label");
	void finishTimer(char* = "label");
	double duration(char* = "label");
private:
	hash_map<char *, clock_t> start, finish;
	hash_map<char *, double> step;
};

//export port
SHARING_PORT_API extern TBCG TB;
SHARING_PORT_API extern CL_CSV CSV;
SHARING_PORT_API extern CL_THREAD THREAD;
SHARING_PORT_API extern CL_Timer TIMER;


