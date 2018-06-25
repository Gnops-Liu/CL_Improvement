#include "tester.h"
CL_THREAD THREAD;


bool CreateMyProcess(char *cmd)
{
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof STARTUPINFO;

	PROCESS_INFORMATION pi;
	BOOL res =
		CreateProcess(NULL, 
					  cmd, 
					  NULL, 
					  NULL, 
					  NULL,
					  NORMAL_PRIORITY_CLASS|
					  CREATE_NO_WINDOW,
					  NULL,
					  NULL,
					  &si,
					  &pi);
	if (res)
	{
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}
	return 1;
}

void CL_THREAD::createMutex()
{
	
}

void CL_THREAD::createEmptyThread(HANDLE th, LPTHREAD_START_ROUTINE func_address)
{
	th = CreateThread(NULL, 0, func_address, NULL, 0, NULL);
}

void CL_THREAD::createStdThread(HANDLE th, LPTHREAD_START_ROUTINE func_address, LPVOID Var_address)
{
	th = CreateThread(NULL, 0, func_address, Var_address,0, NULL);
}


void CL_THREAD::closeThread(HANDLE th)
{
	CloseHandle(th);
}
