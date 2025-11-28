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
			find "${ROOT}" -path "*/build/linux" -not -path "*/node_modules*" -type d -exec cp -vR "{}/." "${DIST}" \; ; \
			find "${ROOT}" -name "bin" -not -path "*/node_modules*" -type d -exec cp -vR "{}/." "${DIST}" \; ; \
			find "${ROOT}" -name "*.tar.gz" -not -path "*/node_modules*" -type f -exec cp -v "{}" "${DIST}" \;

.PHONY: all build-all clean docs example-demo examples install test tests

#####################################################################################
## Default build
#####################################################################################
all: prebuild build-all dist

build-all:
	@$(MILESTONE) "Building all projects with build files..."
	@$(MAKE) -C libdmotservices clean all
	@$(MILESTONE) "Installing libdmotservices jars into local Maven repo..."
	@cd libdmotservices/java && mvn -DskipTests install
	@$(MAKE) -C slideshow-server dist
	@$(MAKE) -C wifiequ clean all
	@$(MAKE) -C txrxcli clean all
	@$(MILESTONE) "Building heapmonj (backend + frontend + docs)..."
	@$(MAKE) -C heapmonj clean all

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
	@$(MAKE) -C heapmonj docs

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
	@$(MAKE) -C txrxcli tests
	@$(MILESTONE) "Running heapmonj tests (backend + frontend)..."
	@$(MAKE) -C heapmonj test
	@$(GATHER)

test: tests
	@$(MILESTONE) "Executing all tests..."
	@$(MAKE) -C libdmotservices test
	@$(MAKE) -C slideshow-server test
	@$(MAKE) -C wifiequ test
	@$(MAKE) -C txrxcli test


#####################################################################################
## Examples
#####################################################################################
examples:
	@$(MILESTONE) "Compiling all examples..."
	@$(MAKE) -C libdmotservices examples
	@$(MAKE) -C wifiequ examples
	@$(MAKE) -C txrxcli examples
	@$(MAKE) -C heapmonj examples || true
	@$(GATHER)

example-demo: examples
	@$(MILESTONE) "Executing all examples and demos..."
	@$(MAKE) -C libdmotservices example-demo
	@$(MAKE) -C wifiequ example-demo
	@$(MAKE) -C txrxcli example-demo
	@$(MAKE) -C heapmonj example-demo


#####################################################################################
## Housekeeping
#####################################################################################
clean:
	@$(MILESTONE) "Removing previous builds..."
	@$(MAKE) -C libdmotservices clean || true
	@$(MAKE) -C slideshow-server clean || true
	@$(MAKE) -C wifiequ clean || true
	@$(MAKE) -C txrxcli clean || true
	@$(MAKE) -C heapmonj clean || true
	@rm -Rf "${DIST}"
