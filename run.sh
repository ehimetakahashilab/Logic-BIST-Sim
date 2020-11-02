#!/usr/bin/bash

set -eu
function catch {
    echo "Error: FFOPS is failed!" >&2
}
trap catch ERR

## file, directory path setting ##
APP_DIR=$(dirname $0)
CIRCUIT_PATH=$1
if [ ! -e ${CIRCUIT_PATH} ]; then 
    echo "cannot read Circuit File" >&2
    exit 1
fi
CIRCUIT_NAME=$(basename ${CIRCUIT_PATH})

User_DIR=$(cd "$(dirname "$0")";pwd)
FF_LIST_DIR=${User_DIR}/FF_STATION

## Paratemter for Simulation ##
TPG=0
TEST_VEC=10
TOOLMODE=3
OBRATE=0.2
SKIPCYCLES=0
SELECTSTATION=3

./tpg/lfsr ${CIRCUIT_PATH} ${TEST_VEC} ${CIRCUIT_NAME}_lfsr_pi.dat

# case $TOOLMODE in
#     "1")
# 	time ./sim ${CIRCUIT_NAME} $TOOLMODE $TPG
# 	;;
#     "2")
# 	for CAPTURE in 5
# 	do
# 	    time ./sim ${CIRCUIT_NAME} $TOOLMODE $TPG $CAPTURE $SKIPCYCLES
# 	done
# 	;;
#     "3")
# 	cnt1=0
# 	for ff_sta_file in  ${FF_LIST_DIR}/${CIRCUIT_NAME}/TOPSIS
# 	do
# 	    if [ -e $ff_sta_file ];
# 	    then
#  		echo === OB_STATION INFO of $ff_sta_file is FOUND, CONTINUE THE FAULT SIM PROCESS===
# 	    else
#  		echo === NO OB_STATION INFO, PLEASE BACK TO FF_SELECTION PROCESS to Generate $ff_sta_file===
# 		exit 1
# 	    fi

# 	    ln -s $ff_sta_file ff_station_$cnt1.dat
# 	    let cnt1=${cnt1}+1
# 	done

# 	for CAPTURE in 5
# 	do
# 	    time ./sim ${CIRCUIT_NAME} $TOOLMODE $TPG $CAPTURE $SKIPCYCLES $OBRATE $cnt1
# 	done
# 	rm -rf ff_station* lfsr*.dat ATPG.dat tmp.test *lfsr_pi.dat
# 	;;
#     "4")
# 	rm -f ${CIRCUIT_NAME}_tgl_FF_input.dat
# 	rm -f ${CIRCUIT_NAME}_tgl_ff_tpi.dat

# 	CAPTURE=5
# 	TGL_RATE=0.01
# 	FF_RATE=0.1

# 	cnt1=0
# 	for ff_sta_file in  ./FF_STATION/${CIRCUIT_NAME}/TOPSIS
# 	do
# 	    if [ -e $ff_sta_file ];
# 	    then
#  		echo === OB_STATION INFO of $ff_sta_file is FOUND, CONTINUE THE FAULT SIM PROCESS===
# 	    else
#  		echo === NO OB_STATION INFO, PLEASE BACK TO FF_SELECTION PROCESS to Generate $ff_sta_file===
# 		exit 1
# 	    fi

# 	    ln -s ${ff_sta_file} ff_station_${CIRCUIT_NAME}_${cnt1}.dat
# 	    let cnt1=${cnt1}+1
# 	done

# 	for TGL_MODE in 1
# 	do

# 	    if [ "$TGL_MODE" = "3" ];
# 	    then
#  		echo === Generate the LFSR Vectors 4 FF_TPI===
# 		./tpg_TPI_ff/lfsr_ff ${CIRCUIT_NAME} $TEST_VEC $FF_RATE $CAPTURE ${CIRCUIT_NAME}_tgl_ff_tpi.dat
# 		ln -sf tmp_ff.dat ${CIRCUIT_NAME}_tgl_ff_tpi.dat

# 	    elif [ "$TGL_MODE" = "4" ];
# 	    then
#  		echo ===  Generate the LFSR Vectors 4 GATE_TPI===
# 		./tpg_TPI_gate/lfsr_gt ${CIRCUIT_NAME} $TEST_VEC $TGL_RATE $CAPTURE tgl_gt_tpi.dat
# 		ln -sf tmp_gt.dat tgl_gt_tpi.dat
#  		echo ===  end ===
# 	    fi

