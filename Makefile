#Dependencies
OBJS = main.c

#Set compiler
CC = gcc

#Set compilation options
#w supress all warnings
COMPILER_FLAGS = -w

#Set libraries to link against
LINKER_FLAGS = -lSDL2

#Excecutable
OBJ_NAME = game 

#Command to create target must begin with tab.
all: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

