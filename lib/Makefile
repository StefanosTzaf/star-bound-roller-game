# Makefile for the k08.a and libraylib.a libraries

# Detect which operating system is being used
ifdef WASM
	SUFFIX = wasm
else ifeq ($(OS),Windows_NT)
	SUFFIX = win
else ifeq ($(shell uname -s),Linux)
	SUFFIX = linux
else
	SUFFIX = macos
endif

all: k08.a libraylib.a

# "Build" k08.a by simply copying the file that matches the current operating system
k08.a:
	cp k08_$(SUFFIX).a k08.a

# Same for libraylib.a
libraylib.a:
	cp libraylib_$(SUFFIX).a libraylib.a

clean:
	$(RM) k08.a libraylib.a