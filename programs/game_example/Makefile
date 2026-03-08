# paths
LIB = ../../lib
INCLUDE = ../../include

# compiler
CC = gcc

# Compile options. -I<dir> tells the compiler to search for include files there
CFLAGS = -Wall -Werror -g -I$(INCLUDE)
LDFLAGS = -lm

# .o files
OBJS = game_example.o state.o interface.o $(LIB)/k08.a $(LIB)/libraylib.a

# Executable
EXEC = game_example

# Parameters for test run
ARGS =

# Parameters of the raylib library
include $(LIB)/libraylib.mk


$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LDFLAGS)

# To build k08.a/libraylib.a run make in the lib directory.
$(LIB)/%.a:
	$(MAKE) -C $(LIB) $*.a

clean:
	rm -f $(OBJS) $(EXEC)

run: $(EXEC)
	export DISPLAY=$$(ip route list default | awk '{print $$3}'):0 && \
	export LIBGL_ALWAYS_INDIRECT=1 && \
	./$(EXEC) $(ARGS)

valgrind: $(EXEC)
	valgrind --error-exitcode=1 --leak-check=full --show-leak-kinds=all ./$(EXEC) $(ARGS)