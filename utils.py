import os
import re
import pandas as pd
import json
import numpy as np

def save_file_additionally(dir_path, prefix_file_name, file_type=""):
    files = os.listdir(dir_path)
    m = rf"{prefix_file_name}(_\d+)?\.{file_type}"
    res_indexes = [0]
    exist = False
    for file in files:
        r = re.match(m, file)
        if r:
            exist = True
        if r and r.group(1):
            res_indexes.append(int(r.group(1)[1:]))
    if not exist:
        new_file_name = f"{prefix_file_name}.{file_type}"
    else:
        max_index = max(res_indexes)
        new_file_name = f"{prefix_file_name}_{max_index+1}.{file_type}"
    return os.path.join(dir_path, new_file_name)


def parse_experiment_results(file_name):
    with open(file_name, "r") as f:
        lines = f.readlines()
    res = pd.DataFrame(columns=["data", "size", "forb_anc_num", "index", "repeat", "order_mode"])
    props =["data", "size", "forb_anc_num", "index", "repeat", "order_mode"]
    for p in ["size", "forb_anc_num", "index", "repeat"]:
        res[p] = res[p].astype("int")
    tmp_record = {}
    for line in lines:
        line = line.strip()
        if line.startswith("{"):
            line = line.replace("'", '"')
            line = line.replace("nan", 'null')
            tmp = json.loads(line)
            # add to the pd.dataframe res
            for key in tmp:
                if key not in res.columns:
                    res[key] = None
                tmp_record[key] = float(tmp[key]) if tmp[key] is not None else "null"
            res = res.append(tmp_record, ignore_index=True)
            tmp_record = {}
        else:
            v_list = line.split(" ")
            for i, k in enumerate(props):
                if k in ["size","forb_anc_num","index","repeat"]:
                    tmp_record[k] = int(v_list[i])
                else:
                    tmp_record[k] = v_list[i]
    return res

def parse_experiment_results_perform(file_name):
    with open(file_name, "r") as f:
        lines = f.readlines()
    res = pd.DataFrame(columns=["data", "size", "index"])
    props =["data", "size", "index"]
    for p in ["size", "index"]:
        res[p] = res[p].astype("int")
    tmp_record = {}
    for line in lines:
        line = line.strip()
        if line.startswith("{"):
            line = line.replace("'", '"')
            line = line.replace("nan", 'null')
            tmp = json.loads(line)
            # add to the pd.dataframe res
            for key in tmp:
                if key not in res.columns:
                    res[key] = None
                tmp_record[key] = float(tmp[key]) if tmp[key] is not None else np.nan
            res = res.append(tmp_record, ignore_index=True)
            tmp_record = {}
        else:
            v_list = line.split(" ")
            for i, k in enumerate(props):
                if k in ["size","index"]:
                    tmp_record[k] = int(v_list[i])
                else:
                    tmp_record[k] = v_list[i]
    return res

def parse_experiment_results_perform_withconf(file_name):
    with open(file_name, "r") as f:
        lines = f.readlines()
    res = pd.DataFrame(columns=["data", "size", "index","conf"])
    props =["data", "size", "index","conf"]
    for p in ["size", "index"]:
        res[p] = res[p].astype("int")
    res["conf"] = res["conf"].astype("float")
    tmp_record = {}
    for line in lines:
        line = line.strip()
        if line.startswith("{"):
            line = line.replace("'", '"')
            line = line.replace("nan", 'null')
            tmp = json.loads(line)
            # add to the pd.dataframe res
            for key in tmp:
                if key not in res.columns:
                    res[key] = None
                tmp_record[key] = float(tmp[key]) if tmp[key] is not None else np.nan
            res = res.append(tmp_record, ignore_index=True)
            tmp_record = {}
        else:
            v_list = line.split(" ")
            for i, k in enumerate(props):
                if k in ["size","index"]:
                    tmp_record[k] = int(v_list[i])
                elif k=="conf":
                    tmp_record[k] = float(v_list[i])
                else:
                    tmp_record[k] = v_list[i]
    return res

def parse_prior_results(file_name='exp/path_prior_evaluation.txt'):
    import pandas as pd
    import json
    with open(file_name, "r") as f:
        lines = f.readlines()
    res = pd.DataFrame(columns=["data"])
    props =["data"]
    tmp_record = {}
    for line in lines:
        line = line.strip()
        if line.startswith("{"):
            line = line.replace("'", '"')
            line = line.replace("nan", 'null')
            tmp = json.loads(line)
            # add to the pd.dataframe res
            for key in tmp:
                if key not in res.columns:
                    res[key] = None
                tmp_record[key] = float(tmp[key]) if tmp[key] is not None else np.nan
            res = res.append(tmp_record, ignore_index=True)
            tmp_record = {}
        else:
            v_list = line.split(" ")
            for i, k in enumerate(props):
                tmp_record[k] = v_list[i]
    return res




