#################################################
## WiFiEqu/linux/Makefile
#################################################

## prevents these targets from being treated as file names
.PHONY: all clean install example-demo examples prebuild setup-user test tests

# locations
ROOT 			:= $(strip $(shell pwd))
LIB_DIR			:= ../../libdmotservices/c/linux
ARCHIVE_DIR  	:= $(LIB_DIR)/lib
ARCHIVE			:= $(ARCHIVE_DIR)/libdmotservices_c-0.3.0.a
CONFIG_DIR		:= $(ROOT)/etc
CONFIG_FILE		:= wifiequd.conf
CONFIG			:= $(CONFIG_DIR)/$(CONFIG_FILE)
SERVICE_DIR		:= $(CONFIG_DIR)/systemd/system
SERVICE_FILE	:= wifiequd.service
SERVICE			:= $(SERVICE_DIR)/$(SERVICE_FILE)

## outputs
BIN_DIR			:= $(CURDIR)/bin
LOG_DIR			:= $(CURDIR)/logs
BIN				:= wifiequd
TESTS_FAILED    := $(LOG_DIR)/tests.failed

## sources
SRC				:= $(shell find src -type f -name '*.c')
OBJ 			:= $(SRC:.c=.o)
MAIN_OBJ		:= src/wifiequd.o
CORE_OBJ		:= $(filter-out $(MAIN_OBJ), $(OBJ))
DEPS			:= $(OBJ:.o=.d)

## parameters
CSTANDARD		?= c11
CFLAGS  		?= -O2 -g -Wall -Wextra -std=$(CSTANDARD) -MMD -MP -D_GNU_SOURCE
CPPFLAGS     	:= -I./include -I../../libdmotservices/c/linux/include -I/usr/include/libnl3
LDLIBS 			:= -L$(ARCHIVE_DIR) -lm -lmicrohttpd -lnl-3 -lnl-genl-3
LDLIBS_TEST		:= -lcmocka

## commands
CC      	?= gcc
RM			?= rm -f
MILESTONE	:= echo === 

## ANSI
ANSI_RED        := \e[0;31m
ANSI_RESET      := \e[0m

#####################################################################################
## default target; creates the objects
#####################################################################################
all: prebuild $(BIN)

%.o: %.c
	@$(MILESTONE) "Compiling the source..."
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

prebuild:
	@$(MILESTONE) "Carrying out prebuild tasks..."
	@mkdir -p "$(BIN_DIR)"
	@mkdir -p "$(LOG_DIR)"
	$(MAKE) -C "$(LIB_DIR)"

$(BIN): $(OBJ)
	@$(MILESTONE) "Linking all objects and libraries to produce wifiequd..."
	$(CC) $(CFLAGS) $(CPPFLAGS) -o "$(BIN_DIR)/$@" $(OBJ) $(ARCHIVE) $(LDLIBS)

#####################################################################################
## deployment
#####################################################################################
install: setup-user deploy

setup-user:
	@$(MILESTONE) "Creating the wifiequ user and group..."
	@getent group wifiequ >/dev/null || groupadd --system wifiequ
	@getent passwd wifiequ >/dev/null || \
		useradd --system --home-dir /var/lib/wifiequ --shell /usr/sbin/nologin \
		        --gid wifiequ --comment "WiFiEqu daemon user" wifiequ
	@install -d -m 0750 -o wifiequ -g wifiequ /run/wifiequ
	@install -d -m 0750 -o wifiequ -g wifiequ /var/lib/wifiequ

deploy:
	@$(MILESTONE) "Deploying wifiequd..."
	@cp -v $(SERVICE) /etc/systemd/system/$(SERVICE_FILE)
	@cp -v $(CONFIG) /etc/$(CONFIG_FILE)
	@cp -v $(BIN_DIR)/$(BIN) /usr/local/bin/$(BIN)
	@chmod +x /usr/local/bin/$(BIN)

#####################################################################################
## builds the test cases
#####################################################################################
TEST_SRCS := $(wildcard tests/*.c)
TEST_BINS := $(TEST_SRCS:.c=)

$(TEST_BINS): %: %.c
	@$(MILESTONE) "Linking core objects and libraries to produce test binaries..."
	$(CC) $(CFLAGS) $(CPPFLAGS) $(CORE_OBJ) -o $@ $< $(ARCHIVE) $(LDLIBS) $(LDLIBS_TEST)
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

$(EX_BINS): %: %.c
	@$(MILESTONE) "Linking core objects and libraries to produce example binaries..."
	$(CC) $(CFLAGS) $(CPPFLAGS) $(CORE_OBJ) -o $@ $< $(ARCHIVE) $(LDLIBS)
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
## housekeeping
#####################################################################################
clean:
	$(RM) $(BIN_DIR)/* $(LOG_DIR)/* $(OBJ) $(LIB) $(DEPS) $(TEST_BINS) $(EX_BINS)
	find examples -name "*.d" -exec $(RM) {} \;
	find tests -name "*.d" -exec $(RM) {} \;
	rm -Rf "$(BIN_DIR)"
	rm -Rf "$(LOG_DIR)"