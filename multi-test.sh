#!/usr/bin/bash

set -eu
function catch {
    echo "Error: Logic-BIST-Sim is failed!" >&2
}
trap catch ERR

#######Path Defination###############
User_DIR=$(cd "$(dirname "$0")";pwd)

if [ ! -e ${User_DIR}/OUTPUTS/MULTI_BIST/ ]; then
    mkdir -p ${User_DIR}/OUTPUTS/MULTI_BIST/
fi

TPG=0 #=0:LFSR,=1:ATPG
TEST_VEC=10 #0 00 # Number of Test patterns
TOOLMODE=2 #=1:Normal Scan test, =2:Multi-cycle Test, =3: Multi-cycle test with Seq OB, =4:Toggle Gate TPI
SKIPCYCLES=0
CAPTURE=5

for CIRCUIT in s9234

do
    rm -f lfsr*.dat ATPG.dat

    ln -s ./circuit/$CIRCUIT
    ln -s ./tpg/lfsr.dat

	rm -f ${CIRCUIT}_lfsr_pi.dat
	./tpg/lfsr $CIRCUIT $TEST_VEC ${CIRCUIT}_lfsr_pi.dat
	ln -s tmp.test ${CIRCUIT}_lfsr_pi.dat

	time ./lbistsim $CIRCUIT $TOOLMODE $TPG $CAPTURE $SKIPCYCLES #>> Switch_ctr.txt

    if [ -e fault_list.dat ];
    then
	mv fault_list.dat "$CIRCUIT"_fault_list.dat
	if [ ! -e ./fault_list ];
        then
            mkdir fault_list
	fi
	mv "$CIRCUIT"_fault_list.dat ./fault_list
    fi
    rm -f $CIRCUIT lfsr.dat *~ tmp* ATPG.dat *lfsr_pi.dat
done
