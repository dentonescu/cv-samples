#################################################
## libdmotservices/Makefile
#################################################

# build prerequisites
ANT ?= $(strip $(shell which ant))
GIT ?= $(strip $(shell which git))
MVN ?= $(strip $(shell which mvn))
VERSION := $(strip $(shell cat VERSION))

export ANT
export GIT
export MVN
export VERSION

# locations
DIST := "dist"
ROOT := $(strip $(shell pwd))

# commands
JAVA_BUILD ?= $(ANT) -Dant=$(ANT) -Dgit=$(GIT) -Dmvn=$(MVN) -f build.xml -Dlibdmotservices.version="$(VERSION)"
MILESTONE := echo === 
GATHER := 	find "${ROOT}" -name "*.a" -type f -exec cp -v "{}" "${DIST}" \; ; \
			find "${ROOT}" -name "build" -type d -exec cp -vR "{}/." "${DIST}" \; ; \
			find "${ROOT}" -name "bin" -type d -exec cp -vR "{}/." "${DIST}" \;

.PHONY: all c clean example-demo examples java test tests version


#####################################################################################
## Default build
#####################################################################################
all: prebuild version c java dist

dist:
	@$(MILESTONE) "Populating the distribution directory..."
	@mkdir -p "${DIST}"
	@$(GATHER)

prebuild:
	@cat libdmotservices.logo


#####################################################################################
## Version management
#####################################################################################
version:
	@$(MILESTONE) "Generating version data..."
	@echo "VERSION: $(VERSION)"
	@./gen-version.py >/dev/null


#####################################################################################
## C
#####################################################################################
c:
	@$(MILESTONE) "Building the C library..."
	@$(MAKE) -C c all


#####################################################################################
## Java
#####################################################################################
java:
	@$(MILESTONE) "Building the Java library..."
	@if [ -f "java/Makefile" ]; then \
		$(MAKE) -C java all; \
	elif [ -f "java/build.xml" ]; then \
		cd java && $(JAVA_BUILD); \
	fi


#####################################################################################
## Unit tests
#####################################################################################
tests:
	@$(MILESTONE) "Compiling all tests..."
	@$(MAKE) -C c tests
	@if [ -f "java/Makefile" ]; then \
		$(MAKE) -C java tests; \
	elif [ -f "java/build.xml" ]; then \
		cd java && $(JAVA_BUILD) test-compile; \
	fi
	@$(GATHER)

test: tests
	@$(MILESTONE) "Executing all tests..."
	@$(MAKE) -C c test
	@if [ -f "java/Makefile" ]; then \
		$(MAKE) -C java test; \
	elif [ -f "java/build.xml" ]; then \
		cd java && $(JAVA_BUILD) test; \
	fi


#####################################################################################
## Examples
#####################################################################################
examples:
	@$(MILESTONE) "Compiling all examples..."
	@$(MAKE) -C c examples
	@echo "Skipping examples target for Java. Not relevant."
	@$(GATHER)

example-demo: examples
	@$(MILESTONE) "Executing all examples and demos..."
	@$(MAKE) -C c example-demo
	@echo "Skipping example-demo target for Java. Not relevant."


#####################################################################################
## housekeeping
#####################################################################################
clean:
	@$(MILESTONE) "Removing previous builds..."
	@$(MAKE) -C c clean || true
	@(cd java && $(JAVA_BUILD) clean) || true
	@rm -Rf "${DIST}"
