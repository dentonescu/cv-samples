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
BUILD_DIR		:= $(CURDIR)/build
BIN_DIR			:= $(BUILD_DIR)/bin
LOG_DIR			:= $(BUILD_DIR)/logs
LIB_DIR			:= $(BUILD_DIR)/lib
OBJ_DIR			:= $(BUILD_DIR)/obj
LIB 			:= $(LIB_DIR)/libdmotservices_c-$(VERSION).a
TESTS_FAILED	:= $(LOG_DIR)/tests.failed

## sources
SRC				:= $(shell find src -type f -name '*.c')
OBJ 			:= $(patsubst src/%.c,$(OBJ_DIR)/%.o,$(SRC))
DEPS			:= $(OBJ:.o=.d)

## ANSI
ANSI_RED	:= \e[0;31m
ANSI_RESET	:= \e[0m

prebuild:
	@mkdir -p "$(BIN_DIR)"
	@mkdir -p "$(LIB_DIR)"
	@mkdir -p "$(LOG_DIR)"
	@mkdir -p "$(OBJ_DIR)"
                                                                                                                                          

#####################################################################################
## default target; creates the objects and the object archive
#####################################################################################
all: prebuild lib

lib: prebuild $(OBJ)
	@echo "AR $(LIB)"
	$(AR) $(ARFLAGS) $(LIB) $(OBJ)

$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# auto-include header dependencies
#include $(DEPS)

#####################################################################################
## builds the test cases
#####################################################################################
TEST_SRCS := $(wildcard tests/*.c)
TEST_BINS := $(patsubst tests/%.c,$(BIN_DIR)/tests/%,$(TEST_SRCS))

$(BIN_DIR)/tests/%: tests/%.c lib
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $< $(LIB) $(LDLIBS)
	@chmod +x "$@";

tests: prebuild $(TEST_BINS)

## executes each test case after building
test: tests
	@[ ! -f "$(TESTS_FAILED)" ] || rm "$(TESTS_FAILED)"
	@find "$(BIN_DIR)" -name "test_*" -not -name "*.d" -exec sh -c 'chmod +x "$$1" && "$$1" || touch "$(TESTS_FAILED)"' _ {} \;;
	@if [ -f "$(TESTS_FAILED)" ]; then \
		rm "$(TESTS_FAILED)"; \
		echo "$(ANSI_RESET)$(ANSI_RED)ERROR: At least one unit test failed.$(ANSI_RESET)"; \
		exit 1; \
	fi
	@find -name "*.log" -not -path "**/logs/*" -exec mv -vf "{}" "$(LOG_DIR)" \;;


#####################################################################################
## builds the examples
#####################################################################################
EX_SRCS := $(wildcard examples/*.c)
EX_BINS := $(patsubst examples/%.c,$(BIN_DIR)/examples/%,$(EX_SRCS))

$(BIN_DIR)/examples/%: examples/%.c lib
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $< $(LIB) $(LDLIBS)
	@if [ -f "$@" ]; then \
		chmod +x "$@"; \
	fi

examples: prebuild $(EX_BINS)

## demonstrates the examples
example-demo: examples
	@set -e; \
	find "$(BIN_DIR)" -name "ex_*" -not -name "*.d" -exec sh -c 'chmod +x "$$1" && "$$1"' _ {} \;;
	@find -name "*.log" -not -path "**/logs/*" -exec mv -vf "{}" "$(LOG_DIR)" \;

#####################################################################################
## Housekeeping
#####################################################################################
clean:
	@rm -rf "$(BUILD_DIR)"
