# ExplicitSemanticAnalysis

### Needed things:
- Windows (Some things need it. Others don't)
- Git Bash
- Python
- g++
- Probably something else

## How to run
Clone this repo

Git bash into the folder.
Run `./texasStateLawStartToFinish.sh`

It'll take a couple minutes to:
- Compile the BagOfWordsMaker (the html/xml scraper) ~7 seconds
- Download and unzip the Texas State Law HTML files from https://statutes.capitol.texas.gov/Download.aspx ~2 minutes
- Create the bag of words ~10 seconds at best
- Create the python virtual environment ~10 seconds
- Pip install `joblib` and `numpy` into the venv. ~10 seconds
- Create the inverse index from the bag of words ~7 seconds at best
- Run the `search.py` (probably takes ~5 seconds to load the inverse index)

Searching things is near instantaneous after that. Haven't really done any performance benchmarking there.

### Background
I chose the Texas State Law database because I wanted to program on something while the entirety of wikipedia was downloading and extracting. I'll try to run this set of programs on Wikipedia later. To get the links, I could have made a script that gets the links from the download page, but it was easier to make a script to make bash commands to download those links and extract them. I don't think those links are going to break in the next month or two.

I used C++ at the first part because in theory it's faster. I can always go in and optimize it further, but it's not heat-death-of-the-universe slow, so it's good enough for now. The Texas State Law 

I used a bash script to call that C++ program a bunch of times. The bash script works on the folders provided as arguments in parallel, and the C++ program scrapes the xml files in the folder in parallel. If you don't have 8+ cores, this might be a lot slower for you.

I used python to turn the output of the C++ program (which is just histograms of every document) into an inverse index, which is dict(token -> Counter(document -> count)). Then I save that file and pass it into ./search.py which does the TF-IDF and cosine similarity math.
