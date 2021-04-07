lab1: main.cpp lab1.cpp common.h
	g++ $^ -lminisat -o $@

run: lab1
	./lab1

example: minisat.cpp
	g++ $^ -lminisat -o $@
	./$@

clean:
	rm -rf lab1
	rm -rf example
	rm -rf answer.txt
