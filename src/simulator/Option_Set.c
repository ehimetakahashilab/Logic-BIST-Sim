#include "declare.h"
#include "def_gtype.h"
#include "def_flt.h"

//#define TEST_VEC "30000" //注意！hamada
#define PATH_LAST "SIM_RESULT/LAST/"
#define PATH_MIDDLE "SIM_RESULT/MIDDLE/"
#define PATH_TOGGLE "test/TOGGLE_RATE/"

ff_sta_src_read(src_num, ob_num, argv) int src_num;
int ob_num;
char *argv[1];
{
	int i, ia, ib, ic;
	FILE *fin;
	char PATH[30];
	int isel[src_num];
	FIN_NODE *finnode;
	i = 0;
	while (i < src_num)
	{
		sprintf(PATH, "ff_station_%d.dat", i);
		if ((fin = fopen(PATH, "rt")) == NULL)
			printf("error:'FF Station File' %s is not found!\n", PATH), exit(1);
		for (ia = 0; ia < ffnum; ia++)
		{
			if (ia < ob_num)
			{
				fscanf(fin, "%d", &isel[i]);
				//printf("%d, ",isel[i]);
				finnode = ffnode.next;
				for (ib = 1; finnode != NULL; finnode = finnode->next, ib++)
				{
					if (isel[i] == ib)
					{
						finnode->node->sel_flog[i] = 1;
						break;
					}
				}
			}
		}
		i++;
		fclose(fin);
		memset(PATH, '\0', 30);
	}
	/*
		finnode=ffnode.next;ib=0;
		for(; finnode!=NULL; finnode=finnode->next){
			if(finnode->node->sel_flog[0]==1) {ib++;
			printf("\n%d, %d",finnode->node->line,ib);
				}}
exit(1);*/
}

