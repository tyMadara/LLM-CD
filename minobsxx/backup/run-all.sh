#!/bin/bash

scores=$1
dir=$2
outFile=$3
gens=$4

for file in $dir/*; do
	cmd="./run-one-case.sh $scores $file $outFile $4"
	eval "$cmd"
done