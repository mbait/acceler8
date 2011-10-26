#!/bin/sh
set -e

test -x $1 || (echo 'usage: sh test.sh <executable>' && exit 1)
test -d test/out || mkdir test/out

echo 'real user system cells'

for f in test/*.in
do
	(time $1 $f ans > /dev/null) 2>&1 | tail -n+2 | perl util/check.pl $f
done
