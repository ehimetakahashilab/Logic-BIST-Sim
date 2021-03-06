#!/bin/sh
#######Path Defination###############
User_DIR=$(cd "$(dirname "$0")";pwd)

OP_LIST_DIR=${User_DIR}/FF_STATION
CP_LIST_DIR=${User_DIR}/CP_STATION

if [ ! -e ${OP_LIST_DIR} ]; then
  mkdir -p ${OP_LIST_DIR}
fi

if [ ! -e ${CP_LIST_DIR} ]; then
  mkdir -p ${CP_LIST_DIR}
fi


#ATPGのテストモードを追加した_王#
#Gloable Parameter Setting:GP
#GPリスト：TOOLMODE, TPG, TEST_VEC, CIRCUIT

##TOOLMODE:シミュレーションの動作目的を指定する
#=1: Normal Scan Test 通常のスキャンテストモード（シングルキャプチャ）
#=2: Multi-cycle Test, 通常のマルチサイクルテストモード、キャプチャ数指定が必要
#CAPTURE: マルチサイクルテスト動作モード時のキャプチャ数、最大５０まで指定可
#=3: Multi-cycle Test with SeqOB,マルチサイクルテスト中間観測する動作モード
#キャプチャ数、ＦＦ観測率（全体ＦＦ数の比率）、観測用ＦＦランキングリストパスの指定が必要
#CAPTURE: マルチサイクルテスト動作モード時のキャプチャ数、最大５０まで指定可
#OBRATE: ＦＦ観測比率（0~1の小数）
#SKIPCYCLES: 観測しないサイクル番号、CAPTURE=10, SKIPCYCLES=3の場合、１０サイクルテストにおいて、第３サイクルからＦＦ値を観測する
#ff_sta_file: 観測用ＦＦランキングリストファイルパス指定
#=4: マルチサイクルテストwithＣＰＩ、ＦＦ－ＣＰＩやLogic-CPIはパラメータで指定する

##TPG: テストパターン生成モード、=0: LFSR, =1: ATPG
##TEST_VEC: テストパターン数を指定する。

TPG=0 #=0:LFSR,=1:ATPG
TEST_VEC=10 #0 00 # Number of Test patterns
TOOLMODE=3 #=1:Normal Scan test, =2:Multi-cycle Test, =3: Multi-cycle test with Seq OB, =4:Toggle Gate TPI

OBRATE=0.2 #Observation Rate
SKIPCYCLES=0
SELECTSTATION=3 #0: SCOAP, 1: cop+type, 2:OP 3: multi observation

ulimit -s unlimited
rm -f *.dat
for CIRCUIT in s9234 #s13207 s15850 s38417 s38584 #b14s.osaka b15s.osaka b20s.osaka  #s9234 s13207 s15850 s38417 s38584  b14s.osaka b15s.osaka b20s.osaka #b20s.osaka #s15850 s35932 s38584 s38417 #s13207 #s1488 s5378 s9234 #s13207 #s9234 #s15850 s38417 #s38584 #b04s.osaka b05s.osaka b06.osaka b14s.osaka b15s.osaka b20s.osaka

do
rm -f lfsr*.dat ATPG.dat
rm -f ${CIRCUIT}_tgl_FF_input.dat
rm -f ${CIRCUIT}_tgl_ff_tpi.dat

  ln -s ./circuit/$CIRCUIT
  ln -s ./tpg/lfsr.dat

	if [ "$TPG" = "0" ];
	then
		rm -f ${CIRCUIT}_lfsr_pi.dat
		./tpg/lfsr $CIRCUIT $TEST_VEC ${CIRCUIT}_lfsr_pi.dat
		ln -s tmp.test ${CIRCUIT}_lfsr_pi.dat
	else
 		 ln -s ./ATPG/SA/Com1Test/"$CIRCUIT".tests ATPG.dat
	fi

case $TOOLMODE in
 "1")
	time ./proofsim $CIRCUIT $TOOLMODE $TPG  #>> Switch_ctr.txt
	;;
"2")
	for CAPTURE in 5 #2 3 4 5 #10 #15 #1 #20 #1 5 10 15 #20 #15 #20 #2 #20 #2 3 4 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
		do
	 time ./proofsim $CIRCUIT $TOOLMODE $TPG $CAPTURE $SKIPCYCLES #>> Switch_ctr.txt
		done
	;;
"3")

if [ ! -e ${OP_LIST_DIR}/${CIRCUIT} ]; then
  mkdir -p ${OP_LIST_DIR}/${CIRCUIT}
fi

	cnt1=0
	for ff_sta_file in  ${FF_LIST_DIR}/${CIRCUIT}/TOPSIS
	do
		if [ -e $ff_sta_file ];
		then
 		 echo === OB_STATION INFO of $ff_sta_file is FOUND, CONTINUE THE FAULT SIM PROCESS===
		else
 		 echo === NO OB_STATION INFO, PLEASE BACK TO FF_SELECTION PROCESS to Generate $ff_sta_file===
			exit
		fi

		ln -s $ff_sta_file ff_station_$cnt1.dat
		let cnt1=${cnt1}+1
	done

	for CAPTURE in 5 # 2 3 4 5  #15 #1 #20 #1 5 10 15 #20 #15 #20 #2 #20 #2 3 4 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
		do
	 time ./proofsim $CIRCUIT $TOOLMODE $TPG $CAPTURE $SKIPCYCLES $OBRATE $cnt1 #>> Switch_ctr.txt
	done
	rm -rf ff_station* lfsr*.dat ATPG.dat tmp.test *lfsr_pi.dat
	;;

