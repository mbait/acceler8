#!/bin/sh
set -e

grep 'physical id' /proc/cpuinfo | sort | uniq | wc -l | xargs printf 'Number of CPUs: %d\n'
grep 'core id' /proc/cpuinfo | sort | uniq | wc -l | xargs printf 'Number of cores: %d\n'

grep 'model name' /proc/cpuinfo | perl -anle 'print join" ",splice @F,3' | \
	sort | uniq | while read cpu
do
	echo $cpu
done
