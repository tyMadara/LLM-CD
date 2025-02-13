#!/bin/bash
#SBATCH --time=16:00:00
#SBATCH --mem-per-cpu=4096

instance=$1
constraints=$2
gens=$3



i=0

for scoreFile in $(eval ls "data/bdeu/${instance}_*.BDeu"); do

	for constraintFile in $(eval ls "data/all-constraints/${constraints}/*"); do


		if [ $i -eq $SLURM_ARRAY_TASK_ID ]; then
			echo "${csvFile} ${constraintFile}"

			baseBDeu=$(basename ${scoreFile})
			baseBDeuExt="${baseBDeu##*.}"
			baseBDeu="${baseBDeu%.*}"

			baseConstraint=$(basename ${constraintFile})
			cmd="./run-one-case.sh $scoreFile $constraintFile data/bdeu/${baseBDeu}_results $gens"
			eval "$cmd"
			exit
		fi
		i=$((i + 1))
	done
done