#include "declare.h"
typedef struct l_node L_NODE;
typedef struct fin_node FIN_NODE;
typedef struct flt_node FLT_NODE;
typedef struct dinff DINFF;
typedef struct t_node T_NODE;
typedef struct scan_chain SCAN_CHAIN;
typedef struct FF_Pat_Flt_List FF_PAT_FLT_LIST;

typedef int CHGATA;           /* int when debugging, char otherwise */
typedef unsigned int BITGATA; /* sort of memorry used unit of bit */

//2進数でパターンごとにキャプチャ毎に故障を記録するため構造体_2015710_王
//typedef struct lfsr LFSR;
struct l_node
{
  int line, type, innum, outnum, OBTimes, addgateflog;
  //toggle gate insertion
  int toggle_flog;
  int toggle_br_flog[100];
  int toggle_br_flog_num;
  int toggle_cnt;
  int toggle_cap[MAXCAP];
  float toggle_rate[MAXCAP];
  unsigned int gdval1_cap[MAXCAP];
  unsigned int ftval1_cap[MAXCAP];
  unsigned int gdval1_br_cap[MAXCAP];
  unsigned int ftval1_br_cap[MAXCAP];
  //
  L_NODE *next, *prev;
  FIN_NODE *finlst, *foutlst;
  unsigned int gdval0, gdval1, ftval0, ftval1;
  char *Name;
  int sel_flog[20]; //5種類のFF選択法
};

struct fin_node
{
  L_NODE *node;
  FIN_NODE *next;
};

struct flt_node
{
  int saval, typeflog, num, line;
  L_NODE *back, *forwd;
  FLT_NODE *next, *prev;
  DINFF *dfflst;
  unsigned int detect[MAXLEN / 32 + 1], activ[MAXLEN / 32 + 1];
  int first_det_pat_scan, first_det_pat_last, first_det_pat_multi, first_det_pat_total;
  int first_det_pat_obsel[20];
  int OBdtime_sel_FF[10];
  int full_ob_dtime;
  //unsigned int PreAddPiValue[30];//PI value of inseted fault site for Transition fault sim
  int dtime, scan_dtime, OBdtime, OBdtime_cap[MAXCAP], last_cap_dtime, TranDetTimes, TranOBDetTimes;
  int scan_dtime_tmp, OBdetime_tmp;
};
struct dinff
{
  L_NODE *node;
  unsigned int val;
  DINFF *next;
};

struct t_node
{
  unsigned int gdval1, ftval1;
  int DetFaultsNum;
  //int DetFaults[20000];
};

struct scan_chain
{
  int length;
  int top;
  int lastval;
};

struct FF_Pat_Flt_List
{ //パターン毎、FF毎に検出した故障情報を保存する構造体
  //int Total_Det_Faults;
  //int OB_Det_Faults;
  //int Last_Det_Faults;
  //unsigned int Cap_Faults[MAXCAP][MAXFLT/32+1];//パターン毎、FF毎、キャプチャ毎に検出故障
  // unsigned int Last_Cap_Faults[MAXFLT/32+1];//パターン毎、FF毎に最終のキャプチャで検出した故障
  // unsigned int OB_Cap_Faults[MAXFLT/32+1];//パターン毎、FF毎に中間キャプチャで検出した故障
  //unsigned int Total_Faults[MAXFLT/32+1];//パターン毎、FF毎に検出した故障の統計
  //unsigned int **Cap_Faults;//パターン毎、FF毎、キャプチャ毎に検出故障
  unsigned int *Last_Cap_Faults; //パターン毎、FF毎に最終のキャプチャで検出した故障
  unsigned int *OB_Cap_Faults;   //パターン毎、FF毎に中間キャプチャで検出した故障
  unsigned int *Total_Faults;    //パターン毎、FF毎に検出した故障の統計
};

L_NODE gnode, inode;
FIN_NODE pinode, ponode, ffnode;
FLT_NODE fltlst, dropflt;
L_NODE add_gate[32], add_pi[32];
FIN_NODE add_fin[32][32], add_fout[32], pi_fout[32];
T_NODE **ff_observe;
T_NODE **po_observe;
SCAN_CHAIN *schain;
FF_PAT_FLT_LIST **Pat_FF_Faults;
/***
L_NODE line_tmp[MAXLINE];
FLT_NODE fltlst_tmp[MAXFLT];
FIN_NODE fin_tmp[MAXLINE*2];
int fill_fin_tmp,fill_line_tmp;
***/

/* net list */
typedef struct element
{
  int type, nfi, nfo, line;
  int fil;
  int fol;
  /* char name[30]; */
} ELEMENT;

/*Instance Variable Defination*/
int MODE_TOOL, MODE_TEST, TAP_NUM, cap_freq, SKIP_CAPTURE, SKIP_CYCLE, FF_SEL_METHOD, TGL_GATE_MODE, clocktime, LBISTMODE, ALPMODE, INTERVAL_CYCLE, TG_FILE, SOControlFlog, SoControlMode, FFSelMode, CHAINLENGTH, length,group_tpi;
int numout, slist, numgate, n_tpi;
int lpnt, inpnum, ffnum, sum_flt, remain_flt, sum_Tran_flt, chainnum, interstatecount[2], tgl_gt_cnt;
int **tgl_tpi;
int flt_cap[MAXCAP];

float Tgl_rate;
float ff_rate;
long double toggle_scn_max, toggle_cap_max, toggle_scn, toggle_scn_in, toggle_scn_out, toggle_cap[MAXCAP];
double toggle_shift_perpat, toggle_cap_perpat[MAXCAP];
double WSA[MAXCAP], MaxWSA;
char *ff_select;
float ShiftPeak[CHAINNUM], OBSERVE_RATE;
int flt_det_num[20];
int **flt_det_flog;
//int flt_det_flog[MAXFLT][11];
int FF_FILE;
int toggle_gates[MAXGATE];
FILE *fout_in;
FILE *fout_flt_in;
