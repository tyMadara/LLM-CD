import numpy as np
import os
from myEva import MetricsDAG 
from prior_evaluation import check_GPT_prior

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

GPT_prior_path='LLM_query/GPT4'
BN_dir = "BN_structure"
map_dir = "BN_structure/mappings"

result_dag_dir='exp/LLM_record'
datasets=("asia","child","insurance","alarm","cancer","mildew","water","barley")
datasize1=(250,500,500,1000,250,8000,1000,2000)
datasize2=(1000,2000,2000,4000,1000,32000,4000,8000)


def prior_accept_evaluation(type='prior',output="exp/prior_accept_evaluation.txt"):
    for i,dataset in enumerate(datasets):
        cause_list,cause_right,cause_wrong=check_GPT_prior(GPT_prior_path,BN_dir,map_dir,dataset)
        for j in range(1,7):
            for size in (datasize1[i],datasize2[i]):
                if type=='prior':
                    conf_dict=(0.5,0.6,0.7,0.8,0.9,0.99999)
                elif type=='prior_allright' or 'noprior':
                    #默认可信度设置
                    conf_dict=[0.99999]
                for conf in conf_dict:
                    path=f'CaMML_{dataset}_{size}_{j}_{conf}.txt'
                    dag = np.loadtxt(f"{result_dag_dir}/{type}/{path}", dtype=int)
                    metric={"accept":0,"reject":0,"TP":0,"TN":0,"FP":0,"FN":0,"TPR":0,"TNR":0,"FPR":0,"FNR":0}
                    for cause in cause_list:
                        if check_path(dag,cause[0],cause[1])==1:
                            metric["accept"]+=1
                        else:
                            metric["reject"]+=1
                            
                    for cause in cause_right:
                        if check_path(dag,cause[0],cause[1])==1:
                            metric["TP"]+=1
                        else:
                            metric["FN"]+=1
                    for cause in cause_wrong:
                        if check_path(dag,cause[0],cause[1])==1:
                            metric["FP"]+=1
                        else:
                            metric["TN"]+=1
                    
                    if (metric['TP']+metric['FN'])==0:
                        metric["TPR"]=np.nan
                    else:
                        metric["TPR"]=metric["TP"]/(metric["TP"]+metric["FN"])
                    if (metric["FP"]+metric["TN"])==0:
                        metric["FPR"]=np.nan
                        metric["TNR"]=np.nan        
                    else:
                        metric["FPR"]=metric["FP"]/(metric["FP"]+metric["TN"])
                        metric["TNR"]=1-metric["FPR"]
                    
                    if (metric['TP']+metric['FP'])==0:
                        metric['precision']=np.nan    
                    else:
                        metric['precision']=metric['TP']/(metric['TP']+metric['FP'])
                    
                    if (metric['TP']+metric['FN'])==0:
                        metric['recall']=0
                    else:
                        metric['recall']=metric['TP']/(metric['TP']+metric['FN'])
                    
                    try:
                        metric['F1']=2*metric['precision']*metric['recall']/(metric['recall']+metric['precision'])
                    except:
                        metric['F1']=np.nan
                    with open(output,'a') as f:
                        f.write(f"{dataset} {size} {j} {conf}\n{metric}\n")
                
                
                        
prior_accept_evaluation(type='prior',output="exp/prior_accept_evaluation.txt")
prior_accept_evaluation(type='prior_allright',output="exp/prior_accept_evaluation_allright.txt")           
prior_accept_evaluation(type='noprior',output="exp/prior_accept_evaluation_noprior.txt")