#!/bin/bash
###
 # @Author: luyuxiao luyuxiao0702@126.com
 # @Date: 2022-12-29 22:01:13
 # @LastEditors: Ban Taiyu
 # @LastEditTime: 2023-03-06 20:34:11
 # @FilePath: /知识驱动因果发现/MINOBS-anc-master/run-one-case-asia.sh
 # @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
### 

dataset=$1
samples=$2
index=$3

./run-one-case.sh ./data/bdeu/"$dataset"_"$samples"_"$index".BDeu ./data/all-constraints/"$dataset"/"$dataset"-0.1-1 tmp.output 10