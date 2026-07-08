# makefile to build the whole thing

CPP=main.cpp simulation.cpp group.cpp individual.cpp 
HPP=simulation.hpp individual.hpp group.hpp parameters.hpp 
EXE=group_foraging_ibm.exe
CXX=g++
CXXFLAGS=-Wall -Wconversion -std=c++20 -O3 

$(EXE) : $(CPP) $(HPP)
	$(CXX) $(CXXFLAGS) -o $(EXE) $(CPP)

clean :
	rm -rf $(EXE)
