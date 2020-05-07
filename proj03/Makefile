CXX=g++
CXXFLAGS=-Wall -O2 -std=c++11
SRC=main.cpp


all:
	$(CXX) $(CXXFLAGS) -fopenmp -o project2 $(SRC)
format:
	clang-format --verbose --sort-includes -i *.cpp *.hpp

