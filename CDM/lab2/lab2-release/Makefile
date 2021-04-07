all: lexical.l syntax.y multree.cpp cfg.cpp minisee.cpp lab2.cpp
	flex lexical.l
	bison -d syntax.y
	g++ -g syntax.tab.c multree.cpp cfg.cpp minisee.cpp lab2.cpp -lfl -ly -lz3 -o minisee

run: all
	./minisee test/test1.c > test/output.txt
	./minisee test/test2.c >> test/output.txt
	./minisee test/test3.c >> test/output.txt
	./minisee test/test4.c >> test/output.txt
	./minisee test/test5.c >> test/output.txt
	./minisee test/test6.c >> test/output.txt
	./minisee test/test7.c >> test/output.txt
	./minisee test/test8.c >> test/output.txt
	./minisee test/test9.c >> test/output.txt
	./minisee test/test10.c >> test/output.txt

clean:
	rm -f lex.yy.c syntax.tab.c syntax.tab.h minisee
	rm -f test/output.txt