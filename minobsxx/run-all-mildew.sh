#!/bin/bash

eval "sbatch --time=40:00:00 --mem-per-cpu=1024 ./run-all.sh data/bdeu/mildew_8000_1.BDeu data/experiment2-constraints/mildew data/bdeu/mildew_8000_1_results 10"
eval "sbatch --time=40:00:00 --mem-per-cpu=1024 ./run-all.sh data/bdeu/mildew_8000_2.BDeu data/experiment2-constraints/mildew data/bdeu/mildew_8000_2_results 10"
eval "sbatch --time=40:00:00 --mem-per-cpu=1024 ./run-all.sh data/bdeu/mildew_8000_3.BDeu data/experiment2-constraints/mildew data/bdeu/mildew_8000_3_results 10"
eval "sbatch --time=40:00:00 --mem-per-cpu=1024 ./run-all.sh data/bdeu/mildew_8000_4.BDeu data/experiment2-constraints/mildew data/bdeu/mildew_8000_4_results 10"
eval "sbatch --time=40:00:00 --mem-per-cpu=1024 ./run-all.sh data/bdeu/mildew_8000_5.BDeu data/experiment2-constraints/mildew data/bdeu/mildew_8000_5_results 10"
eval "sbatch --time=40:00:00 --mem-per-cpu=1024 ./run-all.sh data/bdeu/mildew_8000_6.BDeu data/experiment2-constraints/mildew data/bdeu/mildew_8000_6_results 10"

eval "sbatch --time=40:00:00 --mem-per-cpu=1024 ./run-all.sh data/bdeu/mildew_32000_1.BDeu data/experiment2-constraints/mildew data/bdeu/mildew_32000_1_results 10"
eval "sbatch --time=40:00:00 --mem-per-cpu=1024 ./run-all.sh data/bdeu/mildew_32000_2.BDeu data/experiment2-constraints/mildew data/bdeu/mildew_32000_2_results 10"
eval "sbatch --time=40:00:00 --mem-per-cpu=1024 ./run-all.sh data/bdeu/mildew_32000_3.BDeu data/experiment2-constraints/mildew data/bdeu/mildew_32000_3_results 10"
eval "sbatch --time=40:00:00 --mem-per-cpu=1024 ./run-all.sh data/bdeu/mildew_32000_4.BDeu data/experiment2-constraints/mildew data/bdeu/mildew_32000_4_results 10"
eval "sbatch --time=40:00:00 --mem-per-cpu=1024 ./run-all.sh data/bdeu/mildew_32000_5.BDeu data/experiment2-constraints/mildew data/bdeu/mildew_32000_5_results 10"
eval "sbatch --time=40:00:00 --mem-per-cpu=1024 ./run-all.sh data/bdeu/mildew_32000_6.BDeu data/experiment2-constraints/mildew data/bdeu/mildew_32000_6_results 10"
