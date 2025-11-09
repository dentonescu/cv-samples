#################################################
## slideshow-server/Makefile
#################################################

.PHONY: all build clean dist install example-demo logo prebuild test version vendor-libdmotservices

# build prerequisites
VERSION := $(strip $(shell cat VERSION))
export VERSION

# locations
ROOT        := $(CURDIR)
DIST_DIR    := $(ROOT)/dist
BUILD_DIR   := $(ROOT)/build
DIST_NAME   := slideshow-server-$(VERSION)
DIST_STAGE  := $(DIST_DIR)/$(DIST_NAME)
LIBDMOTSERVICES_ROOT ?= $(ROOT)/../libdmotservices/python

# commands
MILESTONE    := echo === 
PYTHON       ?= python3
TEST_CMD     ?= $(PYTHON) -m pytest -svv -o log-cli=true --log-cli-level=DEBUG

# pip is blocked on some managed hosts, so we vendor libdmotservices instead.
export PYTHONPATH := $(LIBDMOTSERVICES_ROOT):$(LIBDMOTSERVICES_ROOT)/vendor:$(PYTHONPATH)

vendor-libdmotservices:
	@$(MILESTONE) "Vendoring libdmotservices for slideshow-server"
	@$(MAKE) -C "$(LIBDMOTSERVICES_ROOT)" vendor

#####################################################################################
## Default build
#####################################################################################
all: logo prebuild build

logo:
	@cat slideshow-server.logo

version:
	@cp -v VERSION slideshow_server/.version

prebuild: vendor-libdmotservices version

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
		mv -vf "$$file" "$$dest"; \
	done || true
	@[ ! -f "slideshow-server.pyc" ] || mv -vf "slideshow-server.pyc" "$(BUILD_DIR)/slideshow-server.pyc"


#####################################################################################
## Deployment
#####################################################################################

install:
	@$(MILESTONE) "Nothing to install yet."


#####################################################################################
## Unit tests
#####################################################################################

test: prebuild
	@$(MILESTONE) "Running unit tests"
	@$(TEST_CMD)


#####################################################################################
## Distribution bundle
#####################################################################################
dist: clean build
	@$(MILESTONE) "Packaging slideshow-server $(VERSION)"
	@mkdir -p "$(DIST_STAGE)"
	@mkdir -p "$(DIST_STAGE)/vendor"
	@cp -Rv "$(LIBDMOTSERVICES_ROOT)/vendor/libdmotservices" "$(DIST_STAGE)/vendor/"
	@cp -v README.md run-demo.sh slideshow-server.py slideshow-server.css \
		slideshow-server.html slideshow-server.js VERSION "$(DIST_STAGE)"/
	@cp -Rv slideshow_server "$(DIST_STAGE)"/
	@[ ! -d "$(BUILD_DIR)" ] || cp -Rv "$(BUILD_DIR)" "$(DIST_STAGE)"/
	@tar -czf "$(DIST_DIR)/$(DIST_NAME).tar.gz" -C "$(DIST_DIR)" "$(DIST_NAME)"
	@$(MILESTONE) "Bundle available at $(DIST_DIR)/$(DIST_NAME).tar.gz"


#####################################################################################
## Examples
#####################################################################################
example-demo: prebuild
	@$(MILESTONE) "No examples here yet."

#####################################################################################
## Housekeeping
#####################################################################################
clean:
	@$(MILESTONE) "Cleaning generated files"
	@find slideshow_server -name "*.pyc" -delete || true
	@rm -f "slideshow-server.pyc"
	@rm -rf "$(BUILD_DIR)" "$(DIST_DIR)"
