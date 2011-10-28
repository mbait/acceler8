#!/bin/sh
NUM_CORES=${2:-40}
MSP_EXE=${1:-msp_rtl}

qsub -l select=1:ncpus=$NUM_CORES -v MSP_EXE=$MSP_EXE,OMP_NUM_THREADS=$NUM_CORES ./run.sh
