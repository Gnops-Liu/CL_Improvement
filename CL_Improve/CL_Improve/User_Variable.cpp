#include "tester.h"
SiteMask cl_site_mask;

//SiteMutex
SiteMask SiteMutex(0x0);
VOID_VARIABLE(SMutex_Clear,"" )
{
	if (OnHost())
	{
		SiteMutex &= ~SITE_TO_SITEMASK(sender);
		CL_Comment("maskclear:" + sitemask_to_string(SiteMutex));

		if (SiteMutex == 0)
		{
			for (int i = 1; i <= Max_Site; i++)
				remote_set(SMutex_Signal, 0, i, FALSE, INFINITE);
		}
	}
}
VOID_VARIABLE(SMutex_Combine, "")
{
	if (OnHost())
	{
		SiteMutex |= SITE_TO_SITEMASK(sender);
		CL_Comment("mask:" + sitemask_to_string(SiteMutex));
		CL_Comment("cl_mask:" + sitemask_to_string(cl_site_mask));
		if (SiteMutex == cl_site_mask)
		{
			for (int i = 1; i <= Max_Site; i++)
				remote_set(SMutex_Signal, 1, i, false, 0);
		}
	}
}
BOOL_VARIABLE(SMutex_Signal, 0, "")
{
	CL_Comment("Current SMutex_Signal:" + Int2CString(SMutex_Signal));
}
void SMutex()
{

	colin_timer.startTimer();
	CL_Comment("site_mutexing...");
	CL_Comment("Following thing is the state of Site_Mutex");
	CL_Comment("Max Site:" + Int2CString(Max_Site));
	remote_send(SMutex_Combine, 0, TRUE, INFINITE);

	while (!SMutex_Signal){}
	colin_timer.finishTimer();
	CL_Comment("Cost time:" + Double2CString(colin_timer.duration()));
	remote_send(SMutex_Clear, HOST, TRUE, INFINITE);
}	

// CL log
CSTRING_VARIABLE(CASE_FOLDER, "", ""){}
BOOL_VARIABLE(UI_CREATE_DATALOG_FILE_FLAG, 1, "UI_Create_Datalog_File_Flag")
{
	if (UI_CREATE_DATALOG_FILE_FLAG)
	{
		remote_set(UI_DATALOG_SITE, 1, site_num(), TRUE, INFINITE);
	}
	else
	{
		remote_set(UI_DATALOG_SITE, 0, site_num(), TRUE, INFINITE);
	}
}
BOOL_VARIABLE(UI_DATALOG_SITE, 0, "")
{
	if (UI_DATALOG_SITE)
	{
		CString s1, s3;
		s3.Format("\\site%d_", site_num());
		s1 = CASE_FOLDER + s3 + CTime::GetCurrentTime().Format("%H%M%S") + ".txt";
		remote_set("ui_OutputFile", s1, -1, TRUE, INFINITE);
		remote_set("ui_ClearAtTestStart", 1, -1, TRUE, INFINITE);

		s1.ReleaseBuffer();
		s3.ReleaseBuffer();
		output("[Tip 0002] UI_site_datalog opened");
	}
	else
	{
		output("[Tip 0001] UI_site_datalog closed");
	}
}
BOOL_VARIABLE(UI_DATALOG_HOST, 0, "")
{
	if (UI_DATALOG_HOST)
	{
		CString s2;
		output("[Tip 0004] UI_host_datalog opened");
		s2 = CASE_FOLDER;

		CString f = "/HOST.TXT";
		s2 += f;

		remote_set("ui_OutputFile", s2, -1, TRUE, INFINITE);
		s2.ReleaseBuffer();
	}
	else
		output("[Tip 0003] UI_host_datalog closed");
}
VOID_VARIABLE(Clear_Host_Variable, "")
{
	if (OnHost() && sender == 1)
	{
		tblk_list.RemoveAll();
		for (int i = 0; i < 64; i++)
		{
			Host_curr_dut_fail_mask[i].RemoveAll();
			Host_curr_dut_mask[i].RemoveAll();
		}
	}
}

//ui_ProgLoaded
INT_VARIABLE(Max_Site,0, "")
{
	cl_site_mask = string_to_sitemask(remote_get("ui_SiteMask", -1));
	output("ui_SiteMask => %X", sitemask_to_string(cl_site_mask));
	Max_Site = (int)LAST_SITE(cl_site_mask);
	remote_send(Max_Site,0,false,0);
}