Instance_Get(argc, argv) int argc;
char *argv[14];
{
	int ia, ib, ic;
	/*External Parameter Setting*/
	printf("External Parameter Setting\n");
	MODE_TOOL = atoi(argv[2]);
	printf("TOOL_MODE=%d\n", MODE_TOOL);
	switch (MODE_TOOL)
	{
	case 1:
		printf("Tool: Normal BIST Testing Mode\n");
		if (argc > 5)
		{
			printf("error: too much arguments!\n");
			printf("Please Setting the arguments for \nargv1: Tool Mode\nargv2:TPG_MODE\n argv3: Circuit\nargv4: Test_vector_generator path\n");
			exit(1);
		}
		TPG_MODE = atoi(argv[3]);
		if (TPG_MODE == 0)
			printf("TPG: %d bit LFSR \n", LFSR_BIT);
		else
			printf("TPG: In House ATPG\n");
#if TRANSITIONFAULT
		printf("Fault Model: Transition Delay Faults\n");
		cap_freq = 2;
#else
		printf("Fault Model: Stuck-at Faults\n");
		cap_freq = 1;
#endif
		break;
	case 2:
	case 3:
		TPG_MODE = atoi(argv[3]);
		if (TPG_MODE == 0)
			printf("TPG: %d bit LFSR \n", LFSR_BIT);
		else
			printf("TPG: In House ATPG\n");
		cap_freq = atoi(argv[4]);
		SKIP_CAPTURE = atoi(argv[5]);

		if (MODE_TOOL == 2)
		{
			printf("Tool: BIST Multi-Cycle Testing Mode\n");
			if (argc > 6)
			{
				printf("error: too much arguments!\n");
				printf("Please Setting the arguments for \nargv1: Tool Mode\nargv2:TPG_MODE\n argv3: Circuit\nargv4: Test_vector_generator path\n");
				exit(1);
			}
		}
		else
		{
			printf("Tool: BIST Multi-Cycle Testing With Sequential Observation Mode\n");
			if (argc > 8)
			{
				printf("error: too much arguments!\n");
				printf("Please Setting the arguments for \nargv1: Tool Mode\nargv2:TPG_MODE\n argv3: Circuit\nargv4: Test_vector_generator path\n");
				exit(1);
			}
			OBSERVE_RATE = atof(argv[6]);
			FF_FILE = atoi(argv[7]);
			printf("***************************************\nWarning: You would have to check the following setting at 'declare.h'\n");
			printf("PO観測:PO_OBSERVE  = %d\n", PO_OBSERVE);
	  	printf("FF観測モード:SELECT_STATION = %d\n", SELECT_STATION);
			 /* 1 when select FF station 0: Full observation*/

			flt_det_flog = (int **)malloc((sum_flt + 2) * sizeof(int *));
			if (flt_det_flog == NULL)
				printf("memory error @flt_det_flog in flt_info \n"), exit(1);

			for (ia = 0; ia <= sum_flt + 1; ia++)
			{
				flt_det_flog[ia] = (int *)malloc(11 * sizeof(int));
				if (flt_det_flog[ia] == NULL)
					printf("memory error @flt_det_flog \n"), exit(1);
			}
		}

#if FAULTOB
		/*2進数でパターンごと、FFごと、キャプチャごとの故障情報を記録する＿＿2015710_王*/
		Pat_FF_Faults = (FF_PAT_FLT_LIST **)calloc(length + 1, sizeof(FF_PAT_FLT_LIST *));
		if (Pat_FF_Faults == NULL)
			printf("memory error @FF_Faults_lists \n"), exit(1);
		for (ia = 0; ia < length + 1; ia++)
		{
			Pat_FF_Faults[ia] = (FF_PAT_FLT_LIST *)calloc(ffnum, sizeof(FF_PAT_FLT_LIST));
			if (Pat_FF_Faults[ia] == NULL)
				printf("memory error @Pat_FF_Faults for %d %d\n", length, ia), exit(1);
		}
		for (ia = 0; ia < length + 1; ia++)
			for (ib = 0; ib < ffnum; ib++)
			{
				printf("here? %d \n\n", ia);
				Pat_FF_Faults[ia][ib].Last_Cap_Faults = (unsigned int *)calloc(sum_flt / 32 + 1, sizeof(unsigned int));
				Pat_FF_Faults[ia][ib].OB_Cap_Faults = (unsigned int *)calloc(sum_flt / 32 + 1, sizeof(unsigned int));
				Pat_FF_Faults[ia][ib].Total_Faults = (unsigned int *)calloc(sum_flt / 32 + 1, sizeof(unsigned int));
				if (Pat_FF_Faults[ia][ib].Last_Cap_Faults == NULL || Pat_FF_Faults[ia][ib].OB_Cap_Faults == NULL || Pat_FF_Faults[ia][ib].Total_Faults == NULL)
					printf("memory error @Pat_FF_Faults for %d %d\n", length, ia), exit(1);
			}

#endif

		break;

	case 4:
		printf("Tool: Logic CP Insertion MODE\n");
		if (argc > 14)
		{
			printf("error: too much arguments!\n");
			printf("Please Setting the arguments for \nargv1: Tool Mode\nargv2:TPG_MODE\n argv3: Circuit\nargv4: Test_vector_generator path\n");
			exit(1);
		}
		TPG_MODE = atoi(argv[3]);
		if (TPG_MODE == 0){
			printf("TPG: %d bit LFSR \n", LFSR_BIT);
		}
		else
			printf("TPG: In House ATPG\n");

		TGL_GATE_MODE = atoi(argv[4]); //=1: gate TPI, =2:FF TPI

		if (TGL_GATE_MODE == 0 || TGL_GATE_MODE == 2 || TGL_GATE_MODE == 3)
		{
			ff_rate = atof(argv[5]);
			cap_freq = atoi(argv[6]);
			INTERVAL_CYCLE = atoi(argv[7]);
			SKIP_CYCLE = atoi(argv[8]);
			FF_SEL_METHOD = atoi(argv[9]);
			FF_FILE = atoi(argv[10]);
			OBSERVE_RATE = atof(argv[11]);
			for (ia = 0; ia < FF_FILE; ia++)
				flt_det_num[ia] = 0;
			flt_det_num[20] = 0;
			length=atoi(argv[12]);
		}
		else
		{

			Tgl_rate = atof(argv[5]);
			cap_freq = atoi(argv[6]);
			//TG_FILE = atoi(argv[7]);
			INTERVAL_CYCLE = atoi(argv[7]);
			SKIP_CYCLE = atoi(argv[8]);
			FF_FILE = atoi(argv[9]);
			OBSERVE_RATE = atof(argv[10]);
			group_tpi=atoi(argv[11]);

			for (ia = 0; ia < FF_FILE; ia++)
				flt_det_num[ia] = 0;
			flt_det_num[20] = 0;
		}

		printf("Toggle Gate Insertion Mode : %d\n", TGL_GATE_MODE);
		printf("Toggle Gate Insertion Rate : %f\n", Tgl_rate);
		printf("# of Capture Cycles: %d\n", cap_freq);
		printf("Toggle INTERVAL_CYCLE: %d\n", INTERVAL_CYCLE);

		flt_det_flog = (int **)malloc((sum_flt + 2) * sizeof(int *));
		if (flt_det_flog == NULL)
			printf("memory error @flt_det_flog in flt_info \n"), exit(1);

		for (ia = 0; ia <= sum_flt + 1; ia++)
		{
			flt_det_flog[ia] = (int *)malloc(11 * sizeof(int));
			if (flt_det_flog[ia] == NULL)
				printf("memory error @flt_det_flog \n"), exit(1);
		}

		SKIP_CAPTURE = 0;
		//OBSERVE_RATE=atof(argv[8]);
#if TRANSITIONFAULT
		printf("Fault Model: Transition Deley Fault \n");
#else
		printf("Fault Model: Stuck-at Fault \n");
#endif
#if FAULTSIMULATION
		printf("Fault Simulation: Enable\n");
#if FAULTDROP
		printf("Fault Simulation Mode: Fault Dropping\n");
#endif
#else
		printf("Fault Simulation: Disable\n  warning: only Logic Simulation will be performed\n    if you want Fault simulation, please go setting at 'declare.h'\n\n");
#endif

		break;
	default:
		printf("No New Function is Supported by this Program!\n"), exit(1);
	}
}

