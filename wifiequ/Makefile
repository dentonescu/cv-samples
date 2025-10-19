#################################################
## WiFiEqu/Makefile
#################################################

.PHONY: all angular clean install example-demo examples linux prebuild setup-user test tests windows

# build prerequisites
VERSION := $(strip $(shell cat VERSION))

export VERSION

# locations
DIST := "dist"
ROOT := $(strip $(shell pwd))

# commands
MILESTONE := echo === 
GATHER := 	find "${ROOT}" -name "*.a" -type f -exec cp -v "{}" "${DIST}" \; ; \
			find "${ROOT}" -name "build" -type d -exec cp -vR "{}/." "${DIST}" \; ; \
			find "${ROOT}" -name "bin" -type d -exec cp -vR "{}/." "${DIST}" \;


#####################################################################################
## Default build
#####################################################################################
all: prebuild version linux dist

dist:
	@$(MILESTONE) "Populating the distribution directory..."
	@mkdir -p "${DIST}"
	@$(GATHER)

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
	@$(GATHER)

test: tests
	@$(MILESTONE) "Executing all tests..."
	@$(MAKE) -C linux test


#####################################################################################
## Examples
#####################################################################################
examples:
	@$(MILESTONE) "Compiling all examples..."
	@$(MAKE) -C linux examples
	@$(GATHER)

example-demo: examples
	@$(MILESTONE) "Executing all examples and demos..."
	@$(MAKE) -C linux example-demo

#####################################################################################
## housekeeping
#####################################################################################
clean:
	@$(MILESTONE) "Removing previous builds..."
	@$(MAKE) -C linux clean || true
	@rm -Rf "${DIST}"