//Datalog UNIT
CSTRING_VARIABLE(TB_NAME, "", "")
{
	tblk_list.Add(TB_NAME);
}
UINT64_VARIABLE(FDUT_MASK, 0, "")
{
	Host_curr_dut_fail_mask[sender].Add(FDUT_MASK);
}
UINT64_VARIABLE(ADUT_MASK, 0, "")
{
	Host_curr_dut_mask[sender].Add(ADUT_MASK);
}
BOOL_VARIABLE(AFDUT_RESULT_OUT, 0, "")
{
	bool log_flag = 1;
	CString Demo_Info = "";
	CString demo;
	//have bug on one site mode 
	for (int i = 1; i < Max_Site; i += 1)
	{
		if (Host_curr_dut_fail_mask[i].GetSize() != Host_curr_dut_fail_mask[i + 1].GetSize())
		{
			log_flag = 0;
			output("The count:%d", Host_curr_dut_fail_mask[i].GetSize());
			fatal("Record count not match[1]");
		}
	}

	if (tblk_list.GetSize() != Host_curr_dut_fail_mask[1].GetSize())
	{
		log_flag = 0;
		fatal("Record count not match[2]");
	}
	if (tblk_list.GetSize() != Host_curr_dut_mask[1].GetSize())
	{
		log_flag = 0;
		fatal("Record count not match[3]");
	}

	output("+================Current result summary================+");
	if (log_flag)
	{
		for (int index = 0; index < tblk_list.GetSize(); index++)
		{
			Demo_Info = "";
			demo = "";
			demo.Format("%-30s", tblk_list.GetAt(index));
			Demo_Info += demo;
			Demo_Info += ":";
			for (int i = 1; i <= Max_Site; i++)
			{
				for (DutNum dut = t_dut1; dut < max_dut() + 1; ++dut)
				{
					if (Host_curr_dut_fail_mask[i].GetAt(index) & ((UINT64)0x1 << dut))
						Demo_Info += "<COLOR=Red>F </COLOR>";
					else
						(Host_curr_dut_mask[i].GetAt(index) & ((UINT64)0x1 << dut)) ? Demo_Info += "<COLOR=Green>P </COLOR>" : Demo_Info += "- ";
				}
				Demo_Info += "|";
			}
			output(Demo_Info);

		}
	}
}

// Datalog Merge
CStringArray Host_Filename;
CSTRING_VARIABLE(Datalog_output_File, "", "Datalog_output_File"){}
CSTRING_VARIABLE(Datalog_File, "","")
{
	if (OnHost())
		Host_Filename.Add(Datalog_File);
	Host_Filename.FreeExtra();
}
CSTRING_VARIABLE(Datalog_Merge_Py, "", "Datalog_Merge_Py"){}
VOID_VARIABLE(Datalog_Merge,"")
{

	if (OnHost() && sender == 1)
	{
		CString command = "python " + Datalog_Merge_Py + " " + Datalog_output_File + " ";
		for (int i = 0; i < Host_Filename.GetSize(); i++) 
			command += Host_Filename.GetAt(i) + " ";
		char *Pcommand = CString2Char(command);
		CreateMyProcess(Pcommand);
	}
}

//FOLLOWING THINGS IS HOST PIPE SOLUTION[UNCOMPLETED]
IntArray Host_CSV_Index;
INT_VARIABLE(HOST_CSV_INDEX, 0, "")
{
	Host_CSV_Index.SetAtGrow(HOST_CSV_INDEX, sender);
}
void pipe(int site, int val)
{
	remote_set(pipe_index, val, site, TRUE, INFINITE);
	remote_get(Value_Pipe, site,TRUE,INFINITE);
}
VOID_VARIABLE(HOST_FILEOUT, "")
{
	Host_CSV_Index.FreeExtra();
	pipe(1,0);
	TB.TBCG_CSV.CSV_Map_buffer.SetAt(0, Value_Pipe);
	for (int i = 1; i < Host_CSV_Index.GetSize(); i++)
	{
		pipe(Host_CSV_Index.GetAt(i), i);
		TB.TBCG_CSV.CSV_Map_buffer.SetAt(i, Value_Pipe);
	}
}
INT_VARIABLE(pipe_index, 0, ""){}
CSTRING_VARIABLE(Value_Pipe, "", "")
{
	if (sender = 0)
		Value_Pipe = TB.TBCG_CSV.CSV_Map_buffer.GetAt(pipe_index);
}