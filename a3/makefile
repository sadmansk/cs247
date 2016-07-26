CXXFLAGS= -O -Wall -std=c++11
LDFLAGS =
CXX=g++
OBJECTS = Iterator.o MenuComponent.o MenuItem.o Menu.o IteratorTestHarness.o 
DEPENDS = ${OBJECTS:.o=.d}
EXEC=menu

$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(OBJECTS) $(LDFLAGS)


clean:
	rm -f *.o $(EXEC) $(TESTS) $(DEPENDS)

-include ${DEPENDS}

