#include "declare.h"
#include "def_gtype.h"
#include "def_flt.h"
#include "string.h"
#include "math.h"

main(argc, argv) int argc;
char *argv[13];

{
  struct tms cputime;
  int totime, ia, ib, ic, id;
  int remain_flt, faultnum;
  float Shift_rate;
  time_t time_1, time_2;
  char path_last[256], path_middle[256], name[256], path_toggle[256];
  FILE *fout, *fout1, *fout2, *fout3;

  //  if(argc<13){
  //   printf("error: too less arguments!\n");
  //   exit(1);
  //  }

  /*::::::::::when using ATPG, set the test length :::::::::::::*/
  //length=read_pival_01x(argv,pivalset);
  //#if DEBUG3
  //  printf(" Test Pattern are %d Patterns by ATPG\n",length);
  //#endif
  Instance_Get(argc, argv);
  make_line_list(argv);
  sort_node();
  // fnode = gnode.next;
  // for (; fnode != NULL; fnode = fnode->next)
  // {
  //   printf("%d\n", fnode->line);
  // }
  err_check(fltlst.next, 0);

  faultnum = sum_flt;
 // printf("\nfaultnumber=%d %d\n",sum_flt,length);exit(1);

  flt_out(fltlst.next, argv);

#if FLT_OUTPUT
  printf("\nOutput Fault List\n");
  flt_out(fltlst.next, argv);
  exit(1);
#endif

#if DEBUG3
  prn_fltlst(fltlst.next);
  prn_fltlst_format(fltlst.next);
  prn_node(gnode.next);
  prn_subnode(pinode.next);
  prn_subnode(ffnode.next);
  (ponode.next);
  printf(" === Sum of Faults %d %d (ko) ===\n", sum_flt, sum_Tran_flt);
#endif

  times(&cputime);
  totime = cputime.tms_utime;
  time_1 = time((time_t *)0);

  /*#if MAKE_REC
  fout = fopen("fault coverage.log", "a");
  fprintf(fout, "%s\n", argv[1]);
	fprintf(fout, "Fcov(%) ShiftRate(%) capturerate(%)\n");
  fclose(fout);
#endif*/

  faultsim(argv); //←bug 2014_08_23 ===================

#if FLT_PER_PAT
  fclose(fout_fpp);
#endif
//printf("HEHRE???\n");
  Out_Put(argv);
///printf("HEHRE???\n");
  times(&cputime);
  totime = cputime.tms_utime - totime;
  printf(" ***  cputime = %.3f[sec] *** \n\n", (double)totime / 100);
  time_2 = time((time_t *)0);
  /*   printf(" CPU-time %.2f(sec)\n",difftime(time_2,time_1)); */
}
/*
count_flt(flttag)
FLT_NODE *flttag;
{
  int sumflt=0,sumTDflt=0;
 #if TRANSITIONFAULT
 for( ;flttag!=NULL;flttag=flttag->next){

    if(!flttag->TranDetTimes)
      sumTDflt++;
  }
  return sumTDflt;
#else
 for( ;flttag!=NULL;flttag=flttag->next){

    if(!flttag->dtime)
      sumflt++;
  }
  return sumflt;
#endif
}
*/

count_flt(flttag)
    FLT_NODE *flttag;
{
  int sumflt = 0, sumTDflt = 0, count = 0;
  int ia, ib;
#if TRANSITIONFAULT
  for (; flttag != NULL; flttag = flttag->next)
  {

    if (!flttag->TranDetTimes)
      sumTDflt++;
  }
  return sumTDflt;
#else
#if SELECT_STATION
  for (ia = 0; ia < FF_FILE; ia++)
    flt_det_num[ia] = 0;
  flt_det_num[10] = 0;
#endif

  for (; flttag != NULL; flttag = flttag->next)
  {
    count++;
    if (!flttag->dtime)
      sumflt++;
    /*#if SELECT_STATION
	if(MODE_TOOL==4){
	for(ia=0;ia<FF_FILE;ia++) {
		if(flttag->OBdtime_sel_FF[ia]==1) flt_det_num[ia]++;
		}
	if(flttag->full_ob_dtime==1) flt_det_num[20]++;
	}
	#endif*/
    //printf("%d ",flttag->OBdtime_sel_FF[ia]);
    //printf("\n");

  }
  if ( MODE_TOOL == 3 || MODE_TOOL == 4)
  {
    for (ia = 0; ia <= sum_flt; ia++)
    {
      for (ib = 0; ib < FF_FILE; ib++)
      {
        if (flt_det_flog[ia][ib])
          flt_det_num[ib]++;

      }

      if (flt_det_flog[ia][10])
        flt_det_num[10]++;
    }
  }
  /*	#if SELECT_STATION
	for(ia=0;ia<FF_FILE;ia++)
		flt_det_num[ia]+=sum_flt-sumflt;
	flt_det_num[20]+=sum_flt-sumflt;
#endif*/

  return sumflt;
#endif
}


