#include "tester.h"
UINT64 active_dut_mask;
IntArray r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14, r15;	// To store local test results
IntArray r16, r17, r18, r19, r20, r21, r22, r23, r24, r25;	// To store local test results

bool first_touchdown = 1;


TEST_TIMER TB_Timer;
TEST_TIMER FlowTimer;
TBCG TB;
CStringArray				tblk_list;
CArray<UINT64, UINT64>		History_curr_dut_fail_mask;
CArray<UINT64, UINT64>		History_curr_dut_mask;
CStringArray				History_test_time;
CArray<UINT64, UINT64>		Host_curr_dut_fail_mask[64];
CArray<UINT64, UINT64>		Host_curr_dut_mask[64];


void	tbVarGroup::set_value(CString label, CString var)
{
	tbv_label.Add(label);
	tbv_value.Add(var);
}
void	tbVarGroup::RemoveAll()
{
	tb = NULL;
	tbv_label.RemoveAll();
	tbv_value.RemoveAll();
}

void	TBCG::display_test_results(bool flag)
{
	DutNumArray active_duts;
	CString sTb = "";
	CString sTt = "";
	CString str_results = "";


	int count = active_duts_get(&curr_dut_mask);
	if (count != 0)
	{
		if ((r1[0] != -1) || (r2[0] != -1) || (r3[0] != -1) || (r4[0] != -1) || (r5[0] != -1) ||
			(r6[0] != -1) || (r7[0] != -1) || (r8[0] != -1) || (r9[0] != -1) || (r10[0] != -1) ||
			(r11[0] != -1) || (r12[0] != -1) || (r13[0] != -1) || (r14[0] != -1) || (r15[0] != -1) ||
			(r16[0] != -1) || (r17[0] != -1) || (r18[0] != -1) || (r19[0] != -1) || (r20[0] != -1) ||
			(r21[0] != -1) || (r22[0] != -1) || (r23[0] != -1) || (r24[0] != -1) || (r25[0] != -1))
		{
			for (int dut = 0; dut < count; ++dut)
			{
				r1[dut] = r1[dut] && r2[dut] && r3[dut] && r4[dut] && r5[dut]
					&& r6[dut] && r7[dut] && r8[dut] && r9[dut] && r10[dut]
					&& r11[dut] && r12[dut] && r13[dut] && r14[dut] && r15[dut]
					&& r16[dut] && r17[dut] && r18[dut] && r19[dut] && r20[dut]
					&& r21[dut] && r22[dut] && r23[dut] && r24[dut] && r25[dut];
			}
			results_set(r1);
		}

		int count = active_duts_get(&curr_dut_mask);
		count = active_duts_get(&active_duts);
		UINT64 curr_dut_fail_mask = 0;
		for (int ix = 0; ix<count; ix++) {
			if (result_get(active_duts[ix]) == FAIL) {
				curr_dut_fail_mask |= (UINT64)0x01 << (int)active_duts[ix];
			}
		}
		sTb.Format("%-25s  ", current_test_block());
		//ourput the following things
		for (int dut = 0; dut <= max_dut(); dut++) {
			if (curr_dut_fail_mask & ((UINT64)0x1 << dut))
				str_results += "<COLOR=Red>F </COLOR>";
			else
				(curr_dut_mask & ((UINT64)0x1 << dut)) ? str_results += "<COLOR=Green>P </COLOR>" : str_results += "- ";
		}
		sTt.Format("%-10f  ", TB_Timer.StopTimer(""));

		str_results = sTb + sTt + str_results;
		// save down this site's statement
		int index = -1;
		for (int i = 0; i < tblk_list.GetSize(); i++)
		{
			if (tblk_list.GetAt(i) == current_test_block())
			{
				index = i;
				break;
			}
		}
		if (index == -1)
			fatal("[CL Improve]something wrong happened code:02208102");
		History_curr_dut_fail_mask.SetAtGrow(index,curr_dut_fail_mask);
		History_curr_dut_mask.SetAtGrow(index, curr_dut_mask);
		History_test_time.Add(sTt);
		if (flag) output(str_results);
	}//end of count check
}
int		TBCG::header(void)
{
	TB_Timer.StartTimer();
	TBNameStore();
	// use to summay every duts' state
	DutNumArray active_duts1;
	int count = active_duts_get(&active_duts1);
	int tmp = active_duts_get(&active_dut_mask);
	curr_dut_mask = active_dut_mask;

	if (count != 0)
	{
		results_get(&r1);	//	initialize Test result arrays
		results_get(&r2);
		results_get(&r3);
		results_get(&r4);
		results_get(&r5);
		results_get(&r6);
		results_get(&r7);
		results_get(&r8);
		results_get(&r9);
		results_get(&r10);
		results_get(&r11);
		results_get(&r12);
		results_get(&r13);
		results_get(&r14);
		results_get(&r15);
		results_get(&r16);
		results_get(&r17);
		results_get(&r18);
		results_get(&r19);
		results_get(&r20);
		results_get(&r21);
		results_get(&r22);
		results_get(&r23);
		results_get(&r24);
		results_get(&r25);
	}

	return TRUE;
}
int		TBCG::end(bool flag)
{
	display_test_results(flag);
	return MULTI_DUT;
}
void	TBCG::initial()
{
	TBCG_CSV.init();
	curr_dut_mask = 0;
	FailCnt.RemoveAll();
	FailCntHi.RemoveAll();
	TBNameList.RemoveAll();
	tblk2Var.empty();
	XArray.RemoveAll();
	YArray.RemoveAll();
}
void	TBCG::fresh()
{
	for (int i = 0; i < Max_TB_Num; i++)
		CLTB[i].RemoveAll();
}
void	TBCG::set_WaferInfo(CString lot, CString slot)
{
	Lot_ID = lot;
	Slot_ID = slot;
}
void	TBCG::set_Coordinate(int x, int y, int dut_num)
{
	int index = dut_num;
	XArray.SetAtGrow(index, x);
	YArray.SetAtGrow(index, y);
}
void	TBCG::value(CString label, int*		Var)
{
	CString sample;
	char * tblk_name = CString2Char(current_test_block());
	tblk2Var[tblk_name] = &CLTB[GetTBNameLocation()];
	for (int i = 0; i < max_dut()+1; i++)
	{
		sample.Format("%d", Var[i]);
		tblk2Var[tblk_name]->set_value(label,sample);
		sample.Empty();
	}
}
void	TBCG::value(CString label, double*	Var)
{
	CString sample;
	char * tblk_name = CString2Char(current_test_block());
	tblk2Var[tblk_name] = &CLTB[GetTBNameLocation()];

	for (int i = 0; i < max_dut() + 1; i++)
	{
		sample.Format("%f", Var[i]);
		tblk2Var[tblk_name]->set_value(label, sample);
		sample.Empty();
	}
}
void	TBCG::value(CString label, char**	Var)
{
	CString sample;
	char * tblk_name = CString2Char(current_test_block());
	tblk2Var[tblk_name] = &CLTB[GetTBNameLocation()];
	for (int i = 0; i < max_dut() + 1; i++)
	{
		sample.Format("%s", Var[i]);
		tblk2Var[tblk_name]->set_value(label, sample);
		sample.Empty();
	}
}
void	TBCG::value(CString label, UINT64*	Var)
{
	CString sample;
	char * tblk_name = CString2Char(current_test_block());
	tblk2Var[tblk_name] = &CLTB[GetTBNameLocation()];
	for (int i = 0; i < max_dut() + 1; i++)
	{
		sample.Format("%X", Var[i]);
		tblk2Var[tblk_name]->set_value(label, sample);
		sample.Empty();
	}
}
void	TBCG::value(CString label, bool*	Var)
{
	CString sample;
	char * tblk_name = CString2Char(current_test_block());
	tblk2Var[tblk_name] = &CLTB[GetTBNameLocation()];
	for (int i = 0; i < max_dut() + 1; i++)
	{
		sample.Format("%d", Var[i]);
		tblk2Var[tblk_name]->set_value(label, sample);
		sample.Empty();
	}
}
void	TBCG::Fileout(CString Path)
{
//Title Create
	int User_Area;
	TIMER.startTimer("1");
	if (first_touchdown)
	{
		TBCG_CSV.save_title("Event", 0);
		TBCG_CSV.save_title("LotID", 1);
		TBCG_CSV.save_title("SlotID", 2);
		TBCG_CSV.save_title("X", 3);
		TBCG_CSV.save_title("Y", 4);
		TBCG_CSV.save_title("Site", 5);
		TBCG_CSV.save_title("Dut", 6);
		//Test blocks
		for (int i = 0; i < tblk_list.GetSize(); i++)
			TBCG_CSV.save_title(tblk_list.GetAt(i), i + 7);
		User_Area = TBCG_CSV.get_MaxCol();
		SMutex();//ensure that all site get same value of User_Area
		int UValue_Index = 0;
		for (int i = 0; i < tblk_list.GetSize(); i++)
		{
			char *tblk_name;
			tblk2Var[tblk_name] = &CLTB[GetTBNameLocation()];
			if (CLTB[i].tbv_label.GetSize() != 0)
			{
				for (int j = 0; j < CLTB[i].tbv_label.GetSize(); j+= max_dut()+1)
				{
					UValue_Index++;
					int col = UValue_Index + User_Area;
					TBCG_CSV.save_title(CLTB[i].tbv_label.GetAt(j), col);
				}
			}
		}
	}
	TIMER.finishTimer("1");
	CL_Comment("title:" + Double2CString((TIMER.duration("1"))));
// growing area 
	//save Testtime
	TIMER.startTimer("1");
	TBCG_CSV.save_value("Event", 1, "TestTime");
	for (int i = 0; i < History_test_time.GetSize(); i++)
		TBCG_CSV.save_value(tblk_list.GetAt(i), 1, History_test_time.GetAt(i));

	int GSPos = TBCG_CSV.get_MaxRow();
	int related_Pos = 0;
	SoftwareOnlyActiveDutIterator loop;
	while (loop.More(TRUE))
	{
		DutNum dut = active_dut_get();
		TBCG_CSV.save_value("X",	(GSPos + related_Pos), Int2CString(XArray.GetAt(dut)));
		TBCG_CSV.save_value("Y",	(GSPos + related_Pos), Int2CString(XArray.GetAt(dut)));
		TBCG_CSV.save_value("Site", (GSPos + related_Pos), Int2CString(site_num()));
		TBCG_CSV.save_value("Dut",	(GSPos + related_Pos), Int2CString(dut+1));
		// P & F
		for (int i = 0; i < History_test_time.GetSize(); i++)
		{
			UINT64 fail_mask = History_curr_dut_fail_mask.GetAt(i);
			UINT64 active_mask = History_curr_dut_mask.GetAt(i);
			if (fail_mask&((UINT64)0x1 << dut))
				TBCG_CSV.save_value(tblk_list.GetAt(i), (GSPos + related_Pos), "F");
			else
			{
				if (active_mask & ((UINT64)0x1 << dut))
					TBCG_CSV.save_value(tblk_list.GetAt(i), (GSPos + related_Pos), "P");
				else
					TBCG_CSV.save_value(tblk_list.GetAt(i), (GSPos + related_Pos), "-");
			}
		}
		//user value 
		for (int i = 0; i < tblk_list.GetSize(); i++)
		{
			if (CLTB[i].tbv_label.GetSize() != 0)
			{
				if (CLTB[i].tbv_label.GetSize() % (max_dut() + 1) != 0)
					fatal("[CL_Improve]Something wrong happened in TBCG::Fileout");
				int num = (CLTB[i].tbv_label.GetSize()) / (max_dut() + 1);
				for (int j = 0; j < num; j++)
					TBCG_CSV.save_value(CLTB[i].tbv_label.GetAt(dut + j*(max_dut() + 1)), (GSPos + related_Pos), CLTB[i].tbv_value.GetAt(dut + j*(max_dut() + 1)));
			}
		}
		related_Pos++;
	}
	TIMER.finishTimer("1");
	CL_Comment("value :" + Double2CString((TIMER.duration("1"))));
// print out
	if (first_touchdown)
	{
		first_touchdown = 0;
		if (Lot_ID == "" || Slot_ID == "")
		{
			demo_path = Path + "\\Demo_" + CTime::GetCurrentTime().Format("%y.%m.%d");
			FileName_site = demo_path + "\\Demo_Site" + Int2CString(site_num()) + CTime::GetCurrentTime().Format("_%H.%M.%S.csv");
			FileName = demo_path + "\\Demo" + CTime::GetCurrentTime().Format("_%y.%m.%d_%H.%M.%S.csv");
		}
		else
		{
			demo_path = Path + "\\" + Lot_ID + "#" + Slot_ID + CTime::GetCurrentTime().Format("%y.%m.%d");
			FileName_site = demo_path + "\\" + Lot_ID + "#" + Slot_ID + "_site" + Int2CString(site_num()) + CTime::GetCurrentTime().Format("_%H.%M.%S.csv");
			FileName = demo_path + "\\" + Lot_ID + "#" + Slot_ID + CTime::GetCurrentTime().Format("_%y.%m.%d_%H.%M.%S.csv");
		}
		CC_Path(demo_path);
		CPy_Datalog_Merge(demo_path + Py_Datalog_Merge_File);
		remote_set(Datalog_Merge_Py, demo_path + Py_Datalog_Merge_File, 0, FALSE, 0);
		remote_set(Datalog_output_File, FileName, 0, FALSE, 0);
		remote_set(Datalog_File, FileName_site, 0, TRUE, INFINITE);
	}
	TBCG_CSV.makeCSV(FileName_site);
	SMutex();
	remote_send(Datalog_Merge,0,TRUE,INFINITE);
}
void	TBCG::Show_linking()
{
	char * tblk_name = CString2Char(current_test_block());
	tblk2Var[tblk_name] = &CLTB[GetTBNameLocation()];
	for (int j = 0; j < tblk2Var[tblk_name]->tbv_label.GetSize(); j++)
	{
		output("%s:\\",tblk2Var[tblk_name]->tbv_label.GetAt(j));
		output("%s", tblk2Var[tblk_name]->tbv_value.GetAt(j));
	}
}
//inside
void	TBCG::TBNameStore()
{
	bool flag = 1;
	for (int i = 0; i < TBNameList.GetSize(); i++)
	{ 
		if (TBNameList.GetAt(i) == current_test_block())
			flag = 0;
	}
	if (flag)
	{
		TBNameList.Add(current_test_block());
	}

}
bool	TBCG::TBFirstAppear()
{
	for (int i = 0; i < TBNameList.GetSize(); i++)
	{
		if (TBNameList.GetAt(i) == current_test_block())
			return FALSE;
	}
	return TRUE;
}
int		TBCG::GetTBNameLocation(void)
{
	for (int i = 0; i < TBNameList.GetSize(); i++)
	{
		if (TBNameList.GetAt(i) == current_test_block())
			return i;
	}
	return -1;
}

