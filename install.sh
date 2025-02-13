# initialize the python environment
pip install --upgrade pip
pip install -r requirements.txt

# download and setup causal-learn
git clone https://github.com/py-why/causal-learn.git
mv causal-learn/causallearn .
rm -rf causal-learn
cp scripts/ExactSearch.py causallearn/search/ScoreBased/ExactSearch.py

# download and setup minobsx
git clone https://github.com/andrewli77/MINOBS-anc.git
mv MINOBS-anc minobsx
cp scripts/run-one-case-my.sh minobsx/run-one-case-my.sh
cp scripts/run-one-case-PG.sh minobsx/run-one-case-PG.sh
cd minobsx
mkdir anc_file out_BNs PGminobsx PGminobsx/parent_score PGminobsx/out_BNs PGminobsx/prior
cd ../