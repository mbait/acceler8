#!/bin/sh

JOBID=${PBS_JOBID:-$$}
DIST_DIR=${PBS_O_WORKDIR:-$PWD}
DIST_DIR=$DIST_DIR sh $DIST_DIR/util/test.sh $DIST_DIR/out/msp_rtl > $DIST_DIR/test.out.$JOBID
