#################################################
## pkixwebadm/Makefile
#################################################

.PHONY: all build clean dist install example-demo lint-js logo prebuild test vendor-libdmotservices version

# build prerequisites
VERSION := $(strip $(shell cat VERSION))
export VERSION

# locations
ROOT        := $(CURDIR)
DIST_DIR    := $(ROOT)/dist
BUILD_DIR   := $(ROOT)/build
DIST_NAME   := pkixwebadm-$(VERSION)
DIST_STAGE  := $(DIST_DIR)/$(DIST_NAME)
LIBDMOTSERVICES_PY_ROOT ?= $(ROOT)/../libdmotservices/python
LIBDMOTSERVICES_JS_ROOT ?= $(ROOT)/../libdmotservices/js
LIBDMOTSERVICES_JS_VENDOR_FILE ?= $(LIBDMOTSERVICES_JS_ROOT)/vendor/libdmotservices/index.js
PKIXWA_VENDOR_JS_DIR := $(ROOT)/pkixwebadm/web/static/vendor/libdmotservices
NPM ?= npm
NODE_MODULES := node_modules

# commands
MILESTONE    := echo === 
PYTHON       ?= python3
TEST_CMD     ?= $(PYTHON) -m pytest -svv

# pip is blocked on some managed hosts, so we vendor libdmotservices instead.
export PYTHONPATH := $(LIBDMOTSERVICES_PY_ROOT):$(LIBDMOTSERVICES_PY_ROOT)/vendor:$(PYTHONPATH)

vendor-libdmotservices:
	@$(MILESTONE) "Vendoring libdmotservices for pkixwebadm"
	@$(MAKE) -C "$(LIBDMOTSERVICES_PY_ROOT)" vendor
	@$(MAKE) -C "$(LIBDMOTSERVICES_JS_ROOT)" vendor
	@mkdir -p "$(PKIXWA_VENDOR_JS_DIR)"
	@cp -uv "$(LIBDMOTSERVICES_JS_VENDOR_FILE)" "$(PKIXWA_VENDOR_JS_DIR)/index.js"

#####################################################################################
## Default build
#####################################################################################
all: logo prebuild build

logo:
	@cat pkixwebadm.logo

version:
	@cp -v VERSION pkixwebadm/.version

prebuild: vendor-libdmotservices version

lint-js:
	@set -e; \
	if [ ! -d "$(NODE_MODULES)" ]; then \
		$(MILESTONE) "Installing pkixwebadm JavaScript dependencies"; \
		if ! $(NPM) install; then \
			echo "=== Warning: npm install failed; skipping lint (run manually when network is available)"; \
			exit 0; \
		fi; \
	fi; \
	$(MILESTONE) "Linting pkixwebadm JavaScript sources"; \
	$(NPM) run lint:js

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

test: prebuild lint-js
	@$(MILESTONE) "Running unit tests"
	@$(TEST_CMD)


#####################################################################################
## Distribution bundle
#####################################################################################
dist: clean build
	@$(MILESTONE) "Packaging pkixwebadm $(VERSION)"
	@mkdir -p "$(DIST_STAGE)"
	@mkdir -p "$(DIST_STAGE)/vendor"
	@cp -Rv "$(LIBDMOTSERVICES_ROOT)/vendor/libdmotservices" "$(DIST_STAGE)/vendor/"
	@cp -v README.md pkixwebadm.py VERSION "$(DIST_STAGE)"/
	@cp -Rv pkixwebadm "$(DIST_STAGE)"/
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
	@find pkixwebadm -name "*.pyc" -delete || true
	@rm -f "pkixwebadm.pyc"
	@rm -rf "$(BUILD_DIR)" "$(DIST_DIR)" "$(NODE_MODULES)" package-lock.json
