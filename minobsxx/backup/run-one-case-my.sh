dataset=$1
samples=$2
index=$3
cons_path=$4
gens=$5

./run-one-case.sh ./data/bdeu/"$dataset"_"$samples"_"$index".BDeu ${cons_path} tmp.output ${gens}