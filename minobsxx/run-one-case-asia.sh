dataset=$1
samples=$2
index=$3

./run-one-case.sh ./data/bdeu/"$dataset"_"$samples"_"$index".BDeu ./data/all-constraints/"$dataset"/"$dataset"-0.1-1 tmp.output 10