# paths
LIB = ../lib
INCLUDE = ../include
MODULES = ../modules

# compiler
CC = gcc

# Compile options. -I<dir> tells the compiler to search for include files there
CFLAGS = -Wall  -g -I$(INCLUDE)
LDFLAGS = -lm

# .o files

# To test the second implementation, uncomment the line below and comment the next one
OBJS = state_test.o $(MODULES)/state_alt.o $(MODULES)/set_utils/set_utils.o $(LIB)/k08.a $(LIB)/libraylib.a
#OBJS = state_test.o $(MODULES)/state.o $(LIB)/k08.a $(LIB)/libraylib.a
# Executable
EXEC = state_test

# Parameters for test run
ARGS =

# Parameters of the raylib library
include $(LIB)/libraylib.mk


all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LDFLAGS)

clean:
	rm -f $(OBJS) $(EXEC)

run: $(EXEC)
	./$(EXEC) $(ARGS)

# To build k08.a/libraylib.a run make in the lib directory.
$(LIB)/%.a:
	$(MAKE) -C $(LIB) $*.a