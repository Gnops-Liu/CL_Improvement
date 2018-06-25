#include "tester.h"

void CL_SiteBeginBlock(void)
{
	TB.initial();
}

BEFORE_TESTING_BLOCK(tb_CL_Improve_init)
{
	output("all CL_Improvement Variable have been initialized....");
	History_curr_dut_fail_mask.RemoveAll();
	History_curr_dut_mask.RemoveAll();
	History_test_time.RemoveAll();
	remote_send(Clear_Host_Variable,0,TRUE,INFINITE);
	TB.fresh();
}

AFTER_TESTING_BLOCK(tb_CL_Improve_end) {
	output("output from tb_CL_Improve_end!!!!!");
}

