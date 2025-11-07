#################################################
## libdmotservices/js/Makefile
#################################################

.PHONY: all build clean lint test vendor

MILESTONE := echo ===
DIST_DIR := dist
SRC := src/00-banner.js src/logger.js src/formdata.js src/exports.js
DIST_FILE := $(DIST_DIR)/index.js
VENDOR_DIR := vendor/libdmotservices
VENDOR_FILE := $(VENDOR_DIR)/index.js
NPM ?= npm

all: build

build: $(DIST_FILE)

$(DIST_FILE): $(SRC)
	@$(MILESTONE) "Building libdmotservices JS bundle"
	@mkdir -p "$(DIST_DIR)"
	@cat $(SRC) > "$(DIST_FILE)"

vendor: build
	@$(MILESTONE) "Preparing JS vendor artifact"
	@mkdir -p "$(VENDOR_DIR)"
	@cp -v "$(DIST_FILE)" "$(VENDOR_FILE)"

lint:
	@set -e; \
	if [ ! -d node_modules ]; then \
		$(MILESTONE) "Installing JS dependencies"; \
		if ! $(NPM) install; then \
			echo "=== Warning: npm install failed; skipping lint (run manually when network is available)"; \
			exit 0; \
		fi; \
	fi; \
	$(MILESTONE) "Linting libdmotservices JS sources"; \
	$(NPM) run lint

test: build lint
	@$(MILESTONE) "Running libdmotservices JS unit tests"
	@$(NPM) run test

example-demo:
	@$(MILESTONE) "No examples here yet."

clean:
	@$(MILESTONE) "Cleaning libdmotservices JS artifacts"
	@rm -rf "$(DIST_DIR)" "$(VENDOR_DIR)" node_modules package-lock.json
