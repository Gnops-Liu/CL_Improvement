#include "tester.h"

void set_DatalogMode(int i)
{
	switch (i)
	{
	case 0: 
		remote_set(dlog_verbosity, "Off", HOST, TRUE, INFINITE); 
		output("[CL Tip]you have selsect \"Off\" as datalog default type"); 
		break;
	case 1:
		remote_set(dlog_verbosity, "Header Only", HOST, TRUE, INFINITE);
		output("[CL Tip]you have selsect \"Header Only\" as datalog default type");
		break;
	case 2:
		remote_set(dlog_verbosity, "Failing Pins", HOST, TRUE, INFINITE);
		output("[CL Tip]you have selsect \"Failing Pins\" as datalog default type");
		break;
	case 3:
		remote_set(dlog_verbosity, "Passing Pins", HOST, TRUE, INFINITE);
		output("[CL Tip]you have selsect \"Passing Pins\" as datalog default type");
		break;
	case 4:
		remote_set(dlog_verbosity, "Passing & Failing Pins", HOST, TRUE, INFINITE);
		output("[CL Tip]you have selsect \"Passing & Failing Pins\" as datalog default type");
		break;
	default:
		output("<COLOR=RED>[CL Tip]you have selscted a unidentifiable value in \"set_Datalog_type\"</COLOR>");
	}
}

void  Show_All_Dut_Info(void)
{
	CString Demo_Info = "";
	CString demo;
	Demo_Info.ReleaseBuffer();
	Demo_Info = "+====>\t\t\tShow every test detail\t\t\t<====+";
	output(Demo_Info);
	for (int i = 0; i < tblk_list.GetSize(); i++)
	{
		if (i < History_test_time.GetSize())
		{
			Demo_Info = "";
			demo.Format("%-30s", tblk_list.GetAt(i));
			Demo_Info += demo;
			Demo_Info += ":\\";
			output(Demo_Info);
			Demo_Info = "";
			for (int dut = 0; dut <= max_dut(); dut++) {
				if (History_curr_dut_fail_mask.GetAt(i) & ((UINT64)0x1 << dut))
					Demo_Info += "<COLOR=Red>F </COLOR>";
				else
					(History_curr_dut_mask.GetAt(i) & ((UINT64)0x1 << dut)) ? Demo_Info += "<COLOR=Green>P </COLOR>" : Demo_Info += "- ";
			}
			output(Demo_Info);
			//remote send data to host 
			if (site_num() == 1)
				remote_set(TB_NAME, tblk_list.GetAt(i), 0, TRUE, INFINITE);
			remote_set(FDUT_MASK, History_curr_dut_fail_mask.GetAt(i), 0, TRUE, INFINITE);
			remote_set(ADUT_MASK, History_curr_dut_mask.GetAt(i), 0, TRUE, INFINITE);
		}
	}

}

void Show_All_Site_Info(void)
{
	invoke(AFDUT_RESULT_OUT);
}