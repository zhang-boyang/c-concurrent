
out: coroutine.o example.o
	g++ -g -o  out example.o coroutine.o
coroutine.o: coroutine.cpp coroutine.h
	g++ -c -g -o coroutine.o coroutine.cpp --std=c++11
example.o: example.cpp coroutine.h
	g++ -c -g -o example.o example.cpp --std=c++11

clean:
	rm -f out coroutine.o example.o

