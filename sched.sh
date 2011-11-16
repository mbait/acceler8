#!/bin/sh
NUM_CORES=${1:-40}
MSP_EXE=${2:-msp_rtl}
TIME=00:30:00

qsub \
	-l select=1:ncpus=$NUM_CORES,walltime=$TIME \
	-v MSP_EXE=$MSP_EXE,OMP_NUM_THREADS=$NUM_CORES \
	-N "$MSP_EXE-$(printf "%02d" "$NUM_CORES")" \
	./run.sh
