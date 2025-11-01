#################################################
## cv-samples/Makefile
#################################################

# build prerequisites

## none yet

# locations
DIST := "dist"
ROOT := $(strip $(shell pwd))

# commands
MILESTONE := echo === 
GATHER := 	find "${ROOT}" -name "*.a" -not -path "*/node_modules*" -type f -exec cp -v "{}" "${DIST}" \; ; \
			find "${ROOT}" -name "build" -not -path "*/node_modules*" -type d -exec cp -vR "{}/." "${DIST}" \; ; \
			find "${ROOT}" -name "bin" -not -path "*/node_modules*" -type d -exec cp -vR "{}/." "${DIST}" \;

.PHONY: all build-all clean docs example-demo examples install test tests

#####################################################################################
## Default build
#####################################################################################
all: prebuild build-all dist

build-all:
	@$(MILESTONE) "Building all projects with build files..."
	@$(MAKE) -C libdmotservices clean all
	@$(MAKE) -C wifiequ clean all

dist:
	@$(MILESTONE) "Populating the distribution directory..."
	@mkdir -p "${DIST}"
	@$(GATHER)

prebuild:
	@cat cv-samples.logo

#####################################################################################
## Generated documentation
#####################################################################################
docs:
	@$(MILESTONE) "Generating documentation..."
	@$(MAKE) -C wifiequ docs

#####################################################################################
## Installation and deployment
#####################################################################################
install:
	@$(MILESTONE) "Installing deployable projects..."
	@$(MAKE) -C wifiequ install

#####################################################################################
## Unit tests
#####################################################################################
tests:
	@$(MILESTONE) "Compiling all tests..."
	@$(MAKE) -C libdmotservices tests
	@$(MAKE) -C wifiequ tests
	@$(GATHER)

test: tests
	@$(MILESTONE) "Executing all tests..."
	@$(MAKE) -C libdmotservices test
	@$(MAKE) -C wifiequ test


#####################################################################################
## Examples
#####################################################################################
examples:
	@$(MILESTONE) "Compiling all examples..."
	@$(MAKE) -C libdmotservices examples
	@$(MAKE) -C wifiequ examples
	@$(GATHER)

example-demo: examples
	@$(MILESTONE) "Executing all examples and demos..."
	@$(MAKE) -C libdmotservices example-demo
	@$(MAKE) -C wifiequ example-demo


#####################################################################################
## Housekeeping
#####################################################################################
clean:
	@$(MILESTONE) "Removing previous builds..."
	@$(MAKE) -C libdmotservices clean || true
	@$(MAKE) -C wifiequ clean || true
	@rm -Rf "${DIST}"
