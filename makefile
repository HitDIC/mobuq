CC=g++

CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(notdir $(CPP_FILES:.cpp=.o))
CC_FLAGS := -O3 -std=c++11 -I../../include -I../../library -static
LD_FLAGS := -L. -L../../lib -lhitdic -lfmt -lpthread --static
all: robot

robot: mcmc.cpp
	$(CC) $(CC_FLAGS) mcmc.cpp -o $@  $(LD_FLAGS)

clean:
	rm test