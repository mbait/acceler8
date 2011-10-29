#!/bin/sh

NAME=${PBS_JOBNAME:-local}
export PID=${PBS_JOBID:-$$}
export DIST_DIR=${PBS_O_WORKDIR:-$PWD}
export OMP_NUM_THREADS=$OMP_NUM_THREADS
sh $DIST_DIR/util/test.sh $DIST_DIR/out/$MSP_EXE | sort -n > "$DIST_DIR/test.out.$PID.$NAME"