"4")
rm -f ${CIRCUIT}_tgl_FF_input.dat
rm -f ${CIRCUIT}_tgl_ff_tpi.dat

	CAPTURE=5 #the number of capture cycles
	LCP_rate=0.01 #the ratio of Logic CPs in all logic gates
	FFCP_rate=0.1	#the ration of FF-CPs in all FFs
  OPI=0 #=1: Observation Point insertion aviable

  if [ "$OPI" = "1" ];
  then
	 cnt1=0
	  for ff_sta_file in  ./FF_STATION/$CIRCUIT/TOPSIS #./FF_STATION/$CIRCUIT/BRANCH ./FF_STATION/$CIRCUIT/COMPLEX ./FF_STATION/$CIRCUIT/TYPE_1 ./FF_STATION/$CIRCUIT/TYPE_2 ./FF_STATION/$CIRCUIT/TYPE_3 ./FF_STATION/$CIRCUIT/TOPSIS
# you can evaluate multiple FF OP list in one simulation by specifying the path of OB-FF List
	   do

		if [ -e $ff_sta_file ];
		then
 		 echo === OB_STATION INFO of $ff_sta_file is FOUND, CONTINUE THE FAULT SIM PROCESS===
		else
 		 echo === NO OB_STATION INFO, PLEASE BACK TO FF_SELECTION PROCESS to Generate $ff_sta_file===
			exit
		fi

		ln -s ${ff_sta_file} ff_station_${CIRCUIT}_${cnt1}.dat
		let cnt1=${cnt1}+1
	 done
  fi

for CP_CTRL in 1 #3 #0 2 3 #2 3 #4
#=0:non CP control, =1: Logic gate toggling,=2:FF toggling, =3: applying random pattern to FF-CPs, =4: applying random patterns to Logic CPs

do
## when CP runs in random loading mode, the random patterns will be pre-generated by the following Setting
	if [ "$CP_CTRL" = "3" ];
		then
 		 echo === Generate the LFSR Vectors 4 FF_CPI===
			./tpg_TPI_ff/lfsr_ff $CIRCUIT $TEST_VEC $FFCP_rate $CAPTURE ${CIRCUIT}_tgl_ff_tpi.dat
		ln -sf tmp_ff.dat ${CIRCUIT}_tgl_ff_tpi.dat

	elif [ "$CP_CTRL" = "4" ];
		then
 		 echo ===  Generate the LFSR Vectors 4 Logic_CPI===
			./tpg_TPI_gate/lfsr_gt $CIRCUIT $TEST_VEC $LCP_rate $CAPTURE tgl_gt_tpi.dat
			ln -sf tmp_gt.dat tgl_gt_tpi.dat
 		 echo ===  end ===
	fi

