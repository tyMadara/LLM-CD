from gadjid import shd, sid, ancestor_aid
import os
import numpy as np
import csv
from myEva import MetricsDAG
import math
import networkx as nx
from causallearn.utils.DAG2CPDAG import dag2cpdag
from causallearn.graph.Dag import Dag

def dag_to_cpdag(dag):
    """
    Convert a DAG (a networkx.DiGraph) into its CPDAG (essential graph)
    by first computing its skeleton and then orienting v-structures.
    
    Note: This is a simplified conversion that does not implement all
    of Meek's rules. In a complete solution, additional edges might be
    compelled by Meek's orientation rules.
    """
    nodes = list(dag.nodes())
    
    # Step 1: Compute the skeleton (undirected graph)
    skeleton = nx.Graph()
    skeleton.add_nodes_from(nodes)
    for u, v in dag.edges():
        skeleton.add_edge(u, v)
    
    # Step 2: Initialize CPDAG: every edge in the skeleton is initially
    # represented as undirected. We represent an undirected edge by having
    # both directions in our DiGraph.
    cpdag = nx.DiGraph()
    cpdag.add_nodes_from(nodes)
    for u, v in skeleton.edges():
        cpdag.add_edge(u, v)
        cpdag.add_edge(v, u)
    
    # Step 3: Identify and orient v-structures.
    # A v-structure is any triple i -> j <- k (in the original DAG)
    # with no edge between i and k.
    for j in nodes:
        parents = list(dag.predecessors(j))
        for i in parents:
            for k in parents:
                if i < k:  # consider each unordered pair once
                    if not skeleton.has_edge(i, k):
                        # In a v-structure the directions i->j and k->j are compelled.
                        # So remove any reverse edges in our CPDAG.
                        if cpdag.has_edge(j, i):
                            cpdag.remove_edge(j, i)
                        if cpdag.has_edge(j, k):
                            cpdag.remove_edge(j, k)
    return cpdag

def compare_cpdag(cpdag1, cpdag2):
    """
    Compute the Structural Hamming Distance (SHD) between two CPDAGs.
    
    We compare every unordered pair of nodes. For a pair (a,b) we assign an
    "edge type" as follows:
      - 'none' if no edge is present,
      - 'undirected' if both a->b and b->a are in the graph,
      - 'directed' if only one of a->b or b->a is present.
      
    The SHD is then the number of pairs for which cpdag1 and cpdag2 disagree.
    """
    nodes = sorted(cpdag2.nodes())
    shd = 0
    for i in range(len(nodes)):
        for j in range(i + 1, len(nodes)):
            a, b = nodes[i], nodes[j]
            def edge_type(G, a, b):
                if G.has_edge(a, b) and G.has_edge(b, a):
                    return 'undirected'
                elif G.has_edge(a, b):
                    return 'directed'  # we interpret a->b as directed
                elif G.has_edge(b, a):
                    return 'directed'  # or b->a as directed
                else:
                    return 'none'
            if edge_type(cpdag1, a, b) != edge_type(cpdag2, a, b):
                shd += 1
    return shd

def compare_cpdag2(cpdag1, cpdag2):

    nodes = sorted(cpdag2.nodes())
    TP = 0
    for i in range(len(nodes)):
        for j in range(i + 1, len(nodes)):
            a, b = nodes[i], nodes[j]
            if cpdag1.has_edge(a, b) and cpdag2.has_edge(a, b):
                # undirected
                if cpdag1.has_edge(b, a) and cpdag2.has_edge(b, a):
                    TP += 1
                # directed
                elif (not cpdag1.has_edge(b, a)) and (not cpdag2.has_edge(b, a)):
                    TP += 1
            elif cpdag1.has_edge(b, a) and cpdag2.has_edge(b, a):
                if (not cpdag1.has_edge(a, b)) and (not cpdag2.has_edge(a, b)):
                    TP += 1
    cpdag_edge = 0
    for i in range(len(nodes)):
        for j in range(i + 1, len(nodes)):
            a, b = nodes[i], nodes[j]
            if cpdag1.has_edge(a, b) and not cpdag1.has_edge(b, a):
                cpdag_edge += 1
            elif not cpdag1.has_edge(a, b) and cpdag1.has_edge(b, a):
                cpdag_edge += 1
            elif cpdag1.has_edge(a, b) and cpdag1.has_edge(b, a):
                cpdag_edge += 1
    true_cpdag_edge = 0
    for i in range(len(nodes)):
        for j in range(i + 1, len(nodes)):
            a, b = nodes[i], nodes[j]
            if cpdag2.has_edge(a, b) and not cpdag2.has_edge(b, a):
                true_cpdag_edge += 1
            elif not cpdag2.has_edge(a, b) and cpdag2.has_edge(b, a):
                true_cpdag_edge += 1
            elif cpdag2.has_edge(a, b) and cpdag2.has_edge(b, a):
                true_cpdag_edge += 1

    P = TP / cpdag_edge if cpdag_edge > 0 else 0
    R = TP / true_cpdag_edge if true_cpdag_edge > 0 else 0
    F1 = (2 * P * R) / (P + R) if (P + R) > 0 else 0
    return F1


