CXX=clang++
CXXFLAGS=-Wall -O2 -std=c++11
SRC=main.cpp


all:
	$(CXX) $(CXXFLAGS) -DNUMT=16 -DNUM_TRIES=100 -fopenmp -o project0-multi $(SRC)
	$(CXX) $(CXXFLAGS) -DNUMT=1 -DNUM_TRIES=100 -fopenmp -o project0-single $(SRC)
gcc:
	g++ $(CXXFLAGS) -DMESSAGE='"Compiled with GCC"' $(LIBS) -o qsnake $(SRC)

clean:
	@rm -f snake qsnake
	@rm -f *.o
format:
	clang-format --verbose --sort-includes -i *.cpp *.hpp

