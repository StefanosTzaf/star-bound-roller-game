# paths
LIB = ../../lib
INCLUDE = ../../include

# compiler
CC = gcc

# Compile options. -I<dir> tells the compiler to search for include files there
CFLAGS = -Wall  -g -I$(INCLUDE)
LDFLAGS = -lm

# .o files
OBJS = set_utils_test.o set_utils.o $(LIB)/k08.a $(LIB)/libraylib.a

# Executable
EXEC = set_utils

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

valgrind: $(EXEC)
	valgrind --error-exitcode=1 --leak-check=full --show-leak-kinds=all ./$(EXEC) $(ARGS)
	
# To build k08.a/libraylib.a run make in the lib directory.
$(LIB)/%.a:
	$(MAKE) -C $(LIB) $*.a