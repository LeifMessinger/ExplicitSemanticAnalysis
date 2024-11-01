pushd ./InvertedIndexMaker

alias python='winpty python.exe'
python -m venv .venv
source ./.venv/Scripts/activate
pip install -r requirements.txt

cd ..

python ./search.py ./data/InverseIndex.joblib ./data/DocTokenCounts.joblib

deactivate

popd

