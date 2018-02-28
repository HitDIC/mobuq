CC=g++

CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(notdir $(CPP_FILES:.cpp=.o))
CC_FLAGS := -O3 -std=c++11 -static
LD_FLAGS := -L.
all: mcmc

mcmc: mcmc.cpp
	$(CC) $(CC_FLAGS) mcmc.cpp -o $@  $(LD_FLAGS)

clean:
	rm test