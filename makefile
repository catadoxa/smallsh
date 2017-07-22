#SOURCES and EXECUTABLE should be the only variables you need to change for most programs

EXECUTABLE=smallsh
SOURCES=smallsh.c
SOURCES+=build_command.c
SOURCES+=run_command.c
SOURCES+=helper_funcs.c

OBJECTS=$(SOURCES:.c=.o)

CXX=gcc
CXXFLAGS=-c -Wall

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(EXECUTABLE)
	
$(OBJECTS): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES)
	
clean:
	rm *o $(EXECUTABLE)
