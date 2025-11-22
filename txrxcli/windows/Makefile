#################################################
## txrxcli/windows/Makefile
#################################################

DOTNET ?= dotnet
PROJECT := txrxcli-dotnet/txrxcli-dotnet.csproj
RID ?= win-x64
CONFIG ?= Release
PUBLISH_DIR := txrxcli-dotnet/bin/$(CONFIG)/net8.0/$(RID)/publish
COPY_DIR := $(CURDIR)/../build/windows
DOTNET_ARGS ?= --mode server --port 9000 --key SECRET

.PHONY: all clean tests test examples example-demo run restore

all: publish

restore:
	DOTNET_CLI_HOME=$(CURDIR) $(DOTNET) restore $(PROJECT)
	DOTNET_CLI_HOME=$(CURDIR) $(DOTNET) restore txrxcli-dotnet/tests/txrxcli-dotnet.Tests.csproj

publish: restore
	DOTNET_CLI_HOME=$(CURDIR) $(DOTNET) publish $(PROJECT) -c $(CONFIG) -r $(RID) --self-contained true
	@mkdir -p $(COPY_DIR)
	@if [ -f "$(PUBLISH_DIR)/txrxcli-dotnet.exe" ]; then cp -vr "$(PUBLISH_DIR)/" "$(COPY_DIR)/"; fi

clean:
	$(DOTNET) clean $(PROJECT) -c $(CONFIG) || true
	rm -rf txrxcli-dotnet/bin txrxcli-dotnet/obj

tests:
	$(MAKE) restore
	DOTNET_CLI_HOME=$(CURDIR) $(DOTNET) build txrxcli-dotnet/tests/txrxcli-dotnet.Tests.csproj -c $(CONFIG)

test: tests
	DOTNET_CLI_HOME=$(CURDIR) $(DOTNET) test txrxcli-dotnet/tests/txrxcli-dotnet.Tests.csproj -c $(CONFIG) --no-build -l "console;verbosity=detailed"

examples:
	@echo "No examples defined for txrxcli-dotnet."

example-demo: examples
	@echo "No examples to run for txrxcli-dotnet."

run:
	DOTNET_CLI_HOME=$(CURDIR) $(DOTNET) run --project $(PROJECT) -- $(DOTNET_ARGS)
