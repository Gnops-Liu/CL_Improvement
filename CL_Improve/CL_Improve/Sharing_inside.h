#define  HOST 0
//object
static CL_Timer colin_timer;
extern SiteMask cl_site_mask;

//FUNC
extern bool CreateMyProcess(char *);
extern bool CL_Remove(CString cmd);
extern void CL_Comment(CString str);
extern void	flag(int);

EXTERN_VOID_VARIABLE(SMutex_Clear);
EXTERN_VOID_VARIABLE(SMutex_Combine);
EXTERN_BOOL_VARIABLE(SMutex_Signal);
EXTERN_VOID_VARIABLE(Clear_Host_Variable);
//LOG
EXTERN_CSTRING_VARIABLE(TB_NAME);
EXTERN_UINT64_VARIABLE(FDUT_MASK);
EXTERN_UINT64_VARIABLE(ADUT_MASK);
EXTERN_BOOL_VARIABLE(AFDUT_RESULT_OUT);


//Datalog
extern void CPy_Datalog_Merge(CString path);

EXTERN_VOID_VARIABLE(Get_Datalog_file);
EXTERN_CSTRING_VARIABLE(Datalog_output_File);
EXTERN_CSTRING_VARIABLE(Datalog_Merge_Py);
EXTERN_VOID_VARIABLE(Datalog_Merge);
// Variable

extern CStringArray					Host_Filename;
extern CArray<UINT64, UINT64>		History_curr_dut_fail_mask;
extern CArray<UINT64, UINT64>		History_curr_dut_mask;
extern CStringArray					History_test_time;
extern CArray<UINT64, UINT64>		Host_curr_dut_fail_mask[64];
extern CArray<UINT64, UINT64>		Host_curr_dut_mask[64];
extern bool							first_touchdown;