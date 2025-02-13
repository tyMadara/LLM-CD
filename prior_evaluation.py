import numpy as np
import os
from myEva import MetricsDAG 
import re

def check_path(dag:np.array, source, dest):
    #检查是否有有向路径
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


def parse_pattern1(file_path):
    with open(file_path,"r") as fh:
        lines = fh.readlines()
    edges = []
    for line in lines:
        edge = re.findall(r"<edge>(.*?)</edge>",line)
        if len(edge) == 0:
            continue
        edge = edge[0]
        if edge=='variable1->variable2':
            continue
        edge = edge.split("->")
        edge = [e for e in edge]
        edges.append(edge)
    return edges

def parse_pattern2(file_path):
    with open(file_path,"r") as fh:
        lines = fh.readlines()
    edges = []
    for line in lines:
        edge = re.findall(r"<edge>(.*?)</edge>: Correct",line)
        if len(edge) == 0:
            continue
        edge = edge[0]
        if edge=='variable1->variable2':
            continue
        edge = edge.split("->")
        edge = [e for e in edge]
        edges.append(edge)
    return edges

def parse_pattern3(file_path):
    with open(file_path,"r") as fh:
        lines = fh.readlines()
    edges = []
    for line in lines:
        edge_list = re.findall(r"<edge>(.*?)</edge>",line)
        if len(edge_list) == 0:
            continue
        for edge in edge_list:
            if edge!='variable1->variable2':    
                edge = edge.split("->")
                edge = [e for e in edge]
                edges.append(edge)
    return edges

def check_GPT_prior(GPT_prior_path,BN_dir,map_dir,network): 
    
    true_dag = np.loadtxt(f"{BN_dir}/{network}_graph.txt", dtype=int)
    maps = np.loadtxt(f"{map_dir}/{network}.mapping",dtype=str)
    v2i, i2v = {}, {}
    for i,v in enumerate(maps):
        v2i[v] = i
        i2v[i] = v
        
    #不同网络抽取的方法可能有区别
    if network == 'alarm':
        parse_function=parse_pattern2 
        triple =  parse_function(f'{GPT_prior_path}/GPT4_{network}.txt')
    elif network=='mildew':  
        parse_function=parse_pattern3
        triple =  parse_function(f'{GPT_prior_path}/GPT4_{network}.txt')
    else:
        parse_function=parse_pattern1
        triple =  parse_function(f'{GPT_prior_path}/GPT4_{network}.txt')

    ev_dag = np.zeros((len(maps),len(maps)))
    for t in triple:
        v1,v2 = t
        ev_dag[v2i[v1]][v2i[v2]] = 1

    cause=[]
    right_cause=[]
    err_cause = []
    causality = 0
    for pos in np.argwhere(ev_dag==1):
        cause.append((pos[0],pos[1]))
        if check_path(true_dag,pos[0],pos[1]):
            causality += 1
            right_cause.append((pos[0],pos[1]))
        else:
            print(i2v[pos[0]],i2v[pos[1]])
            err_cause.append((pos[0],pos[1]))
    print(f"正确先验",str(right_cause).replace(' ',''))
    print(f"总先验:", str(cause).replace(' ',''))
    print(f"总数:{len(cause)}")
    print(f"正确数量:{causality}")
    print('===================')
    return cause,right_cause,err_cause


def Path_graph_metric(output,cause_list):
    
    true_dag = np.loadtxt(f"{BN_dir}/{dataset}_graph.txt", dtype=int)
    
    path_count=0
    path_dag=np.full_like(true_dag,0)
    path_true_dag=np.full_like(true_dag,0)
    for i in range(len(true_dag)):
        for j in range(len(true_dag)):
            if i!=j and check_path(true_dag,i,j)==1:
                path_count+=1
                path_true_dag[i,j]=1
            else:
                path_true_dag[i,j]=0
    for cause in cause_list:        
        path_dag[cause[0],cause[1]]=1
    
    from myEva import MetricsDAG
    m = MetricsDAG(path_dag, path_true_dag)
    m.metrics['path_count']=path_count
    print(f"{dataset}\n{m.metrics}")
    with open(output,'a') as f:
        f.write(f"{dataset}\n {m.metrics}\n")
    print('===================')


if __name__ == '__main__':
    
    GPT_prior_path='LLM_query/GPT4'
    BN_dir = "BN_structure"
    map_dir = "BN_structure/mappings"
        
    for dataset in ['asia','child','insurance','alarm','cancer','barley','mildew','water']:
                
        print(dataset)
        cause,_,_=check_GPT_prior(GPT_prior_path,BN_dir,map_dir,dataset)
        #Path_graph_metric("exp/path_prior_evaluation.txt",cause)
    

        