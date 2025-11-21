#################################################
## txrxcli/Makefile (no CMake dependency)
#################################################

LINUX_DIR := linux
LINUX_BUILD := $(LINUX_DIR)/build
DIST_DIR := dist

.PHONY: all linux dist clean
.PHONY: test tests examples example-demo

all: linux

linux:
	$(MAKE) -C $(LINUX_DIR)

dist: linux
	@mkdir -p $(DIST_DIR)/linux
	cp $(LINUX_BUILD)/txrxcli $(DIST_DIR)/linux/

clean:
	$(MAKE) -C $(LINUX_DIR) clean
	rm -rf $(DIST_DIR)

# Build unit tests via native make in linux/
tests:
	$(MAKE) -C $(LINUX_DIR) tests

# Run unit tests
test:
	$(MAKE) -C $(LINUX_DIR) test

# No examples for this project (placeholder targets)
examples:
	@echo "No examples defined for txrxcli (placeholder)."

example-demo: examples
	@echo "No examples to run for txrxcli (placeholder)."
