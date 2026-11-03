# makefile to build the whole thing

CPP=main.cpp simulation.cpp group.cpp individual.cpp 
HPP=simulation.hpp individual.hpp group.hpp parameters.hpp 
EXE=group_foraging_ibm.exe
EXE_DEBUG=group_foraging_ibm_dbg.exe
CXX=g++
CXXFLAGS=-Wall -Wconversion -std=c++20 -O3
CXXFLAGS_DEBUG=-Wall -Wconversion -std=c++20 -ggdb

all : $(EXE) $(EXE_DEBUG)

$(EXE) : $(CPP) $(HPP)
	$(CXX) $(CXXFLAGS) -o $(EXE) $(CPP)

$(EXE_DEBUG) : $(CPP) $(HPP)
	$(CXX) $(CXXFLAGS_DEBUG) -o $(EXE_DEBUG) $(CPP)
clean :
	rm -rf $(EXE)
	rm -rf $(EXE_DEBUG)
