#################################################
## txrxcli/Makefile (no CMake dependency)
#################################################

LINUX_DIR := linux
LINUX_BUILD := $(LINUX_DIR)/build
WINDOWS_DIR := windows
DIST_DIR := dist
DOTNET_ARGS ?= --mode server --port 9000 --key SECRET

.PHONY: all linux windows dist clean
.PHONY: test tests examples example-demo run-windows

all: linux windows

linux:
	$(MAKE) -C $(LINUX_DIR)

windows:
	$(MAKE) -C $(WINDOWS_DIR)

dist: linux windows
	@mkdir -p $(DIST_DIR)/linux
	@if [ -x "$(LINUX_BUILD)/txrxcli" ]; then cp $(LINUX_BUILD)/txrxcli $(DIST_DIR)/linux/; fi
	@mkdir -p $(DIST_DIR)/windows
	@if [ -x "$(WINDOWS_DIR)/txrxcli-dotnet/bin/Release/net8.0/win-x64/publish/txrxcli-dotnet.exe" ]; then \
		cp $(WINDOWS_DIR)/txrxcli-dotnet/bin/Release/net8.0/win-x64/publish/txrxcli-dotnet.exe $(DIST_DIR)/windows/; \
	fi

clean:
	$(MAKE) -C $(LINUX_DIR) clean
	$(MAKE) -C $(WINDOWS_DIR) clean
	rm -rf $(DIST_DIR)

# Build unit tests via native make in linux/
tests:
	$(MAKE) -C $(LINUX_DIR) tests
	$(MAKE) -C $(WINDOWS_DIR) tests

# Run unit tests
test:
	$(MAKE) -C $(LINUX_DIR) test
	$(MAKE) -C $(WINDOWS_DIR) test

# No examples for this project (placeholder targets)
examples:
	@echo "No examples defined for txrxcli (placeholder)."

example-demo: examples
	@echo "No examples to run for txrxcli (placeholder)."

# Run the .NET app via dotnet (use DOTNET_ARGS to override)
run-windows:
	$(MAKE) -C $(WINDOWS_DIR) run DOTNET_ARGS="$(DOTNET_ARGS)"
