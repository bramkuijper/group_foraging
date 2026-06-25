# makefile to build the whole thing

CPP=main.cpp simulation.cpp group.cpp individual.cpp 
HPP=simulation.hpp individual.hpp group.hpp parameters.hpp 
EXE=main.exe
CXX=g++
CXXFLAGS=-Wall -Wconversion -O3 -std=c++20

$(EXE) : $(CPP) $(HPP)
	$(CXX) $(CXXFLAGS) -o $(EXE) $(CPP)

clear :
	rm -rf $(EXE)
