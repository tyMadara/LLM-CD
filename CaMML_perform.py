## CaMML
import numpy as np
import re
import networkx as nx
import random
import argparse

def check_path(dag:np.array, source, dest):
    n = dag.shape[0]
    visited = np.zeros(n)
    visited[source] = 1
    queue = [source]
    while len(queue) > 0:
        v = queue.pop(0)
        for i in range(n):
            if dag[v][i] == 1 and visited[i] == 0:
                visited[i] = 1
                queue.append(i)
    return visited[dest] == 1


def parse_CaMML_output(file_path, dag_flag=True):
    node_pattern = r"node\s+(\S+)\s+{.*"
    parents_pattern = r".*parents.*\((.*)\).*"
    node_list = []
    parent_list = []
    with open(file_path,"r",encoding="utf-8") as ifh:
        for line in ifh.readlines():
            m1 = re.match(node_pattern, line)
            if m1:
                node = m1.group(1).strip()
                node_list.append(node)
                continue
            m2 = re.match(parents_pattern, line)
            if m2:
                parents = m2.group(1).strip()
                if parents == "":
                    parent_list.append([])
                else:
                    tp = parents.split(",")
                    tp = [e.strip() for e in tp]
                    parent_list.append(tp)
    p_dict = {}
    n2i = {}
    for i,n in enumerate(node_list):
        n2i[n] = i
        p_dict[n] = parent_list[i]
    
    if not dag_flag:
        return n2i, p_dict
    
    n = len(n2i)
    CaMML_dag = np.zeros((n,n))
    for v in p_dict:
        v1 = n2i[v]
        for p in p_dict[v]:
            p1 = n2i[p]
            CaMML_dag[p1][v1] = 1
    return CaMML_dag


## CaMML performance exp
import os
import numpy as np
from myEva import MetricsDAG

def CaMML_unit(d, s, r, conf, anc =[], forb_anc = [], abs_edges=[], CaMML_base="BI-CaMML",  prefix=""):
    import os
    
    os.chdir(CaMML_base)
    
    anc_path = f"ancs/{prefix}{d}_{s}_{r}.anc"
    out_path = f"output/{prefix}{d}_{s}_{r}.dne"
    # anc = []
    # forb_anc = []

    ancestrals = anc
    with open(anc_path,"w") as ofh:
        reconf=1-conf
        out_str = "arcs{\n"
        for a in ancestrals:
            v1,v2 = a
            out_str += f"{i2p[v1]} => {i2p[v2]} {conf};\n"
        for a in forb_anc:
            v1,v2 = a
            out_str += f"{i2p[v1]} => {i2p[v2]} {reconf};\n"
        for a in abs_edges:
            v1,v2 =a
            out_str += f"{i2p[v1]} -> {i2p[v2]} {reconf:.5f};\n"
        out_str += "}"
        ofh.write(out_str)

    os.system(f"./camml.sh -p {anc_path} data/{d}_{s}_{r}.csv {out_path} > log.txt")
    ev_dag = parse_CaMML_output(out_path)
    
    os.system(f"rm {anc_path}")
    os.system(f"rm {out_path}")
    os.chdir("..")
    return ev_dag

parser = argparse.ArgumentParser(description='experiments on various ')
parser.add_argument('--ancs', type=str, default='[]')
parser.add_argument('--forb_ancs', type=str, default='[]')
parser.add_argument('--abs_edges', type=str, default='[]')
parser.add_argument('--d', type=str, default="asia")
parser.add_argument('--N', type=int, default=250)
parser.add_argument('--l', type=int, default=0)
parser.add_argument('--r', type=int, default=0)
parser.add_argument('--output', type=str, default="")
parser.add_argument('--dag_path', type=str, default="")
parser.add_argument('--conf', type=float, default=0.99999)

args = parser.parse_args()
ancs = eval(args.ancs)
forb_ancs = eval(args.forb_ancs)
abs_edges = eval(args.abs_edges)
d = args.d
s = args.N
l = args.l
r = args.r
conf=args.conf
output=args.output
dag_path=args.dag_path


BN_dir = "BN_structure/"
map_dir = "BN_structure/mappings/"


indexes = np.loadtxt(f"{map_dir}/{d}_abb.mapping",dtype=str)
p2i = {}
i2p = {}

for i,p in enumerate(indexes):
    p2i[p] = i
    i2p[i] = p
    
true_dag = np.loadtxt(f'BN_structure/{d}_graph.txt',dtype=int)

import time
start = time.time()
ev_dag = CaMML_unit(d, s, r, conf, ancs, forb_ancs, abs_edges, "BI-CaMML")
end = time.time()

m = MetricsDAG(ev_dag, true_dag)
m.metrics["time"] = end - start

m.metrics["statisfy"]=0
for prior in ancs:
    if check_path(ev_dag,prior[0],prior[1]):
        m.metrics["statisfy"]+=1

print(f"{d} {s} {r} {conf}\n{m.metrics}")
with open(output,'a') as f:
    f.write(f"{d} {s} {r} {conf}\n {m.metrics}\n")

np.savetxt(f"{dag_path}/CaMML_{d}_{s}_{r}_{conf}.txt", ev_dag, fmt="%d")