#!/bin/sh
NUM_CORES=${1:-40}
MSP_EXE=${2:-msp_rtl}

qsub \
	-l select=1:ncpus=$NUM_CORES \
	-v MSP_EXE=$MSP_EXE,OMP_NUM_THREADS=$NUM_CORES \
	-N "$MSP_EXE-$NUM_CORES" \
	./run.sh
