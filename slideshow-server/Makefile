#################################################
## slideshow-server/Makefile
#################################################

.PHONY: all build clean dist install example-demo prebuild test

# build prerequisites
VERSION := $(strip $(shell cat VERSION))
export VERSION

# locations
ROOT        := $(CURDIR)
DIST_DIR    := $(ROOT)/dist
BUILD_DIR   := $(ROOT)/build
DIST_NAME   := slideshow-server-$(VERSION)
DIST_STAGE  := $(DIST_DIR)/$(DIST_NAME)

# commands
MILESTONE    := echo === 
PYTHON       ?= python3
TEST_CMD     ?= $(PYTHON) -m pytest -vv

#####################################################################################
## Default build
#####################################################################################
all: prebuild build

prebuild:
	@cat slideshow-server.logo

build:
	@$(MILESTONE) "Compiling Python bytecode"
	@rm -rf "$(BUILD_DIR)"
	@mkdir -p "$(BUILD_DIR)"
	@$(PYTHON) -m compileall -q -b slideshow_server slideshow-server.py
	@find slideshow_server -name "*.pyc" -print | while IFS= read -r file; do \
		rel=$${file#"$(ROOT)/"}; \
		dest="$(BUILD_DIR)/$${rel}"; \
		dest_dir="$$(dirname "$$dest")"; \
		mkdir -p "$$dest_dir"; \
		mv "$$file" "$$dest"; \
	done || true
	@[ ! -f "slideshow-server.pyc" ] || mv "slideshow-server.pyc" "$(BUILD_DIR)/slideshow-server.pyc"


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
	@$(MILESTONE) "Packaging slideshow-server $(VERSION)"
	@mkdir -p "$(DIST_STAGE)"
	@cp -v README.md run-demo.sh slideshow-server.py slideshow-server.css \
		slideshow-server.html slideshow-server.js VERSION "$(DIST_STAGE)"/
	@cp -Rv slideshow_server "$(DIST_STAGE)"/
	@[ ! -d "$(BUILD_DIR)" ] || cp -Rv "$(BUILD_DIR)" "$(DIST_STAGE)"/
	@tar -czf "$(DIST_DIR)/$(DIST_NAME).tar.gz" -C "$(DIST_DIR)" "$(DIST_NAME)"
	@$(MILESTONE) "Bundle available at $(DIST_DIR)/$(DIST_NAME).tar.gz"


#####################################################################################
## Examples
#####################################################################################
example-demo:
	@$(MILESTONE) "No examples here yet."

#####################################################################################
## housekeeping
#####################################################################################
clean:
	@$(MILESTONE) "Cleaning generated files"
	@find slideshow_server -name "*.pyc" -delete || true
	@rm -f "slideshow-server.pyc"
	@rm -rf "$(BUILD_DIR)" "$(DIST_DIR)"
