## CaMML
import numpy as np
import re
import networkx as nx
import random
import argparse

## MINOBSxx
import re
import random

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


def parse_screen_output(file_path, n):
    res = np.zeros((n,n))
    with open(file_path, "r", encoding="utf-8") as ifh:
        for line in ifh:
            mt = re.match(r".*=\s(\d+).*\{(.*)\}.*", line)
            if not mt:
                continue
            child = mt.group(1)
            parents = mt.group(2)
            parents = re.split(r"\s+",parents.strip())
            c = int(child)
            for p in parents:
                if p == "":
                    continue
                p = int(p)
                res[p,c] = 1
    return res

def MINOBSx_unit(d, s, r, anc =[], forb_anc = [], order = [], abs_edge=[], MINOBSx_base="minobsxx", timeout=None, iter=10, prefix=""):
    import os
    
    os.chdir(MINOBSx_base)
    
    anc_path = f"anc_file/{prefix}{d}_{s}_{r}.anc"
    out_path = f"out_BNs/{prefix}{d}_{s}_{r}.dne"
    
    with open(anc_path, "w", encoding="utf-8") as ofh:
        ofh.write(f"0\n0\n{len(abs_edge)}\n")
        for c in  abs_edge:
            ofh.write(f"{c[0]} {c[1]}\n")
        ofh.write(f"{len(order)}\n")
        for c in  order:
            ofh.write(f"{c[0]} {c[1]}\n")
        ofh.write(f"{len(anc)}\n")
        for c in  anc:
            ofh.write(f"{c[0]} {c[1]}\n")
        ofh.write(f"{len(forb_anc)}\n")
        for c in forb_anc:
            ofh.write(f"{c[0]} {c[1]}\n")
    if timeout is None:
        os.system(f"./run-one-case-my.sh {d} {s} {r} {anc_path} {iter} > {out_path}")
    else:
        os.system(f"timeout {timeout} ./run-one-case-my.sh {d} {s} {r} {anc_path} {iter} > {out_path}")
    ev_dag = parse_screen_output(f"{out_path}", true_dag.shape[0])
    os.system(f"rm {anc_path}")
    os.system(f"rm {out_path}")
    os.chdir("..")
    return ev_dag


## MINOBSx performance exp
import os
import numpy as np
from myEva import MetricsDAG

parser = argparse.ArgumentParser(description='experiments on various ')
parser.add_argument('--ancs', type=str, default='[]')
parser.add_argument('--forb_ancs', type=str, default='[]')
parser.add_argument('--order', type=str, default='[]')
parser.add_argument('--abs_edges', type=str, default='[]')
parser.add_argument('--d', type=str, default="asia")
parser.add_argument('--N', type=int, default=250)
parser.add_argument('--l', type=int, default=0)
parser.add_argument('--r', type=int, default=1)
parser.add_argument('--output', type=str, default="")
parser.add_argument('--dag_path', type=str, default="")


args = parser.parse_args()
ancs = eval(args.ancs)
forb_ancs = eval(args.forb_ancs)
order = eval(args.order)
abs_edges=eval(args.abs_edges)
d = args.d
s = args.N
l = args.l
r = args.r
output=args.output
dag_path=args.dag_path

BN_dir = "BN_structure/"


true_dag = np.loadtxt(f'BN_structure/{d}_graph.txt',dtype=int)

import time
start = time.time()
ev_dag = MINOBSx_unit(d, s, r, anc =ancs, forb_anc = forb_ancs, order = order, abs_edge=abs_edges, prefix=f"MINONSx_perform_")
end = time.time()

m = MetricsDAG(ev_dag, true_dag)
m.metrics["time"] = end - start

m.metrics["ancs_statisfy"]=0
for prior in ancs:
    if check_path(ev_dag,prior[0],prior[1]):
        m.metrics["ancs_statisfy"]+=1

m.metrics["abs_edge_statisfy"]=0
m.metrics["abs_edge_notstatisfy"]=0
for prior in abs_edges:
    if ev_dag[prior[0],prior[1]]==0:
        m.metrics["abs_edge_statisfy"]+=1
    else:
        m.metrics["abs_edge_notstatisfy"]+=1
print(f"{d} {s} {r}\n{m.metrics}")
with open(output,'a') as f:
    f.write(f"{d} {s} {r}\n {m.metrics}\n")


np.savetxt(f"{dag_path}/MINOBSx_{d}_{s}_{r}.txt", ev_dag, fmt="%d")