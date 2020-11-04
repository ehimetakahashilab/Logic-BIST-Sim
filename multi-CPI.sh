#!/usr/bin/bash

set -eu
function catch {
    echo "Error: Logic-BIST-Sim is failed!" >&2
}
trap catch ERR

#######Path Defination###############
User_DIR=$(cd "$(dirname "$0")";pwd)

OP_LIST_DIR=${User_DIR}/FF_STATION
CP_LIST_DIR=${User_DIR}/CP_STATION

if [ ! -e ${OP_LIST_DIR} ]; then
    mkdir -p ${OP_LIST_DIR}
fi

if [ ! -e ${CP_LIST_DIR} ]; then
    mkdir -p ${CP_LIST_DIR}/LCP
    mkdir -p ${CP_LIST_DIR}/FFCP
fi

TPG=0 #=0:LFSR,=1:ATPG
TEST_VEC=10 #0 00 # Number of Test patterns
TOOLMODE=4 #=1:Normal Scan test, =2:Multi-cycle Test, =3: Multi-cycle test with Seq OB, =4:Toggle Gate TPI


for CIRCUIT in s9234

do
    rm -f lfsr*.dat ATPG.dat

    ln -s ./circuit/$CIRCUIT
    ln -s ./tpg/lfsr.dat

	rm -f ${CIRCUIT}_lfsr_pi.dat
	./tpg/lfsr $CIRCUIT $TEST_VEC ${CIRCUIT}_lfsr_pi.dat
	ln -s tmp.test ${CIRCUIT}_lfsr_pi.dat

    if [ ! -e ${OP_LIST_DIR}/${CIRCUIT} ]; then
	mkdir -p ${OP_LIST_DIR}/${CIRCUIT}
    fi


    CAPTURE=5 #the number of capture cycles
    OBRATE=0.2 #ratio of OP FF
    LCP_rate=0.01 #the ratio of Logic CPs in all logic gates
    CP_GROUP=1 #=1: the number of CPs selected at each iteration

    FFCP_rate=0.1	#the ration of FF-CPs in all FFs
    SKIP_CAP=3 #SKIP_CAP=3, the CP control starts from the third capture cycle.
    INTERVAL_CYCLE=1 #=1: the number of interval cycles

    if [ ! -e ${User_DIR}/OUTPUTS/CPI/${CAPTURE}_cycles/ ]; then
	mkdir -p ${User_DIR}/OUTPUTS/CPI/${CAPTURE}_cycles/
    fi

    cnt1=0
    for ff_sta_file in  ./FF_STATION/$CIRCUIT/TOPSIS
    do

	if [ -e $ff_sta_file ];
	then
 	    echo === OB_STATION INFO of $ff_sta_file is FOUND, CONTINUE THE FAULT SIM PROCESS===
	else
 	    echo === NO OB_STATION INFO, PLEASE BACK TO FF_SELECTION PROCESS to Generate $ff_sta_file===
	    exit
	fi

	ln -s ${ff_sta_file} ff_station_${cnt1}.dat
	let cnt1=${cnt1}+1
    done

    for CP_CTRL in 1
		   #=0:non CP control, =1: Logic gate toggling, =4: applying random patterns to Logic CPs
    do
	## when CP runs in random loading mode, the random patterns will be pre-generated by the following Setting
	if [ "$CP_CTRL" = "4" ];
	then
 	    echo ===  Generate the LFSR Vectors 4 Logic_CPI===
	    ./tpg_TPI_gate/lfsr_gt $CIRCUIT $TEST_VEC $LCP_rate $CAPTURE tgl_gt_tpi.dat
	    ln -sf tmp_gt.dat tgl_gt_tpi.dat
 	    echo ===  end ===
	fi

	if [ "$CP_CTRL" = "1" ] || [ "$CP_CTRL" = "4" ];
	then
	    echo ==simulation for Logic CPI ===

	    #  CP_LIST=0 # =0: normal

	    ln -s ${CP_LIST_DIR}/LCP/${CIRCUIT}/rlcp_$CP_GROUP  tgl_gt_input.dat

	    echo ===$CIRCUIT: $CP_GROUP $INTERVAL_CYCLE===========
	    time ./lbistsim $CIRCUIT $TOOLMODE $TPG $CP_CTRL $LCP_rate $CAPTURE $INTERVAL_CYCLE $SKIP_CAP $cnt1 $OBRATE $CP_GROUP $TEST_VEC
	    echo ===logic CPI simulation process end===
	    rm -rf tgl_gt_input.dat
	fi

    done
    rm -f ff_station_*.dat
done

if [ -e fault_list.dat ];
then
    mv fault_list.dat "$CIRCUIT"_fault_list.dat
    if [ ! -e ./fault_list ];
    then
        mkdir fault_list
    fi
    mv "$CIRCUIT"_fault_list.dat ./fault_list
fi
rm -f $CIRCUIT lfsr.dat *~ tmp*  ff_station.dat ATPG.dat *lfsr_pi.dat
