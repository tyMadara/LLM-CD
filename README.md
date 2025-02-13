
## Integrating Large Language Model for Improved Causal Discovery

The code and materials of the research of Integrating Large Language Model for Improved Causal Discovery.

## Project Structure

- `BI-CaMML`/: MCMC search algorithm; refer to `install.sh` for installation details. Requires a JRE environment for execution.
- `minobsx/`: Tabu search algorithm
- `BN_structure/`: Contains the true Bayesian networks used in the experiments.
- `data/`: Hold the utilized data in csv formats, and note that due to the limitation of upload size, we only keep data of Asia, Cancer and Child for test.
- `exp/`: Store the output and statistics from the experiments. The key results are illustrated as follows:
  - `CaMML_record/`: Estimated DAG generated using CaMML as the backbone algorithm.
  - `MINOBSx_record/`: Estimated DAG generated using MINOBSx as the backbone algorithm.
  - `LLM_perform/`: Accuracy of prior ancestral constraints derived from various LLMs on various datasets, corresponding section IV-D.
  - `real_data/: `Experiments with real-world data, corresponding section IV-G.
  - Other statistics files, such as  `CaMML_prior_statistics.csv`, correspond to section IV-C, including statistics of relevant evaluation metrics of DAG and CPDAG.
- `LLM_query/`: Query results of different LLMs under different datasets.

## Explanation of the code

- `perform_CaMML.sh`: Call `CaMML_perform.py`, support Data-based and LLM-driven methods. (MCMC search)
- `perform_MINOBSx.sh`:  Call `MINOBSx_perform.py`, support Data-based and LLM-driven methods. (Tabu search)
- `prior_accept_evaluation.py`:  Accuracy of prior ancestral constraints derived from various LLMs on various datasets, corresponding section IV-D.
- `statistic.py`: Related statistics of SHD and F1 evaluation metrics, corresponding table III and IV.
- `SID_cal.py`: Statistics of SID evaluation metrics, corresponding table V.
- `draw_pic_pydot.py`: Results plotted on real-world data, corresponding Fig. 5.
- `utils.py`: Used tool functions.

## Installation

1. Install the required packages by `sh install.sh`
2. Use the command `sh perform_MINOBSx.sh` to execute the script. The parameters include the dataset name, dataset size and index, etc.
