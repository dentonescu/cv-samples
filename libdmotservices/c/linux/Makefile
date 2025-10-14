# libdmotservices/c/Makefile

## tools
AR			?= ar
CC      	?= gcc
RM			?= rm -f

## parameters
CSTANDARD	?= c11
CFLAGS  	?= -O2 -Wall -Wextra -std=$(CSTANDARD) -MMD -MP
CPPFLAGS	?= -Iinclude
ARFLAGS 	:= rcs

## 
LDLIBS 		:= -lcmocka -lm		# link with cmocka, math

## outputs
BIN_DIR		:= $(CURDIR)/bin
LOG_DIR		:= $(CURDIR)/logs
LIB 		:= libdmotservices_c.a


## sources
#SRC := \
#  src/math/signals.c \
#  src/util/log.c
SRC		:= $(shell find src -type f -name '*.c')
OBJ 	:= $(SRC:.c=.o)
DEPS	:= $(OBJ:.o=.d)


prebuild:
	@echo
	@echo "████   ███  █████         █████                           █████                                            ███                       "
	@echo "░░███  ░░░  ░░███         ░░███                           ░░███                                            ░░░                            "
	@echo " ░███  ████  ░███████   ███████  █████████████    ██████  ███████    █████   ██████  ████████  █████ █████ ████   ██████   ██████   █████ "
	@echo " ░███ ░░███  ░███░░███ ███░░███ ░░███░░███░░███  ███░░███░░░███░    ███░░   ███░░███░░███░░███░░███ ░░███ ░░███  ███░░███ ███░░███ ███░░  "
	@echo " ░███  ░███  ░███ ░███░███ ░███  ░███ ░███ ░███ ░███ ░███  ░███    ░░█████ ░███████  ░███ ░░░  ░███  ░███  ░███ ░███ ░░░ ░███████ ░░█████ "
	@echo " ░███  ░███  ░███ ░███░███ ░███  ░███ ░███ ░███ ░███ ░███  ░███ ███ ░░░░███░███░░░   ░███      ░░███ ███   ░███ ░███  ███░███░░░   ░░░░███"
	@echo " █████ █████ ████████ ░░████████ █████░███ █████░░██████   ░░█████  ██████ ░░██████  █████      ░░█████    █████░░██████ ░░██████  ██████ "
	@echo "░░░░░ ░░░░░ ░░░░░░░░   ░░░░░░░░ ░░░░░ ░░░ ░░░░░  ░░░░░░     ░░░░░  ░░░░░░   ░░░░░░  ░░░░░        ░░░░░    ░░░░░  ░░░░░░   ░░░░░░  ░░░░░░  "
	@echo
	mkdir -p "$(BIN_DIR)"
	mkdir -p "$(LOG_DIR)"
                                                                                                                                          

#####################################################################################
## default target; creates the objects and the object archive
#####################################################################################
all: prebuild $(LIB)

$(LIB): $(OBJ)
	@echo "AR $@"
	$(AR) $(ARFLAGS) $@ $(OBJ)

%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# auto-include header dependencies
include $(DEPS)

#####################################################################################
## builds the test cases
#####################################################################################
TEST_SRCS := $(wildcard tests/*.c)
TEST_BINS := $(TEST_SRCS:.c=)

$(TEST_BINS): %: %.c $(LIB)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $< $(LIB) $(LDLIBS)

tests: $(TEST_BINS)

## executes each test case after building
test: tests
	@set -e; \
	for t in $(TEST_BINS); \
	do \
		printf "\n\n"; \
		printf "* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n"; \
		printf "* RUN $$t\n"; \
		printf "* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n"; \
		printf "\n\n"; \
		chmod +x "$$t"; \
		[ ! "$$t" ] || mv -v "$$t" "$(BIN_DIR)"; \
		"$(BIN_DIR)/`basename "$$t"`"; \
	done
	find -name "*.log" -not -path "**/logs/*" -exec mv "{}" "$(LOG_DIR)" \;

#####################################################################################
## builds the examples
#####################################################################################
EX_SRCS := $(wildcard examples/*.c)
EX_BINS := $(EX_SRCS:.c=)

$(EX_BINS): %: %.c $(LIB)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $< $(LIB) $(LDLIBS)

examples: $(EX_BINS)

## demonstrates the examples
example-demo: examples
	@set -e; \
	for t in $(EX_BINS); \
	do \
		printf "\n\n"; \
		printf "* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n"; \
		printf "* RUN $$t\n"; \
		printf "* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n"; \
		printf "\n\n"; \
		chmod +x "$$t"; \
		[ ! "$$t" ] || mv -v "$$t" "$(BIN_DIR)"; \
		"$(BIN_DIR)/`basename "$$t"`"; \
	done
	find -name "*.log" -not -path "**/logs/*" -exec mv "{}" "$(LOG_DIR)" \;

#####################################################################################
## housekeeping
#####################################################################################
clean:
	$(RM) $(BIN_DIR)/* $(LOG_DIR)/* $(OBJ) $(LIB) $(TEST_BINS) $(EX_BINS)

## prevents these targets from being treated as file names
.PHONY: all tests test clean examples example-demo
