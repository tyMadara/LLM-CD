#!/bin/bash

eval "sbatch --time=4:00:00 ./run-all.sh data/bdeu/child_500_1.BDeu data/experiment2-constraints/child data/bdeu/child_500_1_results 10"
eval "sbatch --time=4:00:00 ./run-all.sh data/bdeu/child_500_2.BDeu data/experiment2-constraints/child data/bdeu/child_500_2_results 10"
eval "sbatch --time=4:00:00 ./run-all.sh data/bdeu/child_500_3.BDeu data/experiment2-constraints/child data/bdeu/child_500_3_results 10"
eval "sbatch --time=4:00:00 ./run-all.sh data/bdeu/child_500_4.BDeu data/experiment2-constraints/child data/bdeu/child_500_4_results 10"
eval "sbatch --time=4:00:00 ./run-all.sh data/bdeu/child_500_5.BDeu data/experiment2-constraints/child data/bdeu/child_500_5_results 10"
eval "sbatch --time=4:00:00 ./run-all.sh data/bdeu/child_500_6.BDeu data/experiment2-constraints/child data/bdeu/child_500_6_results 10"

eval "sbatch --time=4:00:00 ./run-all.sh data/bdeu/child_2000_1.BDeu data/experiment2-constraints/child data/bdeu/child_2000_1_results 10"
eval "sbatch --time=4:00:00 ./run-all.sh data/bdeu/child_2000_2.BDeu data/experiment2-constraints/child data/bdeu/child_2000_2_results 10"
eval "sbatch --time=4:00:00 ./run-all.sh data/bdeu/child_2000_3.BDeu data/experiment2-constraints/child data/bdeu/child_2000_3_results 10"
eval "sbatch --time=4:00:00 ./run-all.sh data/bdeu/child_2000_4.BDeu data/experiment2-constraints/child data/bdeu/child_2000_4_results 10"
eval "sbatch --time=4:00:00 ./run-all.sh data/bdeu/child_2000_5.BDeu data/experiment2-constraints/child data/bdeu/child_2000_5_results 10"
eval "sbatch --time=4:00:00 ./run-all.sh data/bdeu/child_2000_6.BDeu data/experiment2-constraints/child data/bdeu/child_2000_6_results 10"
