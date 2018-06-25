#include "tester.h"
INT_VARIABLE(CL_dlog, 1, ""){}
INT_VARIABLE(MODE,2,""){}

ONEOF_VARIABLE(dlog_verbosity, " Header Only,Passing & Failing Pins, Off, Failing Pins, Passing Pins", "Dlog_Control")
{
	if (OnHost())
	{
		for (int isite = 1; isite <= Max_Site; isite += 1)
		{
			remote_send(dlog_verbosity, isite, TRUE);
		}
	}
	if (dlog_verbosity == "Off")
		remote_set(CL_dlog, 0, site_num(),TRUE);
	if (dlog_verbosity == "Header Only")
		remote_set(CL_dlog, 1, site_num(), TRUE);
	if (dlog_verbosity == "Failing Pins")
		remote_set(CL_dlog, 2, site_num(), TRUE);
	if (dlog_verbosity == "Passing Pins")
		remote_set(CL_dlog, 3, site_num(), TRUE);
	if (dlog_verbosity == "Passing & Failing Pins")
		remote_set(CL_dlog, 4, site_num(), TRUE);
}
ONEOF_VARIABLE(Mode_Selection, "Standard mode, Debug mode,  Production mode", "Mode_Selection")
{
	if (Mode_Selection == "Debug mode")
		MODE = 1;
	if (Mode_Selection == "Standard mode")
		MODE = 2;
	if (Mode_Selection == "Production mode")
		MODE = 3;
}