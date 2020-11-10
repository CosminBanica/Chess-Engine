build:
	g++ -c -o main.o main.cpp
	g++ main.o tabla.h -std=c++11 -o motor
run:
	./motor

clean:
	rm motor main.o
