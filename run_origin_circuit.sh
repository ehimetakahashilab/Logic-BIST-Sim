#!/usr/bin/bash

set -eu
function catch {
    echo "Error: Logic-BIST-Sim is failed!" >&2
}
trap catch ERR

APP_DIR=$(dirname $0)

## OUTPUT path check
OUTPUT_PATH=${APP_DIR}/OUTPUTS/MULTI_BIST
if [ ! -e ${OUTPUT_PATH} ]; then
    mkdir -p ${OUTPUT_PATH}
fi

## FLT_LIST path check
FLT_LIST_PATH=${APP_DIR}/FLT_LIST
if [ ! -e ${FLT_LIST_PATH} ]; then
    mkdir -p ${FLT_LIST_PATH}
fi

CIRCUIT_PATH=$1
if [ ! -e ${CIRCUIT_PATH} ]; then 
    echo "cannot read Circuit File" >&2
    exit 1
fi
CIRCUIT_NAME=$(basename ${CIRCUIT_PATH})
LFSR_CONFIG_PATH=${APP_DIR}/src/lfsr.dat

TPG=0
TEST_VEC=100000 # Number of Test patterns
TOOLMODE=2
SKIPCYCLES=0
CAPTURE=10

${APP_DIR}/src/tpg/tpg/lfsr ${CIRCUIT_PATH} ${TEST_VEC} ${LFSR_CONFIG_PATH} ${CIRCUIT_NAME}_lfsr_pi.dat
${APP_DIR}/src/simulator/sim ${CIRCUIT_PATH} ${TOOLMODE} ${TPG} ${CAPTURE} ${SKIPCYCLES}

if [ -e ${APP_DIR}/fault_list.dat ]; then
    mv ${APP_DIR}/fault_list.dat ${APP_DIR}/${CIRCUIT_NAME}_fault_list.dat
	mv ${APP_DIR}/${CIRCUIT_NAME}_fault_list.dat ${FLT_LIST_PATH}
fi

# delete LFSR generated pattern
rm -f ${CIRCUIT_NAME}_lfsr_pi.dat