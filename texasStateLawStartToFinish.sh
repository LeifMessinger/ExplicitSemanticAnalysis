if [ ! -f "./BagOfWordsMaker/BagOfWordsMaker.exe" ]; then
	pushd ./BagOfWordsMaker
	echo "Compiling..."
	./compile.sh
	echo "Done compiling."
	popd
fi

if [ ! -d "./TexasStateLaw" ]; then
	mkdir ./TexasStateLaw
	pushd ./TexasStateLaw

	curl -O https://statutes.capitol.texas.gov/Docs/Zips/CN.htm.zip && unzip CN.htm.zip -d "The Texas Constitution"
	curl -O https://statutes.capitol.texas.gov/Docs/Zips/AG.htm.zip && unzip AG.htm.zip -d "Agriculture Code"
	curl -O https://statutes.capitol.texas.gov/Docs/Zips/AL.htm.zip && unzip AL.htm.zip -d "Alcoholic Beverage Code"
	curl -O https://statutes.capitol.texas.gov/Docs/Zips/WL.htm.zip && unzip WL.htm.zip -d "Auxiliary Water Laws"
	curl -O https://statutes.capitol.texas.gov/Docs/Zips/BC.htm.zip && unzip BC.htm.zip -d "Business and Commerce Code"
	curl -O https://statutes.capitol.texas.gov/Docs/Zips/BO.htm.zip && unzip BO.htm.zip -d "Business Organizations Code"
	curl -O https://statutes.capitol.texas.gov/Docs/Zips/CP.htm.zip && unzip CP.htm.zip -d "Civil Practice and Remedies Code"
	curl -O https://statutes.capitol.texas.gov/Docs/Zips/CR.htm.zip && unzip CR.htm.zip -d "Code of Criminal Procedure"
	curl -O https://statutes.capitol.texas.gov/Docs/Zips/ED.htm.zip && unzip ED.htm.zip -d "Education Code"
	curl -O https://statutes.capitol.texas.gov/Docs/Zips/EL.htm.zip && unzip EL.htm.zip -d "Election Code"
	curl -O https://statutes.capitol.texas.gov/Docs/Zips/ES.htm.zip && unzip ES.htm.zip -d "Estates Code"
	curl -O https://statutes.capitol.texas.gov/Docs/Zips/FA.htm.zip && unzip FA.htm.zip -d "Family Code"
	curl -O https://statutes.capitol.texas.gov/Docs/Zips/FI.htm.zip && unzip FI.htm.zip -d "Finance Code"
	curl -O https://statutes.capitol.texas.gov/Docs/Zips/GV.htm.zip && unzip GV.htm.zip -d "Government Code"
	curl -O https://statutes.capitol.texas.gov/Docs/Zips/HS.htm.zip && unzip HS.htm.zip -d "Health and Safety Code"
	curl -O https://statutes.capitol.texas.gov/Docs/Zips/HR.htm.zip && unzip HR.htm.zip -d "Human Resources Code"
	curl -O https://statutes.capitol.texas.gov/Docs/Zips/IN.htm.zip && unzip IN.htm.zip -d "Insurance Code"
	curl -O https://statutes.capitol.texas.gov/Docs/Zips/I1.htm.zip && unzip I1.htm.zip -d "Insurance Code - Not Codified"
	curl -O https://statutes.capitol.texas.gov/Docs/Zips/LA.htm.zip && unzip LA.htm.zip -d "Labor Code"
	curl -O https://statutes.capitol.texas.gov/Docs/Zips/LG.htm.zip && unzip LG.htm.zip -d "Local Government Code"
	curl -O https://statutes.capitol.texas.gov/Docs/Zips/NR.htm.zip && unzip NR.htm.zip -d "Natural Resources Code"
	curl -O https://statutes.capitol.texas.gov/Docs/Zips/OC.htm.zip && unzip OC.htm.zip -d "Occupations Code"
	curl -O https://statutes.capitol.texas.gov/Docs/Zips/PW.htm.zip && unzip PW.htm.zip -d "Parks and Wildlife Code"
	curl -O https://statutes.capitol.texas.gov/Docs/Zips/PE.htm.zip && unzip PE.htm.zip -d "Penal Code"
	curl -O https://statutes.capitol.texas.gov/Docs/Zips/PR.htm.zip && unzip PR.htm.zip -d "Property Code"
	curl -O https://statutes.capitol.texas.gov/Docs/Zips/SD.htm.zip && unzip SD.htm.zip -d "Special District Local Laws Code"
	curl -O https://statutes.capitol.texas.gov/Docs/Zips/TX.htm.zip && unzip TX.htm.zip -d "Tax Code"
	curl -O https://statutes.capitol.texas.gov/Docs/Zips/TN.htm.zip && unzip TN.htm.zip -d "Transportation Code"
	curl -O https://statutes.capitol.texas.gov/Docs/Zips/UT.htm.zip && unzip UT.htm.zip -d "Utilities Code"
	curl -O https://statutes.capitol.texas.gov/Docs/Zips/WA.htm.zip && unzip WA.htm.zip -d "Water Code"
	curl -O https://statutes.capitol.texas.gov/Docs/Zips/CV.htm.zip && unzip CV.htm.zip -d "Vernon's Civil Statutes"
	rm *.zip
	
	popd
fi

echo "Making bag of words..."
time ./BagOfWordsMaker/runBagOfWords.sh ./TexasStateLaw/*
echo "Bag of words is done."

pushd ./InvertedIndexMaker

alias python='winpty python.exe'
python -m venv .venv
source ./.venv/Scripts/activate
pip install -r requirements.txt

echo "Making inverse index..."
time python ./bagOfWordsToInverseIndex.py ../data/bagOfWords.txt
echo "Inverse index done."

cd ..

python ./search.py ./data/InverseIndex.joblib ./data/DocTokenCounts.joblib

deactivate

popd

