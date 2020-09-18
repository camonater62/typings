CXX = g++
CXXFLAGS = -O2
LIBS = -ltermbox

default: typings

typings: typings.cpp 
	$(CXX) $(CXXFLAGS) typings.cpp -o typings $(LIBS)

clean:
	$(RM) typings