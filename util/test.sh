#!/bin/sh
set -e

die()
{
	echo >&2 "$@"
	exit 1
}

PID=${PID:-$$}
WD=${DIST_DIR:-$PWD}
OUT_DIR=$WD/test/out.$PID

[ "$#" -eq 1 ] && test -f $1 && test -x $1 || die 'usage: sh test.sh <executable>'
test -d $OUT_DIR || mkdir $OUT_DIR

echo 'cells real user system'

for f in $WD/test/*.in
do
	ans=$OUT_DIR/$(echo $f | sed 's/.in$/.ans/' | xargs basename)
	(time $1 $f $ans > /dev/null) 2>&1 | tail -n+2 | perl $WD/util/time.pl $f
done
