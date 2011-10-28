#!/bin/sh

export PID=${PBS_JOBID:-$$}
export DIST_DIR=${PBS_O_WORKDIR:-$PWD}
export MSP_EXEC=${1:-msp_rtl}
export OMP_NUM_THREADS=${2:-30}
sh $DIST_DIR/util/test.sh $DIST_DIR/out/$MSP_EXEC > $DIST_DIR/test.out.$PID
