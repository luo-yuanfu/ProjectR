#### directories
INCDIR = include
SRCDIR = src
BINDIR = bin

#### opencv 
CFLAGS = `pkg-config --cflags opencv` -pg
LIBS = `pkg-config --libs opencv`
####

#### compiler/linker options
INCL = -I $(INCDIR)
CXX = g++

### files
SOURCES = $(shell find -L $(SRCDIR) -name '*.cc')
PROG = $(BINDIR)/random_forest_regressor

### target
.PHONY: all clean

all: directory $(PROG)

directory:
	mkdir -p $(BINDIR)

$(PROG): directory
	$(CXX) $(SOURCES) $(INCL) -o $(PROG) $(CFLAGS) $(LIBS)

clean:
	rm -rf $(BINDIR)


