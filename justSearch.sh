pushd ./InvertedIndexMaker

alias python='winpty python.exe'
python -m venv .venv
if [ -d ./.venv/Scripts ]; then
	source ./.venv/Scripts/activate
else
	source ./.venv/bin/activate
fi
pip install -r requirements.txt

cd ..

python ./search.py ./data/InverseIndex.joblib ./data/DocTokenCounts.joblib

deactivate

popd

