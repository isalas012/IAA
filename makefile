make: ts.cpp
	g++ ts.cpp -o ts.out  -std=c++11

run: ts.out
	./ts.out ../instancias/abz5.txt
