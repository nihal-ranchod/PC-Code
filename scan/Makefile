CXX = mpic++
RM = rm -f
CXXFLAGS=-g -std=c++20 -fopenmp

SRCS = main.cpp scan_omp.cpp scan.cpp
OBJS = $(subst .cpp,.o, $(SRCS))

all: scan

scan: $(OBJS)
    $(CXX) $(CXXFLAGS) -o scan $(OBJS) 

main.o: main.cpp
scan_omp.o: scan_omp.cpp
scan.o: scan.cpp
#util.o: util.cpp

clean:
    $(RM) $(OBJS)

distclean: clean
    $(RM) scan