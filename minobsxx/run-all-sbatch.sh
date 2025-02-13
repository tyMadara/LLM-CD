#!/bin/bash

scores=$1
dir=$2
outFile=$3
gens=$4

for file in $dir/*; do
	cmd="sbatch --time=16:00:00 --mem-per-cpu=4096 ./run-one-case.sh $scores $file $outFile $gens"
	eval "$cmd"
done