# This Makefile is located at the root of the entire project and is used to
# easily compile multiple parts of the project together. This Makefile calls
# make in the various directories as
#   $(MAKE) -C <dir> <target>
# which is equivalent to running make <target> inside the directory <foo>.

# All directories inside the programs directory
PROGRAMS = $(subst programs/, , $(wildcard programs/*))

# Compile: all, programs, tests
all: programs tests

# The following line creates a target programs-<foo> for any <foo>. The variable $* contains "foo"
programs-%:
	$(MAKE) -C programs/$*

programs: $(addprefix programs-, $(PROGRAMS))		# depend on programs-<foo> for each element of PROGRAMS

tests:
	$(MAKE) -C tests all

# Execution: all, programs, tests
run: run-tests run-programs

run-programs-%:
	$(MAKE) -C programs/$* run

run-programs: $(addprefix run-programs-, $(PROGRAMS))

run-tests:
	$(MAKE) -C tests run

# Cleanup
clean-programs-%:
	$(MAKE) -C programs/$* clean

clean: $(addprefix clean-programs-, $(PROGRAMS))
	$(MAKE) -C tests clean
	$(MAKE) -C lib clean

# Declare that the following rules are virtual, they do not produce files. That is,
# we want "make programs" to execute even though there is already a directory "programs".
#
.PHONY: programs tests lib run run-programs run-tests clean