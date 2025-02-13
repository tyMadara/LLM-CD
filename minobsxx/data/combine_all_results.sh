#!/bin/bash

base=$1
cur=$(pwd)

base1="$cur/bdeu/${base}_1_all_results"
base2="$cur/bdeu/${base}_2_all_results"
base3="$cur/bdeu/${base}_3_all_results"
base4="$cur/bdeu/${base}_4_all_results"
base5="$cur/bdeu/${base}_5_all_results"
base6="$cur/bdeu/${base}_6_all_results"


cat $base1 >> $cur/bdeu/"${base}_all_results"
cat $base2 >> $cur/bdeu/"${base}_all_results"
cat $base3 >> $cur/bdeu/"${base}_all_results"
cat $base4 >> $cur/bdeu/"${base}_all_results"
cat $base5 >> $cur/bdeu/"${base}_all_results"
cat $base6 >> $cur/bdeu/"${base}_all_results"

rm $base1
rm $base2
rm $base3
rm $base4
rm $base5
rm $base6