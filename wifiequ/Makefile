#################################################
## WiFiEqu/Makefile
#################################################

.PHONY: all angular clean dist docs install example-demo examples linux prebuild setup-user test tests windows

# build prerequisites
VERSION := $(strip $(shell cat VERSION))

export VERSION

# locations
ROOT            := $(CURDIR)
DIST            := dist
OPENAPI         := $(ROOT)/api/openapi.yaml

# outputs
DOC_DIR_API     := docs/api
DOC_API_HTML    := $(DOC_DIR_API)/index.html

# commands
MILESTONE := echo === 


#####################################################################################
## Default build
#####################################################################################
all: prebuild version linux dist docs

dist:
	@$(MILESTONE) "Populating the distribution directory..."
	@mkdir -p "$(DIST)"
	@find "$(ROOT)/linux" -name "*.a" -type f -exec cp -v "{}" "$(DIST)"/ \;
	@for dir in "$(ROOT)/linux/bin" "$(ROOT)/linux/build"; do \
		if [ -d "$$dir" ]; then \
			cp -vR "$$dir"/. "$(DIST)"/; \
		fi; \
	done

docs: $(DOC_API_HTML)

prebuild:
	@cat wifiequ.logo


#####################################################################################
## Version management
#####################################################################################
version:
	@$(MILESTONE) "Generating version data..."
	@echo "VERSION: $(VERSION)"
	@./gen-version.py >/dev/null


#####################################################################################
## Deployment
#####################################################################################

install:
	@$(MILESTONE) "Deploying the applications..."
	# TO-DO: This target would have to do different things depending on which OS called it.


#####################################################################################
## Linux build
#####################################################################################
linux:
	@$(MILESTONE) "Building the Linux application..."
	@$(MAKE) -C linux all


#####################################################################################
## Windows build
#####################################################################################
windows:
	@$(MILESTONE) "Building the Windows application..."
	@$(MAKE) -C windows all


#####################################################################################
## Angular build
#####################################################################################
angular:
	@$(MILESTONE) "Building the Angular application..."
	@$(MAKE) -C angular all


#####################################################################################
## Unit tests
#####################################################################################
tests:
	@$(MILESTONE) "Compiling all tests..."
	@$(MAKE) -C linux tests

test: tests
	@$(MILESTONE) "Executing all tests..."
	@$(MAKE) -C linux test


#####################################################################################
## Examples
#####################################################################################
examples:
	@$(MILESTONE) "Compiling all examples..."
	@$(MAKE) -C linux examples

example-demo: examples
	@$(MILESTONE) "Executing all examples and demos..."
	@$(MAKE) -C linux example-demo

#####################################################################################
## housekeeping
#####################################################################################
clean:
	@$(MILESTONE) "Removing previous builds..."
	@$(MAKE) -C linux clean || true
	@rm -Rf "$(DIST)"
	@rm -Rf "$(DOC_DIR_API)"

#####################################################################################
## documentation helpers
#####################################################################################

$(DOC_DIR_API):
	@mkdir -p "$@"

$(DOC_API_HTML): $(OPENAPI) | $(DOC_DIR_API)
	@$(MILESTONE) "Generating the OpenAPI specification as HTML..."
	@if ! npx --no-install @redocly/cli --version >/dev/null 2>&1; then \
		echo "Installing @redocly/cli (dev dependency)..."; \
		npm install --save-dev @redocly/cli >/dev/null; \
	fi
	@npx @redocly/cli build-docs "$<" --output "$@"
