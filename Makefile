CXX=g++
RM=rm -f

all: main

clean:
	$(RM) main

main: main.cpp
	$(CXX) main.cpp -o main --std=c++23 -O2

