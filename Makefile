OBJS = src/*.cpp
CC = g++
COMPILER_FLAGS = -w -std=c++11
INCLUDE_PATH = -I./include/
OBJ_NAME = popcorn
all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATH) $(COMPILER_FLAGS) -o $(OBJ_NAME)