saf_list_check(flttag)
    FLT_NODE *flttag;
{
  int count = 0;
  for (; flttag != NULL; flttag = flttag->next)
    count++;

  return count;
}

err_check(flttag, time)
    FLT_NODE *flttag;
int time;
{
  while (flttag != NULL)
  {
    if (flttag->dfflst != NULL)
    {
      printf(" error fault %d  time %d\n", flttag->back->line, time);
      exit(1);
    }
    flttag = flttag->next;
  }
}

fault_det_cap(fflist, totalflts, argv) int fflist[ffnum][40000];
int totalflts;
char *argv[10];
{
  int ia, ib, ic, id, flog;
  float FF_TCov[ffnum], FF_Only_Flt[ffnum];
  FILE *FF_flt_list;
  for (ia = 0; ia < ffnum; ia++)
  {
    FF_Only_Flt[ia] = 0.0;
    FF_TCov[ia] = (float)fflist[ia][0] / totalflts * 100;
    for (ib = 1; ib <= fflist[ia][0]; ib++)
    {
      flog = 0;
      for (ic = 0; ic < ffnum; ic++)
      {
        if (ia == ic)
          continue;
        for (id = 1; id <= fflist[ic][0]; id++)
        {
          if (fflist[ia][ib] == fflist[ic][id])
          {
            flog = 1;
            break;
          }
        }
        if (flog == 1)
          break;
      }
    }

    if (flog == 0)
      FF_Only_Flt[ia]++;
  }
  FF_flt_list = fopen("TCov_of_FFs.txt", "a");
  fprintf(FF_flt_list, "%s  %s  %s\n", argv[1], argv[3], argv[4]);
  for (ia = 0; ia < ffnum; ia++)
    fprintf(FF_flt_list, "%4.3f ", FF_TCov[ia]);
  fprintf(FF_flt_list, "\n");
  for (ia = 0; ia < ffnum; ia++)
    fprintf(FF_flt_list, "%d ", FF_Only_Flt[ia]);
  fprintf(FF_flt_list, "\n");

  fclose(FF_flt_list);
}

flt_out(flttag, argv)
    FLT_NODE *flttag;
char *argv[11];
{
  int sumflt = 0, sumTDflt = 0;
  FILE *flstout;
  char outfile[30];
  sprintf(outfile, "./FLT_LIST/%s", argv[1]);

  if (NULL == (flstout = fopen(outfile, "a")))
    printf(" error fault list file does not exit!\n"), exit(0);

  fprintf(flstout, "%d \n", sum_flt);
  int ie = 0;

  for (; flttag != NULL; flttag = flttag->next)
  {
    flt_list[ie][0] = flttag->line;
    flt_list[ie][1] = flttag->saval;
    if (flttag->forwd != NULL)
      fprintf(flstout, "%d\n", flttag->num);
    else
      fprintf(flstout, "%d\n", flttag->num);
    ie++;
  }
  fclose(flstout);
}

/*
err_check(flttag,time)
FLT_NODE *flttag;
int time;
{
  while(flttag!=NULL){
    if(flttag->dfflst!=NULL){
      printf(" error fault %d  time %d\n",flttag->back->line,time);
      exit(1);
    }
    flttag=flttag->next;
  }
}

err_occur(buf)
char buf[];
{
  if(strcmp(buf, "mem") == 0)
    fprintf(stdout, "memory allocate error!\n");
  else
    fprintf(stdout, "'%s' is not found!\n", buf);

  exit(1);
}
*/