CString TBCG::SwitchU(double value, e_log_type log_type){
	CString cstr;

	switch (log_type) {
	case LOG_V:	 // convert to voltage
		if (fabs(value) >= (1 V)) {
			cstr.Format("%8.3f v", value / (1 V));
		}
		else {
			cstr.Format("%8.3f mv", value / (1 MV));
		}
		break;
	case LOG_I:  // convert to current
		if (fabs(value) >= (1 MA)) {
			cstr.Format("%8.3f ma", value / (1 MA));
		}
		else if (fabs(value) >= (1 UA)) {
			cstr.Format("%8.3f ua", value / (1 UA));
		}
		else {
			cstr.Format("%8.0f na", value / (1 NA));
		}
		break;
	case LOG_T: break;
	default:	cstr.Format("#############");
	}

	return (cstr);
}  // end of tp_cnvt_result
void	TBCG::Pdatalog(PinList *pPinList, IVMeasType meas_type) //call after a parametric test
{
	remote_fetch(CL_dlog, 0, FALSE, INFINITE);

	if (CL_dlog < 1)	return;


	// determine the what type of test was just run
	e_log_type log_type, frc_type;
	switch (parametric_mode()) {
	case 0:	log_type = LOG_I; frc_type = LOG_V; break;
	case 2:	log_type = LOG_I; frc_type = LOG_V; break;
	case 1:	log_type = LOG_V; frc_type = LOG_I; break;
	default: output("Invalid parametric mode : %d", parametric_mode());
	}

	if (CL_dlog > 1) {
		if (measure() == TRUE) {
			output("--------------------------------------------------------------------------------------------------");
			output("Tester  Pin                      Pin         Force            Low         High            Measured");
			output(" Pin    Name                    Result       Value            Limit       Limit           Value   ");
			output("--------------------------------------------------------------------------------------------------");
		}

	}  
	int count = active_duts_get(&last_active_duts);
	for (int ix = 0; ix<count; ix++) {
		DutNum dut_num = last_active_duts[ix];
		active_duts_enable(dut_num);		
		// Print header info

		CArray<double, double> meas_results;
		CArray<PFState, PFState> pf_results;

		// check the pinlist for either dps pins or signal pins
		// get the measured results for this dut
		if (all_dps(pPinList)){
			Dps_meas(meas_results);
			Dps_pf(pf_results);
		}
		else if (all_hv(pPinList)) {
			Hv_pf(pf_results);
			Hv_meas(meas_results);
		}
		else if (all_dpo(pPinList))
		{
			Dpo_meas(meas_results);
			Dpo_pf(pf_results);
		}
		else if (all_dpohv(pPinList))
		{
			DpoHv_meas(meas_results);
			DpoHv_pf(pf_results);
		}
		else {
			Pin_meas(meas_results);
			Pin_pf(pf_results);
		}

		if (CL_dlog > 1) 
			output("Dut %d : %s", dut_num + 1, result_get(dut_num) ? "<COLOR=Green>PASS</COLOR>" : "<COLOR=Red>FAIL</COLOR>");

		int meas_index = 0;

		// Loop through all dut pins
		DutPin * dpin;
		for (int dpi = 0; pin_info(pPinList, dpi, &dpin); ++dpi) {

			// Get the HDTesterPin for this dut
			HDTesterPin tpin;
			pin_info(dpin, dut_num, &tpin);
			if (tpin == t_na)
				continue;

			CString pin_name = resource_name(dpin);
			CString tpin_name = testerpin_name(tpin);

			if (measure())
			{
				if (no_dps(pPinList) && no_dpo(pPinList) && no_dpohv(pPinList))
				{
					switch (CL_dlog)
					{
					case 0:
					case 1:
						break;
					case 2: //FAIL Only
						if (!pf_results[meas_index])
							output("%-10s\t%-10s\t\t%4s\t\t%s\t%s\t%s\t%s", tpin_name, pin_name,
							"<COLOR=Red>FAIL</COLOR>",
							frc_type == LOG_I ? SwitchU(ipar_force(dpin), frc_type) : SwitchU(vpar_force(dpin), frc_type),
							frc_type == LOG_I ? SwitchU(vpar_low(dpin), log_type) : SwitchU(ipar_low(dpin), log_type),
							frc_type == LOG_I ? SwitchU(vpar_high(dpin), log_type) : SwitchU(ipar_high(dpin), log_type),
							SwitchU(meas_results[meas_index], log_type));
						break;
					case 3: //PASS only
						if (pf_results[meas_index])
							output("%-10s\t%-10s\t\t%4s\t\t%s\t%s\t%s\t%s", tpin_name, pin_name,
							"<COLOR=Green>PASS</COLOR>",
							frc_type == LOG_I ? SwitchU(ipar_force(dpin), frc_type) : SwitchU(vpar_force(dpin), frc_type),
							frc_type == LOG_I ? SwitchU(vpar_low(dpin), log_type) : SwitchU(ipar_low(dpin), log_type),
							frc_type == LOG_I ? SwitchU(vpar_high(dpin), log_type) : SwitchU(ipar_high(dpin), log_type),
							SwitchU(meas_results[meas_index], log_type));
						break;
					case 4: //All
						output("%-10s\t%-10s\t\t%4s\t\t%s\t%s\t%s\t%s", tpin_name, pin_name,
							pf_results[meas_index] ? "<COLOR=Green>PASS</COLOR>" : "<COLOR=Red>FAIL</COLOR>",
							frc_type == LOG_I ? SwitchU(ipar_force(dpin), frc_type) : SwitchU(vpar_force(dpin), frc_type),
							frc_type == LOG_I ? SwitchU(vpar_low(dpin), log_type) : SwitchU(ipar_low(dpin), log_type),
							frc_type == LOG_I ? SwitchU(vpar_high(dpin), log_type) : SwitchU(ipar_high(dpin), log_type),
							SwitchU(meas_results[meas_index], log_type));
						break;
					}
				}
				else if (all_dpohv(pPinList))
				{
					switch (CL_dlog)
					{
					case 0:
					case 1:
						break;
					case 2: //FAIL Only
						if (!pf_results[meas_index])
							output("%-10s\t%-10s\t\t%4s\t\t%s\t%s\t%s\t%s", tpin_name, pin_name,
							"<COLOR=Red>FAIL</COLOR>",
							frc_type == LOG_I ? SwitchU(dpo_hv_ipar_force_get(dpin), frc_type) : SwitchU(dpo_hv_voltage_get(dpin), frc_type),
							frc_type == LOG_I ? SwitchU(dpo_hv_vpar_low_get(dpin), log_type) : SwitchU(dpo_hv_ipar_low_get(dpin), log_type),
							frc_type == LOG_I ? SwitchU(dpo_hv_vpar_high_get(dpin), log_type) : SwitchU(dpo_hv_ipar_high_get(dpin), log_type),
							SwitchU(meas_results[meas_index], log_type));
						break;
					case 3: //PASS only
						if (pf_results[meas_index])
							output("%-10s\t%-10s\t\t%4s\t\t%s\t%s\t%s\t%s", tpin_name, pin_name,
							"<COLOR=Green>PASS</COLOR>",
							frc_type == LOG_I ? SwitchU(dpo_hv_ipar_force_get(dpin), frc_type) : SwitchU(dpo_hv_voltage_get(dpin), frc_type),
							frc_type == LOG_I ? SwitchU(dpo_hv_vpar_low_get(dpin), log_type) : SwitchU(dpo_hv_ipar_low_get(dpin), log_type),
							frc_type == LOG_I ? SwitchU(dpo_hv_vpar_high_get(dpin), log_type) : SwitchU(dpo_hv_ipar_high_get(dpin), log_type),
							SwitchU(meas_results[meas_index], log_type));
						break;
					case 4: //All
						output("%-10s\t%-10s\t\t%4s\t\t%s\t%s\t%s\t%s", tpin_name, pin_name,
							pf_results[meas_index] ? "<COLOR=Green>PASS</COLOR>" : "<COLOR=Red>FAIL</COLOR>",
							frc_type == LOG_I ? SwitchU(dpo_hv_ipar_force_get(dpin), frc_type) : SwitchU(dpo_hv_voltage_get(dpin), frc_type),
							frc_type == LOG_I ? SwitchU(dpo_hv_vpar_low_get(dpin), log_type) : SwitchU(dpo_hv_ipar_low_get(dpin), log_type),
							frc_type == LOG_I ? SwitchU(dpo_hv_vpar_high_get(dpin), log_type) : SwitchU(dpo_hv_ipar_high_get(dpin), log_type),
							SwitchU(meas_results[meas_index], log_type));
						break;
					}
				}

				else if (all_dpo(pPinList))
				{
					switch (CL_dlog)
					{
					case 0:
					case 1:
						break;
					case 2: //FAIL Only
						if (!pf_results[meas_index])
							output("%-10s\t%-10s\t\t%4s\t\t%s\t%s\t%s\t%s", tpin_name, pin_name,
							"<COLOR=Red>FAIL</COLOR>",
							frc_type == LOG_I ? SwitchU(dpo_ipar_force_get(dpin), frc_type) : SwitchU(dpo_voltage_get(dpin), frc_type),
							frc_type == LOG_I ? SwitchU(dpo_voltage_low_get(dpin), log_type) : SwitchU(dpo_current_low_get(dpin), log_type),
							frc_type == LOG_I ? SwitchU(dpo_voltage_high_get(dpin), log_type) : SwitchU(dpo_current_high_get(dpin), log_type),
							SwitchU(meas_results[meas_index], log_type));
						break;
					case 3: //PASS only
						if (pf_results[meas_index])
							output("%-10s\t%-10s\t\t%4s\t\t%s\t%s\t%s\t%s", tpin_name, pin_name,
							"<COLOR=Green>PASS</COLOR>",
							frc_type == LOG_I ? SwitchU(dpo_ipar_force_get(dpin), frc_type) : SwitchU(dpo_voltage_get(dpin), frc_type),
							frc_type == LOG_I ? SwitchU(dpo_voltage_low_get(dpin), log_type) : SwitchU(dpo_current_low_get(dpin), log_type),
							frc_type == LOG_I ? SwitchU(dpo_voltage_high_get(dpin), log_type) : SwitchU(dpo_current_high_get(dpin), log_type),
							SwitchU(meas_results[meas_index], log_type));
						break;
					case 4: //All
						output("%-10s\t%-10s\t\t%4s\t\t%s\t%s\t%s\t%s", tpin_name, pin_name,
							pf_results[meas_index] ? "<COLOR=Green>PASS</COLOR>" : "<COLOR=Red>FAIL</COLOR>",
							frc_type == LOG_I ? SwitchU(dpo_ipar_force_get(dpin), frc_type) : SwitchU(dpo_voltage_get(dpin), frc_type),
							frc_type == LOG_I ? SwitchU(dpo_voltage_low_get(dpin), log_type) : SwitchU(dpo_current_low_get(dpin), log_type),
							frc_type == LOG_I ? SwitchU(dpo_voltage_high_get(dpin), log_type) : SwitchU(dpo_current_high_get(dpin), log_type),
							SwitchU(meas_results[meas_index], log_type));
						break;
					}
				}
				else
				{
					if (CL_dlog > 1)
						output("%-10s\t%-10s\t\t%4s\t\t%s\t%s\t%s\t%s", tpin_name, pin_name,
						pf_results[meas_index] ? "<COLOR=Green>PASS</COLOR>" : "<COLOR=Red>FAIL</COLOR>",
						//bugs here  add judgement
						SwitchU(meas_type == 0 ? dps(dpin) : ipar_force(dpin), frc_type),
						//bugs here(change dps_current_low( dpin ) to vpar_low(dpin))  ----colin
						SwitchU(meas_type == 0 ? ipar_low(dpin) : vpar_low(dpin), log_type),
						SwitchU(meas_type == 0 ? ipar_high(dpin) : vpar_high(dpin), log_type),
						SwitchU(meas_results[meas_index], log_type));
				}
			}
			else{
				if (CL_dlog > 1)
					output("%-10s   %-10s   %4s", tpin_name, pin_name,
					pf_results[meas_index] ? "<COLOR=Green>PASS</COLOR>" : "<COLOR=Red>FAIL</COLOR>");
			}
			++meas_index;
		}
		ASSERT(meas_index == meas_results.GetSize());
	}

	// restore original duts
	active_duts_enable(last_active_duts);
}
void	TBCG::Fdatalog(PinList* pPinList)
{
	TesterPin tpin;
	HDTesterPin hd_tpin;
	DutPin * dpin;
	DutNum	dut_num;
	CString pin_name;
	PFState pass_fail;


	remote_fetch(CL_dlog, 0, FALSE, INFINITE);

	int count = active_duts_get(&last_active_duts);  // get a list of active devices

	if (test_pin(pPinList))
	{
		if (CL_dlog > 1) output("TestBlock:%-15s TestNumber:%-3d <COLOR=Green>PASSed</COLOR>", current_test_block(), test_number());
	}
	else
	{
		for (int ix = 0; ix<count; ix++) {
			dut_num = last_active_duts[ix];

			if (CL_dlog > 1) output("Dut %d : %s\n", last_active_duts[ix] + 1, result_get(last_active_duts[ix]) ? "<COLOR=Green>PASS</COLOR>" : "<COLOR=Red>FAIL</COLOR>");

			// Loop through all pins and look for pass/fail
			for (int pin_index = 0; pin_info(pPinList, pin_index, &dpin); pin_index++) {

				pin_info(dpin, dut_num, &tpin);			// get the tester channel based on DutPin

				if (tpin == t_na)
					continue;

				CString pin_name = resource_name(dpin);
				CString tpin_name = testerpin_name(tpin);

				// create a temporary pinlist with only one pin
				PinList *pPin = pinlist_create(&tpin, 1);
				pass_fail = test_pin(pPin);
				pinlist_destroy(pPin);

				pin_info(dpin, dut_num, &hd_tpin);			// get the tester channel based on DutPin
				CString hd_tpin_name = testerpin_name(hd_tpin);

				pin_name = resource_name(dpin);

				switch (CL_dlog)
				{
				case 0:
				case 1:
					break;
				case 2:	//Fail Only
					if (pass_fail == FAIL)
					{
						output("Pin = %6s, tester channel = %s  %s", pin_name,
							hd_tpin_name,
							"<COLOR=Red>FAIL</COLOR>");
					}
					break;
				case 3:	//Pass Only
					if (pass_fail == PASS)
					{
						output("Pin = %6s, tester channel = %s  %s", pin_name,
							hd_tpin_name,
							"<COLOR=Green>PASS</COLOR>");
					}
					break;
				case 4:	//All
					output("Pin = %6s, tester channel = %s  %s", pin_name,
						hd_tpin_name,
						pass_fail ? "<COLOR=Green>PASS</COLOR>" : "<COLOR=Red>FAIL</COLOR>");
					break;
				}
			}
		}
	}
}