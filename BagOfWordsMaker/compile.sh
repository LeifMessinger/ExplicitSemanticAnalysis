if [[ "$OSTYPE" == "linux-gnu" ]]; then
	g++ -O3 -std=c++17 *.cpp -o BagOfWordsMaker.exe -lpthread
else
	g++ -O3 -std=c++17 *.cpp -o BagOfWordsMaker.exe
fi
