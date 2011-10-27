#!/bin/sh
set -e

die()
{
	echo >&2 "$@"
	exit 1
}

WD=${DIST_DIR:-$PWD}

[ "$#" -eq 1 ]  && test -x $1 || die 'usage: sh test.sh <executable>'
test -d test/out || mkdir $WD/test/out

echo 'cells real user system'

for f in $WD/test/*.in
do
	(time $1 $f ans > /dev/null) 2>&1 | tail -n+2 | perl $WD/util/time.pl $f
done
