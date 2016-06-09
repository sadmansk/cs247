CXX = g++
CXXFLAGS = -std=c++11 -g -Wall
OBJECTS = GraphTestHarness.o Graph.o Building.o BCode.o Collection.o
EXEC = GraphRobust

${EXEC} : $ ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

GraphTestHarness.o : GraphTestHarness.cpp Collection.o Graph.o
Graph.o : Graph.cpp Graph.h Building.o BaseException.o
Collection.o : Collection.cpp Collection.h Building.o BaseException.o
Building.o : Building.cpp Building.h BCode.o BaseException.o
BCode.o : BCode.h BCode.cpp BaseException.o
BaseException.o : BaseException.h

clean:
	rm -rf ${OBJECTS} ${EXEC}