# 	    for FF_SEL_METHOD in 7
# 	    do
# 		for SKIP_CAP in 3
# 		do
# 		    if [ "$TGL_MODE" = "1" ] || [ "$TGL_MODE" = "4" ];
# 		    then
# 			for METHOD in 21 # 9 #2 #3 #0 1 2 3
# 			do
# 			    for GP in 1 10 20 30 40 50
# 			    do
# 				case "$METHOD" in
# 	 			    "0") ln -s ./GATE_TOGGLE/${CIRCUIT_NAME}/TGL_Prob_FCOV_IND_$SKIP_CAP tgl_gt_input.dat;;
# 	 			    "1") ln -s ./GATE_TOGGLE/${CIRCUIT_NAME}/TGL_Prob_FCOV_Overlap tgl_gt_input.dat;;
# 	 			    "2") ln -s ./GATE_TOGGLE/${CIRCUIT_NAME}/TGL_STRUCT_FCOV_IND tgl_gt_input.dat;;
# 	 			    "3") ln -s ./GATE_TOGGLE/${CIRCUIT_NAME}/TGL_STRUCT_FCOV_Overlap tgl_gt_input.dat;;
# 				    "4") ln -s ./GATE_TOGGLE/to_Wang/${CIRCUIT_NAME}/TOPSIS_GT_PRO tgl_gt_input.dat;;
# 				    "5") ln -s ./GATE_TOGGLE/to_Wang/${CIRCUIT_NAME}/TOPSIS_GT_STRUC tgl_gt_input.dat;;
# 				    "6") ln -s ./GATE_TOGGLE/to_Wang/${CIRCUIT_NAME}/TOPSIS_GT_STRUC+PRO tgl_gt_input.dat;;
# 				    "7") ln -s ./GATE_TOGGLE/to_Wang/${CIRCUIT_NAME}/TOPSIS_GT_CONF tgl_gt_input.dat;;
#         			    "8") ln -s ./GATE_TOGGLE/to_Wang/${CIRCUIT_NAME}/hybrid_max tgl_gt_input.dat;;
# 				    "9") ln -s ./GATE_TOGGLE/to_Wang/${CIRCUIT_NAME}/hybrid_sum tgl_gt_input.dat;;
# 				    "10") ln -s ./GATE_TOGGLE/to_Wang/${CIRCUIT_NAME}/renesas_tpi_cop tgl_gt_input.dat;;
# 				    "11") ln -s ./GATE_TOGGLE/to_Wang/${CIRCUIT_NAME}/renesas_tpi_cop+type tgl_gt_input.dat;;
# 				    "12") ln -s ./GATE_TOGGLE/to_Wang/${CIRCUIT_NAME}/renesas_tpi_cop+type_init_ave tgl_gt_input.dat;;
# 				    "13") ln -s ./GATE_TOGGLE/to_Wang/${CIRCUIT_NAME}/hybrid_max_init_ave tgl_gt_input.dat;;
# 				    "14") ln -s ./GATE_TOGGLE/to_Wang/${CIRCUIT_NAME}/renesas_tpi_copO_ave+type_init_ave tgl_gt_input.dat;;
# 				    "15") ln -s ./GATE_TOGGLE/to_Wang/${CIRCUIT_NAME}/hybrid_tpi tgl_gt_input.dat;;
# 				    "16") ln -s ./GATE_TOGGLE/to_Wang/${CIRCUIT_NAME}/hybrid_tpi_init_ave tgl_gt_input.dat;;
# 				    "17") ln -s ./GATE_TOGGLE/to_Wang/${CIRCUIT_NAME}/hybrid_tpi_init_ave_PRPF tgl_gt_input.dat;;
# 				    "18") ln -s ./GATE_TOGGLE/to_Wang/${CIRCUIT_NAME}/hybrid_tpi_init_ave_PRPF_PO tgl_gt_input.dat;;
# 				    "19") ln -s ./GATE_TOGGLE/to_Wang/${CIRCUIT_NAME}/renesas_tpi_fixed_gate+type_init_ave tgl_gt_input.dat;;
# 				    "20") ln -s ./GATE_TOGGLE/to_Wang/${CIRCUIT_NAME}/renesas_tpi_contribure_tgl_init_ave tgl_gt_input.dat;;
# 				    "21") ln -s /mnt/tiger/home/wang/renesas/Renesas-TPI/Renesas-TPI-FF-init-master/Renesas-TPI-FF-init-master/OUTPUTS/${CIRCUIT_NAME}/rtpi_gp_$GP tgl_gt_input.dat;;
# 				esac

# 				for CYCLE in 1
# 				do
# 				    echo ===${CIRCUIT_NAME}: $METHOD $CYCLE===========
# 				    time ./sim ${CIRCUIT_NAME} $TOOLMODE $TPG   $TGL_MODE $TGL_RATE $CAPTURE $METHOD $CYCLE $SKIP_CAP $cnt1 $OBRATE $GP $TEST_VEC
#     				    echo ===time end===
# 				done
# 				rm -f tgl_gt_input.dat
# 			    done
# 			done
# 		    else
#     			echo ==$FF_SEL_METHOD $SKIP_CAP===
# 			ln -s ./GATE_TOGGLE/to_Wang/${CIRCUIT_NAME}/Method_${FF_SEL_METHOD} ${CIRCUIT_NAME}_tgl_FF_input.dat

# 			for CYCLE in 1
# 			do
# 			    echo ===${CIRCUIT_NAME}: $CYCLE $SKIP_CAP===========
# 			    time ./sim ${CIRCUIT_NAME} $TOOLMODE $TPG $TGL_MODE $FF_RATE $CAPTURE $CYCLE $SKIP_CAP $FF_SEL_METHOD $cnt1 $OBRATE $TEST_VEC
#     			    echo ===time end===
# 			done
# 		    fi
# 		done
# 		rm -f ${CIRCUIT_NAME}_tgl_FF_input.dat
# 	    done
# 	    rm -f ${CIRCUIT_NAME}_tgl_ff_tpi.dat
# 	done
# 	rm -f ff_station_*.dat
# 	;;
# esac

# if [ -e fault_list.dat ];
# then
#     mv fault_list.dat "${CIRCUIT_NAME}"_fault_list.dat
#     if [ ! -e ./fault_list ];
#     then
#         mkdir fault_list
#     fi
#     mv "${CIRCUIT_NAME}"_fault_list.dat ./fault_list
# fi
# rm -f ${CIRCUIT_NAME} lfsr.dat *~ tmp*  ff_station.dat ATPG.dat *lfsr_pi.dat
