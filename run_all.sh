#!/usr/bin/bash

TEST_FILES_PATH="/home/mainek00n/github/github.com/ehimetakahashilab/test_files"

# parallel --results ${TEST_FILES_PATH}/no_dft/logs '/home/mainek00n/github/github.com/ehimetakahashilab/Logic-BIST-Sim/run_origin_circuit.sh /home/mainek00n/github/github.com/ehimetakahashilab/seq-benchmark/circuits/iscas89/{}' ::: s9234 s13207 s15850 s38417 s38584

# for mode in "MulTPI" "Separate"
# do
#     for n_iter in 1
#     do
#         for cpi in "Hybrid-TPI" "Logic-CPI"
#         do
#             for topsis in "COP_Structure" "Structure"
#             do
#                 parallel --results ${TEST_FILES_PATH}/${mode}/${n_iter}iter/${cpi}/${topsis}/logs "/home/mainek00n/github/github.com/ehimetakahashilab/Logic-BIST-Sim/run_cp_op_circuit.sh /home/mainek00n/github/github.com/ehimetakahashilab/seq-benchmark/circuits/iscas89/{} ${TEST_FILES_PATH}/${mode}/${n_iter}iter/${cpi}/${topsis}/lab/fflist/{}/FF_STATION/TOPSIS ${TEST_FILES_PATH}/${mode}/${n_iter}iter/${cpi}/${topsis}/lab/cplist/{}" ::: s9234 s13207 s15850 s38417 s38584
#             done
#         done
#     done
# done

for mode in "MulTPI" "MulTPI_reverse" "Separate"
do
    for n_iter in 1 5 10
    do
        for cpi in "CPSC_top" "CPSF_top"
        do
            for topsis in "COP_Structure" "Structure"
            do
                parallel --results ${TEST_FILES_PATH}/${mode}/${n_iter}iter/${cpi}/${topsis}/logs "/home/mainek00n/github/github.com/ehimetakahashilab/Logic-BIST-Sim/run_cp_op_circuit.sh /home/mainek00n/github/github.com/ehimetakahashilab/seq-benchmark/circuits/iscas89/{} ${TEST_FILES_PATH}/${mode}/${n_iter}iter/${cpi}/${topsis}/lab/fflist/{}/FF_STATION/TOPSIS ${TEST_FILES_PATH}/${mode}/${n_iter}iter/${cpi}/${topsis}/lab/cplist/{}" ::: s9234 s13207 s15850 s38417 s38584
                cp -r /home/mainek00n/github/github.com/ehimetakahashilab/Logic-BIST-Sim/OUTPUTS/FLT_DET_LIST ${TEST_FILES_PATH}/${mode}/${n_iter}iter/${cpi}/${topsis}/logs
            done
        done
    done
done