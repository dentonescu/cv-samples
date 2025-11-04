#################################################
## pkixwebadm/Makefile
#################################################

.PHONY: all build clean dist install example-demo prebuild test

# build prerequisites
VERSION := $(strip $(shell cat VERSION))
export VERSION

# locations
ROOT        := $(CURDIR)
DIST_DIR    := $(ROOT)/dist
BUILD_DIR   := $(ROOT)/build
DIST_NAME   := pkixwebadm-$(VERSION)
DIST_STAGE  := $(DIST_DIR)/$(DIST_NAME)

# commands
MILESTONE    := echo === 
PYTHON       ?= python3
TEST_CMD     ?= $(PYTHON) -m pytest -svv
PIP			 ?= pip
DEPS_CMD	 ?= $(PIP) install -e .

#####################################################################################
## Default build
#####################################################################################
all: prebuild build

prebuild:
	@cat pkixwebadm.logo
	@$(DEPS_CMD)
	@cp -v VERSION pkixwebadm/.version

build:
	@$(MILESTONE) "Compiling Python bytecode"
	@rm -rf "$(BUILD_DIR)"
	@mkdir -p "$(BUILD_DIR)"
	@$(PYTHON) -m compileall -q -b pkixwebadm pkixwebadm.py
	@find pkixwebadm -name "*.pyc" -print | while IFS= read -r file; do \
		rel=$${file#"$(ROOT)/"}; \
		dest="$(BUILD_DIR)/$${rel}"; \
		dest_dir="$$(dirname "$$dest")"; \
		mkdir -p "$$dest_dir"; \
		mv -vf "$$file" "$$dest"; \
	done || true
	@[ ! -f "pkixwebadm.pyc" ] || mv -vf "pkixwebadm.pyc" "$(BUILD_DIR)/pkixwebadm.pyc"


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
	@$(MILESTONE) "Packaging pkixwebadm $(VERSION)"
	@mkdir -p "$(DIST_STAGE)"
	@cp -v README.md pkixwebadm.py VERSION "$(DIST_STAGE)"/
	@cp -Rv pkixwebadm "$(DIST_STAGE)"/
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
	@find pkixwebadm -name "*.pyc" -delete || true
	@rm -f "pkixwebadm.pyc"
	@rm -rf "$(BUILD_DIR)" "$(DIST_DIR)"
