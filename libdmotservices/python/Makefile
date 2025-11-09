#################################################
## libdmotservices/python/Makefile
#################################################

.PHONY: all build clean dist install example-demo prebuild test version vendor

# build prerequisites
VERSION := $(strip $(shell cat ../VERSION))
export VERSION

# locations
ROOT        := $(CURDIR)
DIST_DIR    := $(ROOT)/dist
BUILD_DIR   := $(ROOT)/build
DIST_NAME   := libdmotservices-py-$(VERSION)
DIST_STAGE  := $(DIST_DIR)/$(DIST_NAME)
VENDOR_ROOT := $(ROOT)/vendor
PACKAGE_DIR := libdmotservices

# commands
MILESTONE     := echo === 
PYTHON        ?= python3
TEST_CMD      ?= $(PYTHON) -m pytest -svv -o log-cli=true --log-cli-level=DEBUG

export PYTHONPATH := $(ROOT):$(VENDOR_ROOT):$(PYTHONPATH)

vendor: prebuild
	@$(MILESTONE) "Vendoring $(PACKAGE_DIR) into $(VENDOR_ROOT)"
	@rm -rf "$(VENDOR_ROOT)/$(PACKAGE_DIR)"
	@mkdir -p "$(VENDOR_ROOT)"
	@cp -Rv "$(PACKAGE_DIR)" "$(VENDOR_ROOT)/"

#####################################################################################
## Default build
#####################################################################################
all: vendor build

version:
	@sed -i 's/^version = ".*"/version = "$(VERSION)"/' pyproject.toml

prebuild: version
	@cp -v ../VERSION libdmotservices/.version

build:
	@$(MILESTONE) "Compiling Python bytecode"
	@rm -rf "$(BUILD_DIR)"
	@mkdir -p "$(BUILD_DIR)"
	@$(PYTHON) -m compileall -q -b libdmotservices
	@find libdmotservices -name "*.pyc" -print | while IFS= read -r file; do \
		rel=$${file#"$(ROOT)/"}; \
		dest="$(BUILD_DIR)/$${rel}"; \
		dest_dir="$$(dirname "$$dest")"; \
		mkdir -p "$$dest_dir"; \
		mv -vf "$$file" "$$dest"; \
	done || true


#####################################################################################
## Deployment
#####################################################################################

install:
	@$(MILESTONE) "Nothing to install yet."


#####################################################################################
## Unit tests
#####################################################################################

test:
	@$(MILESTONE) "Running unit tests"
	@$(TEST_CMD)


#####################################################################################
## Distribution bundle
#####################################################################################
dist: clean build
	@$(MILESTONE) "Packaging libdmotservices $(VERSION)"
	@mkdir -p "$(DIST_STAGE)"
	@cp -v README.md ../VERSION "$(DIST_STAGE)"/
	@cp -Rv libdmotservices "$(DIST_STAGE)"/
	@[ ! -d "$(BUILD_DIR)" ] || cp -Rv "$(BUILD_DIR)" "$(DIST_STAGE)"/
	@tar -czf "$(DIST_DIR)/$(DIST_NAME).tar.gz" -C "$(DIST_DIR)" "$(DIST_NAME)"
	@$(MILESTONE) "Bundle available at $(DIST_DIR)/$(DIST_NAME).tar.gz"


#####################################################################################
## Examples
#####################################################################################
example-demo:
	@$(MILESTONE) "No examples here yet."

#####################################################################################
## Housekeeping
#####################################################################################
clean:
	@$(MILESTONE) "Cleaning generated files"
	@find libdmotservices -name "*.pyc" -delete || true
	@rm -rf "$(BUILD_DIR)" "$(DIST_DIR)"