##following, you need to specify the CP list
for FF_SEL_METHOD in 7 #12 15 16 17 #7 8 10 11 #12 13 #5 7 #2 3 4 5 6
do
#SKIP_CAP=3, the CP control starts from the third capture cycle.
for SKIP_CAP in 3 #4 5
	do
	#if [ "$CP_CTRL" = "0" ] || [ "$CP_CTRL" = "1" ] || [ "$CP_CTRL" = "4" ];
	if [ "$CP_CTRL" = "1" ] || [ "$CP_CTRL" = "4" ];
	then
		for CP_LIST in 21 # 9 #2 #3 #0 1 2 3
		do
		for GP in 1 10 20 30 40 50
		do
		case "$CP_LIST" in
	 		"0") ln -s ./GATE_TOGGLE/$CIRCUIT/TGL_Prob_FCOV_IND_$SKIP_CAP tgl_gt_input.dat;;
	 		"1") ln -s ./GATE_TOGGLE/$CIRCUIT/TGL_Prob_FCOV_Overlap tgl_gt_input.dat;;
	 		"2") ln -s ./GATE_TOGGLE/$CIRCUIT/TGL_STRUCT_FCOV_IND tgl_gt_input.dat;;
	 		"3") ln -s ./GATE_TOGGLE/$CIRCUIT/TGL_STRUCT_FCOV_Overlap tgl_gt_input.dat;;
			"4") ln -s ./GATE_TOGGLE/to_Wang/$CIRCUIT/TOPSIS_GT_PRO tgl_gt_input.dat;;
			"5") ln -s ./GATE_TOGGLE/to_Wang/$CIRCUIT/TOPSIS_GT_STRUC tgl_gt_input.dat;;
			"6") ln -s ./GATE_TOGGLE/to_Wang/$CIRCUIT/TOPSIS_GT_STRUC+PRO tgl_gt_input.dat;;
			"7") ln -s ./GATE_TOGGLE/to_Wang/$CIRCUIT/TOPSIS_GT_CONF tgl_gt_input.dat;;
      "8") ln -s ./GATE_TOGGLE/to_Wang/$CIRCUIT/hybrid_max tgl_gt_input.dat;;
			"9") ln -s ./GATE_TOGGLE/to_Wang/$CIRCUIT/hybrid_sum tgl_gt_input.dat;;
			"10") ln -s ./GATE_TOGGLE/to_Wang/$CIRCUIT/renesas_tpi_cop tgl_gt_input.dat;;
			"11") ln -s ./GATE_TOGGLE/to_Wang/$CIRCUIT/renesas_tpi_cop+type tgl_gt_input.dat;;
			"12") ln -s ./GATE_TOGGLE/to_Wang/$CIRCUIT/renesas_tpi_cop+type_init_ave tgl_gt_input.dat;;
			"13") ln -s ./GATE_TOGGLE/to_Wang/$CIRCUIT/hybrid_max_init_ave tgl_gt_input.dat;;
			"14") ln -s ./GATE_TOGGLE/to_Wang/$CIRCUIT/renesas_tpi_copO_ave+type_init_ave tgl_gt_input.dat;;
			"15") ln -s ./GATE_TOGGLE/to_Wang/$CIRCUIT/hybrid_tpi tgl_gt_input.dat;;
			"16") ln -s ./GATE_TOGGLE/to_Wang/$CIRCUIT/hybrid_tpi_init_ave tgl_gt_input.dat;;
			"17") ln -s ./GATE_TOGGLE/to_Wang/$CIRCUIT/hybrid_tpi_init_ave_PRPF tgl_gt_input.dat;;
			"18") ln -s ./GATE_TOGGLE/to_Wang/$CIRCUIT/hybrid_tpi_init_ave_PRPF_PO tgl_gt_input.dat;;
      "19") ln -s ./GATE_TOGGLE/to_Wang/$CIRCUIT/renesas_tpi_fixed_gate+type_init_ave tgl_gt_input.dat;;
			"20") ln -s ./GATE_TOGGLE/to_Wang/$CIRCUIT/renesas_tpi_contribure_tgl_init_ave tgl_gt_input.dat;;
			"21") ln -s /mnt/tiger/home/wang/renesas/Renesas-TPI/Renesas-TPI-FF-init-master/Renesas-TPI-FF-init-master/OUTPUTS/$CIRCUIT/rtpi_gp_$GP tgl_gt_input.dat;;
		esac

		for CYCLE in 1 #2 3 4
			do
			echo ===$CIRCUIT: $CP_LIST $CYCLE===========
			time ./proofsim $CIRCUIT $TOOLMODE $TPG $CP_CTRL $LCP_rate $CAPTURE $CP_LIST $CYCLE $SKIP_CAP $cnt1 $OBRATE $GP $TEST_VEC #>> Switch_ctr.txt
    			echo ===time end=== #>> $LOG_FILE
		done
		rm -f tgl_gt_input.dat
		done
	done
	else
    			echo ==$FF_SEL_METHOD $SKIP_CAP=== #>> $LOG_FILE

		#ln -s /mnt/tiger/home/wang/renesas/FF-CPI_IEICE/GATE_TOGGLE/$CIRCUIT/TGL_FF_STATION/Method_${FF_SEL_METHOD}_${SKIP_CAP}.csv ${CIRCUIT}_tgl_FF_input.dat
		#ln -s /mnt/tiger/home/wang/renesas/FF-CPI_IEICE/GATE_TOGGLE/$CIRCUIT/TGL_FF_STATION/Method_${FF_SEL_METHOD}.csv ${CIRCUIT}_tgl_FF_input.dat
		ln -s ./GATE_TOGGLE/to_Wang/$CIRCUIT/Method_${FF_SEL_METHOD} ${CIRCUIT}_tgl_FF_input.dat

		for CYCLE in 1 #2 3 4
			do
			echo ===$CIRCUIT: $CYCLE $SKIP_CAP===========

			time ./proofsim $CIRCUIT $TOOLMODE $TPG $CP_CTRL $FFCP_rate $CAPTURE $CYCLE $SKIP_CAP $FF_SEL_METHOD $cnt1 $OBRATE $TEST_VEC #>> Switch_ctr.txt

    			echo ===time end=== #>> $LOG_FILE
		done
		#rm -f tgl_*.dat ff_station_*
	fi
done
rm -f ${CIRCUIT}_tgl_FF_input.dat
done
rm -f ${CIRCUIT}_tgl_ff_tpi.dat
done
rm -f ff_station_*.dat
;;
esac

    if [ -e fault_list.dat ];
      then
      mv fault_list.dat "$CIRCUIT"_fault_list.dat
      if [ ! -e ./fault_list ];
        then
        mkdir fault_list
      fi
      mv "$CIRCUIT"_fault_list.dat ./fault_list
    fi
#if exist fault list file -end-
#rm -f #ff_station* tgl_* nfs*
rm -f $CIRCUIT lfsr.dat *~ tmp*  ff_station.dat ATPG.dat *lfsr_pi.dat
done