Out_Put(argv) char *argv[13];
{
	char outpath[200];
	FILE *fout;
	int ia, ib, ic, id;
	switch (MODE_TOOL)
	{
	case 1:
		remain_flt = count_flt(fltlst.next);
		sprintf(outpath, "./OUTPUTS/BIST/%s.txt", argv[1]);
		if ((fout = fopen(outpath, "w")) == NULL)
			printf("BIST output file is not exist!\n"), exit(1);
		printf("*************OUTPUT RESULTS****************\n");
		printf("#Test Pat., #Faults, #Det. Flts, #UnDet.Flts, Fcov.\n");
		fprintf(fout, "#Test Pat., #Faults, #Det. Flts, #UnDet.Flts, Fcov.\n");
		printf("%8d,%8d,%8d,%8d,  %4.4f\n", length, sum_flt, sum_flt - remain_flt, remain_flt, (1 - (float)remain_flt / (float)sum_flt) * 100.0);
		fprintf(fout, "%8d,%8d,%8d,%8d,  %4.4f\n", length, sum_flt, sum_flt - remain_flt, remain_flt, (1 - (float)remain_flt / (float)sum_flt) * 100.0);
#if POWEREVA
		printf("Test Power Evaluation \n#SHIFT_IN,  SHIFT_OUT, SHIFT\n");
		fprintf(fout, "Test Power Evaluation \n#SHIFT_IN,  SHIFT_OUT, SHIFT");
#if PEAK
		fprintf(fout, "SHIFT_TGL_RATE, PEAK TGL");
#endif
		fprintf(fout, "\n");
		fprintf(fout, "%.0lf,%.0lf,%.0lf,%.0lf", (float)toggle_scn_in, (float)toggle_scn_out, (float)toggle_scn, (float)toggle_scn_max);
#if PEAK
		float Max = 0;
		for (ia = 0; ia < chainnum; ia++)
		{
			if (ShiftPeak[ia] / (schain[ia].length - 1) > Max)
				Max = ShiftPeak[ia] / (schain[ia].length - 1);
		}
		fprintf(fout, "%4.3f, %4.3f", (float)toggle_scn / (float)toggle_scn_max * 100.0, Max * 100);
#endif
		fprintf(fout, "\n");
#if WSA_REC
		printf("Capture Power \n#Cap=%d\n", cap_freq);
		for (ia = 1; ia <= cap_freq; ia++)
			fprintf(fout, ",%.0f", WSA[ia] / clocktime); // exit(1);
		fprintf(fout, ",%.0f", MaxWSA);
#endif
#endif
		fprintf(fout, "*************OUTPUT END****************\n");
		fclose(fout);
		printf("*************OUTPUT END****************\n");
		break;
	case 2:
		remain_flt = count_flt(fltlst.next);
		//printf("here?\n"); exit(1);
		sprintf(outpath, "./OUTPUTS/MULTI_BIST/%s.txt", argv[1]);
		if ((fout = fopen(outpath, "w")) == NULL)
			printf("MULTI_BIST output file is not exist!\n"), exit(1);
		printf("*************OUTPUT RESULTS****************\n");
		printf("#Test Pat.,	#Faults,	#Det. Flts,	#UnDet.Flts,	Fcov.\n");
		fprintf(fout, "#Test Pat.,	#Faults,	#Det. Flts,	#UnDet.Flts,	Fcov.\n");
		printf("%10d,%10d,%10d,%10d,%4.6f\n", length, sum_flt, sum_flt - remain_flt, remain_flt, (1 - (float)remain_flt / (float)sum_flt) * 100.0);
		fprintf(fout, "%10d,%10d,%10d,%10d,%4.6f\n", length, sum_flt, sum_flt - remain_flt, remain_flt, (1 - (float)remain_flt / (float)sum_flt) * 100.0);
#if POWEREVA
		printf("Test Power Evaluation \n#SHIFT_IN,  SHIFT_OUT, SHIFT\n");
		fprintf(fout, "Test Power Evaluation \n#SHIFT_IN,  SHIFT_OUT, SHIFT");
#if PEAK
		fprintf(fout, "SHIFT_TGL_RATE, PEAK TGL");
#endif
		fprintf(fout, "\n");
		fprintf(fout, "%.0lf,%.0lf,%.0lf,%.0lf", (float)toggle_scn_in, (float)toggle_scn_out, (float)toggle_scn, (float)toggle_scn_max);
#if PEAK
		float Max = 0;
		for (ia = 0; ia < chainnum; ia++)
		{
			if (ShiftPeak[ia] / (schain[ia].length - 1) > Max)
				Max = ShiftPeak[ia] / (schain[ia].length - 1);
		}
		fprintf(fout, "%4.3f, %4.3f", (float)toggle_scn / (float)toggle_scn_max * 100.0, Max * 100);
#endif
		fprintf(fout, "\n");

#if FF_CAP
		FIN_NODE *finnode;
		L_NODE *fnode;
		char outpath2[200];
		FILE *fout2;
		sprintf(outpath2, "./OUTPUTS/MULTI_BIST/FF_TOGGLE_%s_%d.csv", argv[1], cap_freq);
		if ((fout2 = fopen(outpath2, "w")) == NULL)
			printf("FF_TOGGLE output file is not exist!\n"), exit(1);
		finnode = ffnode.next;
		for (ia = 0; finnode != NULL; finnode = finnode->next, ia++)
		{
			fnode = finnode->node;
			printf("%d,%d,", ia + 1, fnode->line);
			fprintf(fout2, "%d,%d,", ia + 1, fnode->line);
			for (ib = 1; ib <= cap_freq; ib++)
			{
				printf("%d,", fnode->toggle_cap[ib]);
				fprintf(fout2, "%d,", fnode->toggle_cap[ib]);
			}
			printf("\n");
			fprintf(fout2, "\n");
		}
		fclose(fout2);
#endif
#if WSA_REC
		printf("Capture Power \n#Cap=%d\n", cap_freq);
		for (ia = 1; ia <= cap_freq; ia++)
			fprintf(fout, ",%.0f", WSA[ia] / clocktime); // exit(1);
		fprintf(fout, ",%.0f", MaxWSA);
#endif
#endif
		fprintf(fout, "\n*************OUTPUT END****************\n");
		fclose(fout);
		printf("\n*************OUTPUT END****************\n");
		break;

	case 3:
#if SELECT_STATION
		count_flt(fltlst.next);
		sprintf(outpath, "./OUTPUTS/MULTI_BIST_OB/%s.txt", argv[1]);
		if ((fout = fopen(outpath, "w")) == NULL)
			printf("MULTI_BIST_OB output file is not exist!\n"), exit(1);
		printf("\n*************OUTPUT RESULTS****************\n");
		printf("#Test Pat.,	#Faults,	#Det. Flts,	#UnDet.Flts,	Fcov.\n");
		fprintf(fout, "#Test Pat.,	#Faults,	#Det. Flts,	#UnDet.Flts,	Fcov.\n");
		printf("%10d,%10d", length, sum_flt);
		fprintf(fout, "%10d,%10d", length, sum_flt);
		for (ia = 0; ia < FF_FILE; ia++)
		{
			printf(",%10d,%10d,%4.6f", flt_det_num[ia], sum_flt - flt_det_num[ia], (float)flt_det_num[ia] / (float)sum_flt * 100.0);
			fprintf(fout, ",%10d,%10d,%4.6f", flt_det_num[ia], sum_flt - flt_det_num[ia], (float)flt_det_num[ia] / (float)sum_flt * 100.0);
		}
		printf("\n");
		fprintf(fout, "\n");

#else
		remain_flt = count_flt(fltlst.next);
		sprintf(outpath, "./OUTPUTS/MULTI_FULL_OB_BIST/%s.txt", argv[1]);
		if ((fout = fopen(outpath, "w")) == NULL)
			printf("MULTI_OB_BIST output file is not exist!\n"), exit(1);
		printf("\n*************OUTPUT RESULTS****************\n");
		printf("#Test Pat.,	#Faults,	#Det. Flts,	#UnDet.Flts,	Fcov.\n");
		fprintf(fout, "#Test Pat.,	#Faults,	#Det. Flts,	#UnDet.Flts,	Fcov.\n");
		printf("%10d,%10d,%10d,%10d,%4.6f\n", length, sum_flt, sum_flt - remain_flt, remain_flt, (1 - (float)remain_flt / (float)sum_flt) * 100.0);
		fprintf(fout, "%10d,%10d,%10d,%10d,%4.6f\n", length, sum_flt, sum_flt - remain_flt, remain_flt, (1 - (float)remain_flt / (float)sum_flt) * 100.0);
#endif
#if POWEREVA
		printf("Test Power Evaluation \n#SHIFT_IN,  SHIFT_OUT, SHIFT\n");
		fprintf(fout, "Test Power Evaluation \n#SHIFT_IN,  SHIFT_OUT, SHIFT");
#if PEAK
		fprintf(fout, "SHIFT_TGL_RATE, PEAK TGL");
#endif
		fprintf(fout, "\n");
		fprintf(fout, "%.0lf,%.0lf,%.0lf,%.0lf", (float)toggle_scn_in, (float)toggle_scn_out, (float)toggle_scn, (float)toggle_scn_max);
#if PEAK
		float Max = 0;
		for (ia = 0; ia < chainnum; ia++)
		{
			if (ShiftPeak[ia] / (schain[ia].length - 1) > Max)
				Max = ShiftPeak[ia] / (schain[ia].length - 1);
		}
		fprintf(fout, "%4.3f, %4.3f", (float)toggle_scn / (float)toggle_scn_max * 100.0, Max * 100);
#endif
		fprintf(fout, "\n");
#if WSA_REC
		printf("Capture Power \n#Cap=%d\n", cap_freq);
		for (ia = 1; ia <= cap_freq; ia++)
			fprintf(fout, ",%.0f", WSA[ia] / clocktime); // exit(1);
		fprintf(fout, ",%.0f", MaxWSA);
#endif
#endif
		fprintf(fout, "*************OUTPUT END****************\n");
		fclose(fout);
		printf("*************OUTPUT END****************\n");
#if SELECT_STATION
#else
#if FAULTOB
		//四つの故障リストを出力する、故障リストは2進数の形で32個ごとに格納している
		//例：故障番号＝10であれば、2進数0000　0000　0100　0000　0000　0000　0000　0000で表し、10進数（4194304）で格納する　
		printf("Output Fault Lists.........\n");

		char *OPName1 = "Total_Results", *OPName2 = "Last_Cap_Results", *OPName3 = "OB_Cap_Results", *OPName4 = "Eve_Cap_Results";
		char out1[50] = {'\0'}, out2[50] = {'\0'}, out3[50] = {'\0'}, out4[50] = {'\0'}, tmp[10] = {'\0'};
		char *atpg = "ATPG", *lfsr = "LFSR", mode[4] = {'\0'};
		printf("%s\n", mode);
		strcpy(tmp, argv[1]);

		if (TPG_MODE == 1)
			strcpy(mode, atpg);
		else
			strcpy(mode, lfsr);

		sprintf(out1, "./FLT_LIST/%s_%s_%s", tmp, OPName1, mode);
		sprintf(out2, "./FLT_LIST/%s_%s_%s", tmp, OPName2, mode);
		sprintf(out3, "./FLT_LIST/%s_%s_%s", tmp, OPName3, mode);
		sprintf(out4, "./FLT_LIST/%s_%s_%s", tmp, OPName4, mode);

		fout = fopen(out1, "w");
		fout1 = fopen(out2, "w");
		fout2 = fopen(out3, "w");
		fout3 = fopen(out4, "w");

		remain_flt = count_flt(fltlst.next);
		printf("\nFaultCoverage = %4.3f \n", (1 - (float)remain_flt / (float)sum_flt) * 100.0);
		fprintf(fout, "%d,%d,%d \n", length, ffnum, sum_flt);
		fprintf(fout1, "%d,%d,%d \n", length, ffnum, sum_flt);
		fprintf(fout2, "%d,%d,%d \n", length, ffnum, sum_flt);
		fprintf(fout3, "%d,%d,%d,%d\n", length, ffnum, cap_freq, sum_flt);
		for (ia = 0; ia <= length; ia++)
		{
			if (ia % PRN_FLT_INT == 0)
				//printf("I Am alive,%d/%d was finished, please wait!!\n",ia,length);
				fprintf(fout, "patten %d,  ", ia);
			if (ia != 0)
			{
				fprintf(fout1, "patten %d,  ", ia);
				fprintf(fout2, "patten %d,  ", ia);
			}
			fprintf(fout3, "patten %d,  ", ia);

			for (ib = 0; ib < ffnum; ib++)
			{
				fprintf(fout, "\nFF %1d %1d", ib, Pat_FF_Faults[ia][ib].Total_Faults[0]);
				if (ia != 0)
				{
					fprintf(fout1, "\nFF %1d %1d", ib, Pat_FF_Faults[ia][ib].Last_Cap_Faults[0]);
					fprintf(fout2, "\nFF %1d %1d", ib, Pat_FF_Faults[ia][ib].OB_Cap_Faults[0]);
				}
				fprintf(fout3, "\nFF %1d", ib);

				for (id = 1; id <= sum_flt / 32 + 1; id++)
				{
					fprintf(fout, " %1x ", Pat_FF_Faults[ia][ib].Total_Faults[id]);
					if (ia != 0)
					{
						fprintf(fout1, " %1x ", Pat_FF_Faults[ia][ib].Last_Cap_Faults[id]);
						fprintf(fout2, " %1x ", Pat_FF_Faults[ia][ib].OB_Cap_Faults[id]);
					}
				}
			}
			fprintf(fout3, "\n");
			fprintf(fout, "\n");
			if (ia != 0)
			{
				fprintf(fout1, "\n");
				fprintf(fout2, "\n");
			}
		}

		fclose(fout);
		fclose(fout1);
		fclose(fout2);
		fclose(fout3);

		for (ia = 0; ia < length + 1; ia++)
			free(Pat_FF_Faults[ia]);
		free(Pat_FF_Faults);
#endif
#endif

		break;

	case 4:
		remain_flt = count_flt(fltlst.next);

		switch (TGL_GATE_MODE)
		{
		case 0: //Non Toggle gate insertion
			sprintf(outpath, "./OUTPUTS/CPI/%dcycles/Non_%s.txt", cap_freq, argv[1]);
			break;
		case 1: //toggle gate insert
		case 4:

			sprintf(outpath, "./OUTPUTS/CPI/%d_cycles/%s_LogicCPI.txt", cap_freq, argv[1]);
			break;
		case 2: //toggle FF insert
		case 3:
			sprintf(outpath, "./OUTPUTS/CPI/%d_cycles/%s_FFCPI.txt", cap_freq,argv[1]);
			break;

		default:
			break;
		}

		/*	if(TGL_GATE_MODE==1) {
			switch (TG_FILE){
				case 0: sprintf(outpath, "./OUTPUTS/TGL_GATE/%dcycles/%dSKIP/%s_Prob_IND_%d_%d.txt", cap_freq,SKIP_CYCLE,argv[1],INTERVAL_CYCLE,SKIP_CYCLE);break;
				case 1: sprintf(outpath, "./OUTPUTS/TGL_GATE/%dcycles/%s_Prob_Over_%d.txt",cap_freq, argv[1],INTERVAL_CYCLE);break;
				case 2: sprintf(outpath, "./OUTPUTS/TGL_GATE/%dcycles/%s_Struc_IND_%d.txt",cap_freq, argv[1],INTERVAL_CYCLE);break;
				case 3: sprintf(outpath, "./OUTPUTS/TGL_GATE/%dcycles/%s_Struc_Over_%d.txt",cap_freq, argv[1],INTERVAL_CYCLE);break;
				default: break;
			}

		}
		else sprintf(outpath, "./OUTPUTS/TGL_GATE/%s_non.txt", argv[1]);*/

		if ((fout = fopen(outpath, "w")) == NULL)
			printf("#TGL_GATE output file is not exist!\n"), exit(1);
		printf("\n\n*************OUTPUT RESULTS****************\n");
		fprintf(fout, "*************OUTPUT RESULTS****************\n");
		if (TGL_GATE_MODE == 1)
		{
			printf("#gate, #tgl_gt\n%d, %d \n", numgate, tgl_gt_cnt);
			fprintf(fout, "#gate, #tgl_gt\n%d, %d \n", numgate, tgl_gt_cnt);
		}

		printf("#Test Pat.,	#Faults,	#Det. Flts,	#UnDet.Flts,	Fcov.\n");
		fprintf(fout, "#Test Pat.,	#Faults,	#Det. Flts,	#UnDet.Flts,	Fcov.\n");
		printf("%d, %d, %d, %d, %4.6f\n", length, sum_flt, sum_flt - remain_flt, remain_flt, (1 - (float)remain_flt / (float)sum_flt) * 100.0);
		fprintf(fout, "%d,%d,%d,%d,%4.6f\n", length, sum_flt, sum_flt - remain_flt, remain_flt, (1 - (float)remain_flt / (float)sum_flt) * 100.0);
#if POWEREVA
		printf("Test Power Evaluation \n#SHIFT_IN,  SHIFT_OUT, SHIFT\n");
		fprintf(fout, "Test Power Evaluation \n#SHIFT_IN,  SHIFT_OUT, SHIFT");
#if PEAK
		fprintf(fout, "SHIFT_TGL_RATE, PEAK TGL");
#endif
		fprintf(fout, "\n");
		fprintf(fout, "%.0lf,%.0lf,%.0lf,%.0lf", (float)toggle_scn_in, (float)toggle_scn_out, (float)toggle_scn, (float)toggle_scn_max);
#if PEAK
		float Max = 0;
		for (ia = 0; ia < chainnum; ia++)
		{
			if (ShiftPeak[ia] / (schain[ia].length - 1) > Max)
				Max = ShiftPeak[ia] / (schain[ia].length - 1);
		}
		fprintf(fout, "%4.3f, %4.3f", (float)toggle_scn / (float)toggle_scn_max * 100.0, Max * 100);
#endif
		fprintf(fout, "\n");

#if WSA_REC
		printf("Capture Power \n#Cap=%d\n", cap_freq);
		fprintf(fout, "Capture Power \n#Cap=%d\n", cap_freq);
		for (ia = 1; ia <= cap_freq; ia++)
		{
			fprintf(fout, ",%.0f", WSA[ia] / clocktime); // exit(1);
			printf(",%.0f", WSA[ia] / clocktime);		 // exit(1);
		}
		fprintf(fout, ",%.0f", MaxWSA);
		printf(",%.0f", MaxWSA);
#endif
#endif
		fprintf(fout, "\n*************OUTPUT END****************\n");
		fclose(fout);
		printf("\n*************OUTPUT END****************\n");
		for (ia = 0; ia <= sum_flt + 1; ia++)
			free(flt_det_flog[ia]);
		free(flt_det_flog);
		break;

	default:
		printf("No New Function is Supported by this Program!\n"), exit(1);
	}
}
