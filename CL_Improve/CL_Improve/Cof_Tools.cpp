#include "tester.h"


hash_map<TestBlock *, CString> fail_index;
hash_map<TestBlock *, CString> pass_index;

TEST_BIN(cof_bin)
{
	DutNumArray active_duts_buffer;
	active_duts_get(&active_duts_buffer);

	hash_map<DutNum, bool> Dut_Bin_Flag;
	SoftwareOnlyActiveDutIterator Dut_loop;
	while (Dut_loop.More(TRUE))
	{
		DutNum dut = active_dut_get();
		Dut_Bin_Flag[dut] = 1;
	}

	//Scanning for the fail bin
	for (int i = 0; i < tblk_list.GetSize(); i++)//Scannning all test block
	{
		active_duts_enable(0x0, FALSE);
		for (DutNum dut = t_dut1; dut < max_dut()+1; ++dut)
		{
			if (History_curr_dut_fail_mask.GetAt(i) & ((UINT64)0x1 << dut))
			{
				active_duts_enable(dut, FALSE);
				if (Dut_Bin_Flag[dut])
				{
					CString  Demo = tblk_list.GetAt(i);
					char * demo = Demo.GetBuffer(Demo.GetLength());
					CString bin_name = resource_name(TestBlock_find(demo));
					if (TestBin_find(fail_index[TestBlock_find(demo)]) != NULL)
					{
						invoke(TestBin_find(fail_index[TestBlock_find(demo)]));
						Dut_Bin_Flag[dut] = 0;
					}
				}
				
				active_duts_enable(0x0, FALSE);
			}
			else
				continue;
		}
	}
	active_duts_enable(active_duts_buffer, FALSE);
}