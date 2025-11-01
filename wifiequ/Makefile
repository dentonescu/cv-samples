#################################################
## WiFiEqu/Makefile
#################################################

.PHONY: all angular clean dist docs docs-openapi install example-demo examples linux prebuild setup-user test tests windows

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
MILESTONE 		:= echo === 
GATHER_BINS		:= for dir in "$(ROOT)/linux/bin" "$(ROOT)/linux/build"; do \
						if [ -d "$$dir" ]; then \
							cp -vR "$$dir"/. "$(DIST)"/; \
						fi; \
					done


#####################################################################################
## Default build
#####################################################################################
all: prebuild version linux dist

dist:
	@$(MILESTONE) "Populating the distribution directory..."
	@mkdir -p "$(DIST)"
	@find "$(ROOT)/linux" -name "*.a" -type f -exec cp -v "{}" "$(DIST)"/ \;
	@$(GATHER_BINS)

docs: docs-openapi

docs-openapi: $(DOC_DIR_API) $(OPENAPI)
	@$(MILESTONE) "Generating the OpenAPI specification as HTML..."
	@if ! npx --no-install @redocly/cli --version >/dev/null 2>&1; then \
		echo "Installing @redocly/cli (dev dependency)..."; \
		npm install --save-dev @redocly/cli >/dev/null; \
	fi
	@npx @redocly/cli build-docs "$(OPENAPI)" --output "$(DOC_API_HTML)"

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
	@$(MAKE) -C linux install


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
	@$(GATHER_BINS)

test: tests
	@$(MILESTONE) "Executing all tests..."
	@$(MAKE) -C linux test


#####################################################################################
## Examples
#####################################################################################
examples:
	@$(MILESTONE) "Compiling all examples..."
	@$(MAKE) -C linux examples
	@$(GATHER_BINS)

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

#####################################################################################
## documentation helpers
#####################################################################################

$(DOC_DIR_API):
	@mkdir -p "$@"
