#################################################
## txrxcli/linux/Makefile
#################################################

CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -pedantic
INCLUDES := -Iinclude
SRCS := $(wildcard src/*.cpp) $(wildcard src/server/*.cpp) $(wildcard src/client/*.cpp)
TEST_SRCS := $(wildcard tests/*.cpp) src/protocol.cpp
BUILD_DIR ?= build
TARGET := $(BUILD_DIR)/txrxcli
TEST_BIN := $(BUILD_DIR)/txrxcli_tests
LDFLAGS := 
LDLIBS := 

# Optional libs for tests
TEST_LIBS := -lgtest -lgtest_main -lpthread -lspdlog -lfmt

.PHONY: all clean test tests examples example-demo

all: $(TARGET)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(TARGET): $(SRCS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRCS) -o $(TARGET) $(LDFLAGS) $(LDLIBS)

clean:
	rm -rf $(BUILD_DIR)

# Build unit tests
tests: $(TEST_BIN)

$(TEST_BIN): $(TEST_SRCS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(TEST_SRCS) -o $(TEST_BIN) $(LDFLAGS) $(LDLIBS) $(TEST_LIBS)

# Run unit tests
test: $(TEST_BIN)
	$(TEST_BIN)

# No examples for this project (placeholder targets)
examples:
	@echo "No examples defined for txrxcli (placeholder)."

example-demo: examples
	@echo "No examples to run for txrxcli (placeholder)."
