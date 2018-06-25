#include "tester.h"

/***************************************
*SN:		folder_existed
*Format:	int folder_existed(char *fn)
*Parameter: fn>>file's name
*returns:	0 >>This file is not existed
*			1 >>This file is existed
****************************************/
int folder_existed(CString fn)
{
	return (_access(fn, 00) == 00);
}

/***************************************
*SN:		CC_Path
*Format:	void CC_Path(CString Type_path, bool datalog_control)
*Parameter: Type_path>>path's name
datalog_control >> control log
*Returns:	NULL
*Mention:   If anyone wanna use this subroutine
to Create and Check Path, The separator
can only be "\\"
****************************************/
void CC_Path(CString Type_path, bool datalog_control)
{
	CString path = Type_path;
	CArray<int, int> separator;
	while (path.Find("\\") != -1)
	{
		separator.Add(path.Find("\\"));
		path.SetAt(path.Find("\\"), ' ');
	}
	separator.Add(Type_path.GetLength());

	if (datalog_control)
	{
		output(">>>>>>>>>>>>>>Check/Create folder<<<<<<<<<<<<<<");
		output("Check/Create %s", Type_path);
	}
	for (int path_level = 0; path_level < separator.GetSize(); path_level++)
	{
		path = Type_path;
		for (int i = separator.ElementAt(path_level); i < Type_path.GetLength(); i++)
		{
			path.SetAt(i, ' ');
		}
		if (datalog_control)
		{
			output("%s\t\\", path);
			output("%-7s\\", (folder_existed(path) ? "Existed" : "Non-Existed -> "));
		}
		if (!folder_existed(path))
		{
			CString str1 = "md ";
			str1 += path;
			char *str2 = str1.GetBuffer(str1.GetLength());
			str1.ReleaseBuffer();
			system(str2);
			if (datalog_control)
			{
				output("%s", (folder_existed(path) ? "Done" : "Fail"));
			}
		}
		else
		if (datalog_control)
		{
			output(" ");
		}
	}
}


//timer
CL_Timer TIMER;
void	CL_Timer::startTimer(char*  label)
{
	start[label] = clock();
}
void	CL_Timer::finishTimer(char*  label)
{
	finish[label] = clock();
}
double	CL_Timer::duration(char*  label)
{
	step[label] = (double)(finish[label] - start[label]);
	return step[label];
}

//comment
void flag(int num)
{
	output("[flag%d]\\", num);
}
void CL_Comment(CString str)
{
	if (MODE < 2)
		output("<COLOR=MAROON>[CL IMPROVE]%s</COLOR>", str);
}
//Format change
CString		Int2CString(int val)
{
	register CString temp;
	temp.Format("%d",val);
	return  temp;
}
CString		Double2CString(double val)
{
	register CString temp;
	temp.Format("%lf", val);
	return temp;
}

//CL_CSV func
CL_CSV CSV;
void	CL_CSV::init()
{
	CSV_Map.RemoveAll();
	CSV_Map_buffer.RemoveAll();
	for (int i = 0; i < Max_gross_die; i++)
		buffer[i].RemoveAll();
}
void	CL_CSV::save_title(CString label, int column_index)
{
	CSV_Map.SetAtGrow(0,&buffer[0]);
	//TODO:maybe can be skip
	CStringArray *titleLine = CSV_Map.GetAt(0);
	titleLine->SetAtGrow(column_index, label);
	//TODO:maybe can be skip
	CSV_Map.SetAt(0, titleLine);
}
void	CL_CSV::save_value(CString label, int row_index, CString content)
{
	remote_set(HOST_CSV_INDEX, row_index, 0, TRUE, INFINITE);
	if (row_index < 1)	fatal("[CL_ImproveError]you set illegal row_index into CL_CSV::save_value!!!");
	CSV_Map.SetAtGrow(row_index, &buffer[row_index]);
	long col = -1;
	CStringArray *titleLine = CSV_Map.GetAt(0);
	for (int i = 0; i < titleLine->GetSize(); i++)
	{
		if (titleLine->GetAt(i) == label)
		{
			col = i;
		}
	}
	if (row == -1) fatal("[CL_ImproveError]Set Title First!!!");
	CStringArray *Hinst = CSV_Map.GetAt(row_index);
	Hinst->SetAtGrow(col, content);
	CSV_Map.SetAt(row_index, Hinst);
}
int		CL_CSV::get_MaxRow(void)
{
	CSV_Map.FreeExtra();
	return CSV_Map.GetSize();
}
int		CL_CSV::get_MaxCol(void)
{
	CStringArray *titleLine = CSV_Map.GetAt(0);
	titleLine->FreeExtra();
	return titleLine->GetSize();
}
int		CL_CSV::makeCSV(CString Path)
{
	CSV_Map.FreeExtra();
	fileout.open(Path);
	for (int i = 0; i < CSV_Map.GetSize(); i++)
		CSV_Map.SetAtGrow(i, &buffer[i]);

	for (int row = 0; row < CSV_Map.GetSize(); row++)
	{
		for (int col = 0; col < CSV_Map.GetAt(row)->GetSize(); col++)
		{
			fileout << CSV_Map.GetAt(row)->GetAt(col) + ",";
		}
		fileout << "\n";
	}
	fileout.close();
	return 0;
}
void	CL_CSV::print_down()
{
	CSV_Map.FreeExtra();
	for (int i = 0; i < CSV_Map.GetSize(); i++)
		CSV_Map.SetAtGrow(i, &buffer[i]);
	for (int row = 0; row < CSV_Map.GetSize(); row++)
	{
		output("[%d]\\",row);
		for (int col = 0; col < CSV_Map.GetAt(row)->GetSize(); col++)
		{
			output("%s\t,\\",CSV_Map.GetAt(row)->GetAt(col));
		}
		output("\n");
	}
}
bool	CL_CSV::If_TitleExisted(CString label)
{
	for (int i = 0; i < CSV_Map.GetAt(0)->GetSize(); i++)
	{
		if (CSV_Map.GetAt(0)->ElementAt(i) == label)
			return 1;
	}
	return 0;
}