method_list = ['CaMML', 'MINOBSx']
prior_type = ['noprior', 'prior']
dataset_list_upper = ['Cancer','Asia','Child','Insurance','Alarm','Water','Mildew','Barley']
dataset_list_lower = ['cancer','asia','child','insurance','alarm','water','mildew','barley']
dataset2size = [{"asia": 250, "child": 500, "insurance": 500, "alarm": 1000, "cancer": 250, "mildew": 8000, "water": 1000, "barley": 2000},
                        {"asia": 1000, "child": 2000, "insurance": 2000, "alarm": 4000, "cancer": 1000, "mildew": 32000, "water": 4000, "barley": 8000}]
datasize_index_list = [0,1]

dir_path = 'exp/'
true_dag_dir = 'BN_structure/'

def read_adjacency_matrices(folder_path, method):
    
    mean_value_list = []
    for dataset_ in dataset_list_lower:
        true_dag_path = true_dag_dir + dataset_ + '_graph.txt'
        with open(true_dag_path, 'r') as file:
            matrix_true_dag = np.loadtxt(file, dtype=np.int8) 
            # G1 = nx.from_numpy_array(matrix_true_dag)
            # cpdag_true = dag_to_cpdag(G1)
            G1 = nx.from_numpy_array(matrix_true_dag)
            true_dag = nx.DiGraph()
            true_dag.add_edges_from(list(G1.edges()))
            cpdag_true = dag_to_cpdag(true_dag)
            
        for size in datasize_index_list:
            filtered_files = []
            for filename in os.listdir(folder_path):
                parts = filename.split('_')
                
                if method == 'MINOBSx':
                    if len(parts) == 4:
                        _, dataset, datasize, index_with_extension = parts
                        index = index_with_extension.split('.')[0]
                        if dataset == dataset_ and int(datasize) == dataset2size[size][dataset_]:
                            filtered_files.append(filename)
                elif method == 'CaMML':
                    if len(parts) == 5:
                        _, dataset, datasize, index, confidence_with_extension = parts
                        confidence = confidence_with_extension.split('.t')[0]
                        if dataset == dataset_ and int(datasize) == dataset2size[size][dataset_] and confidence == '0.99999':
                            filtered_files.append(filename)
                        
            print(filtered_files)
            
            SHD_list = []
            for filename in filtered_files:
                file_path = os.path.join(folder_path, filename)
                if os.path.isfile(file_path):
                    with open(file_path, 'r') as file:
                        matrix = np.loadtxt(file, dtype=np.int8) 
                        
                        # G = nx.from_numpy_array(matrix)
                        # cpdag_est = dag_to_cpdag(G)
                        
                        G2 = nx.from_numpy_array(matrix)
                        est_dag = nx.DiGraph()
                        est_dag.add_edges_from(list(G2.edges()))
                        cpdag_est = dag_to_cpdag(est_dag)
                        
                        # SHD = compare_cpdag(cpdag_est, cpdag_true)
                        SHD = compare_cpdag2(cpdag_est, cpdag_true)
                        
                        m = MetricsDAG(matrix, matrix_true_dag)
                        # if math.isnan(m.metrics['F1']):
                        #     continue
                        # else:
                        #     SHD_list.append(SHD)
                        if SHD == 0:
                            continue
                        else:
                            SHD_list.append(SHD)
                        # SHD_list.append(SHD)
            mean_value = np.mean(SHD_list)
            mean_value_list.append(mean_value)
            
    return mean_value_list

output_dir = 'exp/F1_CPDAG_statistics.csv'
with open(output_dir, mode="w", newline="", encoding="utf-8") as file:
    writer = csv.writer(file)
    for method in method_list:
        former_list = []
        last_list = []
        for prior in prior_type:
            folder_path = dir_path + method + '_record/' + prior
            mean_value_list = read_adjacency_matrices(folder_path, method)
            if prior == 'noprior':
                former_list = mean_value_list
            elif prior == 'prior':
                last_list = mean_value_list
            formatted_data = [f"{x:.2f}" for x in mean_value_list]
            writer.writerow(formatted_data)
        imp_list = []
        for i in range(0,len(former_list)):
            improvement = (last_list[i] - former_list[i]) / former_list[i]
            formatted_value = f"{improvement * 100:.1f}%"
            imp_list.append(formatted_value)
        writer.writerow(imp_list)
        