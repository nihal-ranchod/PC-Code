CXX = mpic++
RM = rm -f
CXXFLAGS=-g -std=c++20 -fopenmp

SRCS = main.cpp sssp_omp.cpp sssp.cpp util.cpp
OBJS = $(subst .cpp,.o, $(SRCS))

all: sssp

sssp: $(OBJS)
    $(CXX) $(CXXFLAGS) -o sssp $(OBJS) 

main.o: main.cpp
sssp_omp.o: sssp_omp.cpp
sssp.o: sssp.cpp
#util.o: util.cpp

clean:
    $(RM) $(OBJS)

distclean: clean
    $(RM) sssp