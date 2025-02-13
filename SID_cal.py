
from gadjid import shd, sid, ancestor_aid
import os
import numpy as np
import csv
from myEva import MetricsDAG
import math

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
            
            SID_list = []
            for filename in filtered_files:
                file_path = os.path.join(folder_path, filename)
                if os.path.isfile(file_path):
                    with open(file_path, 'r') as file:
                        matrix = np.loadtxt(file, dtype=np.int8) 
                        
                        SID = sid(matrix_true_dag, matrix, edge_direction="from row to column")
                        m = MetricsDAG(matrix, matrix_true_dag)
                        if math.isnan(m.metrics['F1']):
                            continue
                        else:
                            SID_list.append(SID[1])
            mean_value = np.mean(SID_list)
            mean_value_list.append(mean_value)
            
    return mean_value_list

output_dir = 'exp/SID_statistics.csv'
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
            formatted_value = f"{improvement * 100:.2f}%"
            imp_list.append(formatted_value)
        writer.writerow(imp_list)