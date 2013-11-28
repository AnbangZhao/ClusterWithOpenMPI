CXX  	    = $(CXX)
CXXFLAGS    = $(CXXFLAGS)
LINKFLAGS   = $(LINKFLAGS)
LIBFLAGS    = $(LIBFLAGS)
LIBDIR      = $(LIBDIR)

PROGRAM     = OpemMpiCluster

SRC         = algorithm.cpp  cluster.cpp  kmeans.cpp  master.cpp  pointBaseVec.cpp  slave.cpp stringHelper.cpp twoDPoints.cpp main.cpp
OBJ         = algorithm.o    cluster.o    kmeans.o    master.o    pointBaseVec.o    slave.o stringHelper.o twoDPoints.o main.o

.SUFFIXES:
.SUFFIXES: .o .c .cpp

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $*.cpp

all: $(PROGRAM)

$(PROGRAM): $(OBJ) 
	$(CXX) $(LINKFLAGS) $(LIBFLAGS) -o $(PROGRAM) $(OBJ) $(ASM)

clean:
	-rm -f *.o $(PROGRAM)


