from utils import *

def noprior_dag_metric(input="exp/CaMML_noprior.txt",output="exp/CaMML_noprior_statistics.csv"):
    res = parse_experiment_results_perform(input)
    res=res.dropna()

    # ignore warnings
    import warnings
    warnings.filterwarnings("ignore")
    d_res = res.groupby(["data","size"]).mean()
    d_res[["missing","extra","reverse","shd","precision","recall","F1","statisfy","time"]].to_csv(output,float_format="%.2f")


def prior_dag_metric(input,output):
    res = parse_experiment_results_perform_withconf(input)
    res=res.dropna()

    # ignore warnings
    import warnings
    warnings.filterwarnings("ignore")
    d_res = res.groupby(["data","size","conf"]).mean()
    d_res[["missing","extra","reverse","shd","precision","recall","F1","statisfy","time"]].to_csv(output,float_format="%.2f")

def prior_accept(input,output):
    res = parse_experiment_results_perform_withconf(input)

    # ignore warnings
    import warnings
    warnings.filterwarnings("ignore")
    d_res = res.groupby(["data","size","conf"]).mean()
    d_res[["accept","reject","TP","TN","FP","FN","TPR","TNR","precision","recall","F1"]].to_csv(output,float_format="%.2f")


def GPT_prior():
    res=parse_prior_results(file_name='exp/path_prior_evaluation.txt')
    res[["data","missing","extra","reverse","shd","precision","recall","F1","path_count"]].to_csv('exp/path_dag_evaluation.csv',float_format="%.2f")
    
# prior_dag_metric("exp/CaMML_prior_allright.txt","exp/CaMML_prior_allright_statistics.csv")
# prior_accept("exp/prior_accept_evaluation_allright.txt","exp/prior_accept_evaluation_allright_statistics.csv")
# prior_accept("exp/prior_accept_evaluation.txt","exp/prior_accept_evaluation_statistics.csv")
# prior_accept("exp/prior_accept_evaluation_noprior.txt","exp/prior_accept_evaluation_noprior_statistics.csv")
# noprior_dag_metric(input="exp/HC_noprior.txt",output="exp/HC_noprior_statistics.csv")

noprior_dag_metric(input="exp_MB/CaMML_noprior.txt",output="exp_MB/CaMML_noprior_statistics.csv")
prior_dag_metric("exp_MB/CaMML_prior.txt","exp_MB/CaMML_prior_statistics.csv")