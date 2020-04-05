CXX=clang++
CXXFLAGS=-Wall -std=c++17
MERGESRC=mergesort.cpp
INSERTSRC=mergesort.cpp

all:
	$(CXX) $(CXXFLAGS) -o mergesort $(MERGESRC)
	$(CXX) $(CXXFLAGS) -o mergesort $(INSERTSRC)
gcc:
	g++ $(CXXFLAGS) -DMESSAGE='"Compiled with GCC"' $(LIBS) -o qsnake $(SRC)

clean:
	@rm -f snake qsnake
	@rm -f *.o
format:
	clang-format --verbose --sort-includes -i *.cpp *.hpp

