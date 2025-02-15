#!/bin/bash
datasets=("asia" "child" "insurance" "alarm" "cancer" "mildew" "water" "barley")
datasize1=(250 500 500 1000 250 8000 1000 2000)
datasize2=(1000 2000 2000 4000 1000 32000 4000 8000)
conf_dict=(0.5 0.6 0.7 0.8 0.9 0.99999)


# with prior
ancs_dict=("[(0,1),(1,5),(1,7),(2,3),(2,4),(3,5),(3,7),(4,7),(5,6)]" "[(0,2),(2,5),(3,9),(4,10),(5,12),(11,16),(11,19),(14,6),(17,4),(18,1)]" "[(1,12),(4,5),(5,19),(7,22),(8,16),(12,7),(12,9),(17,15),(18,15),(24,23)]" "[(3,1),(3,2),(5,4),(5,6),(6,35),(11,34),(13,14),(14,36),(16,17),(18,19),(19,20),(22,21),(24,17),(26,17),(26,25),(27,28),(28,17),(32,15),(33,34),(33,36),(35,36)]" "[(0,2),(1,2),(1,4),(2,3),(2,4)]"  "[(1,0),(2,1),(3,0),(4,0),(8,7),(9,8),(10,7),(11,7),(14,13),(15,14),(16,13),(17,13),(19,34),(20,19),(21,20),(22,19),(23,19),(24,34),(25,5),(26,5),(27,12),(28,12),(29,18),(30,18),(31,5),(32,12),(33,18)]" "[(1,8),(2,12),(2,13),(3,12),(6,15),(9,16),(10,20),(10,21),(11,20),(14,23),(17,24),(18,28),(18,29),(19,28),(22,31)]" "[(0,6),(0,7),(2,3),(3,7),(3,10),(5,3),(6,14),(8,9),(11,12),(13,14),(16,9),(16,18),(17,18),(21,23),(21,24),(25,32),(25,37),(26,33),(27,34),(28,30),(30,41),(35,27),(38,39),(40,20)]")
forb_ancs_dict=("[]" "[]" "[]" "[]" "[]" "[]" "[]" "[]")
output="exp/CaMML_prior.txt"
dag_path="exp/CaMML_record/prior"

for j in {0..5};
do
    conf=${conf_dict[$j]};
    for i in {0..7};
    do
        d=${datasets[$i]};
        anc=${ancs_dict[$i]};
        forb_anc=${forb_ancs_dict[$i]};
        for s in ${datasize1[$i]} ${datasize2[$i]};
        do
            for r in {1..6};
            do
                python CaMML_perform.py --d=$d --N=$s --ancs=${anc} --r=$r --forb_ancs=${forb_anc} --output=$output --dag_path=$dag_path --conf=$conf &
            done
        done
    done
    wait;
done

# without prior
# ancs_dict=("[]" "[]" "[]" "[]" "[]" "[]" "[]" "[]")
# forb_ancs_dict=("[]" "[]" "[]" "[]" "[]" "[]" "[]" "[]")
# output="exp/CaMML_noprior.txt"
# dag_path="exp/CaMML_record/noprior"


# for i in {0..7};
# do
#     d=${datasets[$i]};
#     anc=${ancs_dict[$i]};
#     forb_anc=${forb_ancs_dict[$i]};
#     for s in ${datasize1[$i]} ${datasize2[$i]};
#     do
#         for r in {1..6};
#         do
#             python CaMML_perform.py --d=$d --N=$s --ancs=${anc} --r=$r --forb_ancs=${forb_anc} --output=$output --dag_path=$dag_path &
#         done
#     done
# done
