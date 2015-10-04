#### directories
INCDIR = include
SRCDIR = src
BINDIR = bin
LIBDIR = /opt/local/lib

#### compiler/linker options
INCL = -I $(INCDIR) 

INCL2 = -I/opt/local/include \
        -I/opt/local/include/opencv \
        -I/opt/local/include/opencv2/core

LIB = -L$(LIBDIR) -lopencv_core -lopencv_highgui -lopencv_imgproc

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
	$(CXX) $(INCL2) $(SOURCES) $(INCL) -o $(PROG) $(LIB)

clean:
	rm -rf $(BINDIR)


