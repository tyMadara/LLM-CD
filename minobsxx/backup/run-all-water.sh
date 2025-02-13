#!/bin/bash

eval "sbatch --time=40:00:00 --mem-per-cpu=1024 ./run-all.sh data/bdeu/water_1000_1.BDeu data/experiment2-constraints/water data/bdeu/water_1000_1_results 10"
eval "sbatch --time=40:00:00 --mem-per-cpu=1024 ./run-all.sh data/bdeu/water_1000_2.BDeu data/experiment2-constraints/water data/bdeu/water_1000_2_results 10"
eval "sbatch --time=40:00:00 --mem-per-cpu=1024 ./run-all.sh data/bdeu/water_1000_3.BDeu data/experiment2-constraints/water data/bdeu/water_1000_3_results 10"
eval "sbatch --time=40:00:00 --mem-per-cpu=1024 ./run-all.sh data/bdeu/water_1000_4.BDeu data/experiment2-constraints/water data/bdeu/water_1000_4_results 10"
eval "sbatch --time=40:00:00 --mem-per-cpu=1024 ./run-all.sh data/bdeu/water_1000_5.BDeu data/experiment2-constraints/water data/bdeu/water_1000_5_results 10"
eval "sbatch --time=40:00:00 --mem-per-cpu=1024 ./run-all.sh data/bdeu/water_1000_6.BDeu data/experiment2-constraints/water data/bdeu/water_1000_6_results 10"

eval "sbatch --time=40:00:00 --mem-per-cpu=1024 ./run-all.sh data/bdeu/water_4000_1.BDeu data/experiment2-constraints/water data/bdeu/water_4000_1_results 10"
eval "sbatch --time=40:00:00 --mem-per-cpu=1024 ./run-all.sh data/bdeu/water_4000_2.BDeu data/experiment2-constraints/water data/bdeu/water_4000_2_results 10"
eval "sbatch --time=40:00:00 --mem-per-cpu=1024 ./run-all.sh data/bdeu/water_4000_3.BDeu data/experiment2-constraints/water data/bdeu/water_4000_3_results 10"
eval "sbatch --time=40:00:00 --mem-per-cpu=1024 ./run-all.sh data/bdeu/water_4000_4.BDeu data/experiment2-constraints/water data/bdeu/water_4000_4_results 10"
eval "sbatch --time=40:00:00 --mem-per-cpu=1024 ./run-all.sh data/bdeu/water_4000_5.BDeu data/experiment2-constraints/water data/bdeu/water_4000_5_results 10"
eval "sbatch --time=40:00:00 --mem-per-cpu=1024 ./run-all.sh data/bdeu/water_4000_6.BDeu data/experiment2-constraints/water data/bdeu/water_4000_6_results 10"
