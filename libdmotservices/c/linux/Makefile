#################################################
## libdmotservices/c/linux/Makefile
#################################################

## prevents these targets from being treated as file names
.PHONY: all clean example-demo examples prebuild test tests

## project
VERSION			:= $(strip $(shell cat ../../VERSION))

## tools
AR				?= ar
CC      		?= gcc
RM				?= rm -f

## parameters
CSTANDARD		?= c11
CFLAGS  		?= -O2 -Wall -Wextra -std=$(CSTANDARD) -MMD -MP
CPPFLAGS		?= -Iinclude
ARFLAGS 		:= rcs
LDLIBS 			:= -lcmocka -lm		# link with cmocka, math

## outputs
BIN_DIR			:= $(CURDIR)/bin
LOG_DIR			:= $(CURDIR)/logs
LIB_DIR			:= $(CURDIR)/lib
LIB 			:= $(LIB_DIR)/libdmotservices_c-$(VERSION).a
TESTS_FAILED	:= $(LOG_DIR)/tests.failed

## sources
SRC				:= $(shell find src -type f -name '*.c')
OBJ 			:= $(SRC:.c=.o)
DEPS			:= $(OBJ:.o=.d)

## ANSI
ANSI_RED	:= \e[0;31m
ANSI_RESET	:= \e[0m

prebuild:
	@mkdir -p "$(BIN_DIR)"
	@mkdir -p "$(LIB_DIR)"
	@mkdir -p "$(LOG_DIR)"
                                                                                                                                          

#####################################################################################
## default target; creates the objects and the object archive
#####################################################################################
all: prebuild lib

lib: $(OBJ)
	@echo "AR $(LIB)"
	$(AR) $(ARFLAGS) $(LIB) $(OBJ)

%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# auto-include header dependencies
#include $(DEPS)

#####################################################################################
## builds the test cases
#####################################################################################
TEST_SRCS := $(wildcard tests/*.c)
TEST_BINS := $(TEST_SRCS:.c=)

$(TEST_BINS): %: %.c lib
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $< $(LIB) $(LDLIBS)
	@chmod +x "$@";
	@[ ! "$@" ] || mv -v "$@" "$(BIN_DIR)";

tests: $(TEST_BINS)

## executes each test case after building
test: tests
	@[ ! -f "$(TESTS_FAILED)" ] || rm "$(TESTS_FAILED)"
	@find "$(BIN_DIR)" -name "test_*" -exec sh -c 'chmod +x "$$1" && "$$1" || touch "$(TESTS_FAILED)"' _ {} \;;
	@if [ -f "$(TESTS_FAILED)" ]; then \
		rm "$(TESTS_FAILED)"; \
		echo "$(ANSI_RESET)$(ANSI_RED)ERROR: At least one unit test failed.$(ANSI_RESET)"; \
		exit 1; \
	fi
	@find -name "*.log" -not -path "**/logs/*" -exec mv -v "{}" "$(LOG_DIR)" \;;


#####################################################################################
## builds the examples
#####################################################################################
EX_SRCS := $(wildcard examples/*.c)
EX_BINS := $(EX_SRCS:.c=)

$(EX_BINS): %: %.c lib
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $< $(LIB) $(LDLIBS)
	@if [ -f "$@" ]; then \
		chmod +x "$@"; \
		mv -v "$@" "$(BIN_DIR)"; \
	fi

examples: $(EX_BINS)

## demonstrates the examples
example-demo: examples
	@set -e; \
	find "$(BIN_DIR)" -name "ex_*" -exec sh -c 'chmod +x "$$1" && "$$1"' _ {} \;;
	@find -name "*.log" -not -path "**/logs/*" -exec mv -v "{}" "$(LOG_DIR)" \;

#####################################################################################
## Housekeeping
#####################################################################################
clean:
	@$(RM) $(BIN_DIR)/* $(LOG_DIR)/* $(OBJ) $(LIB) $(DEPS) $(TEST_BINS) $(EX_BINS)
	@find examples -name "*.d" -exec $(RM) {} \;
	@find tests -name "*.d" -exec $(RM) {} \;
	@rm -Rf "$(BIN_DIR)"
	@rm -Rf "$(LIB_DIR)"
	@rm -Rf "$(LOG_DIR)"

