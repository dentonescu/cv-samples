#################################################
## libdmotservices/java/Makefile
#################################################

.PHONY: all clean dist example-demo test tests version

VERSION          := $(strip $(shell cat ../VERSION))
ANT              ?= ant
MVN              ?= mvn
JAVA_HOME        ?= /usr/lib/jvm/java-17-openjdk-amd64
ANT_BUILD_DIR    ?= $(CURDIR)/build-ant

ANT_ARGS = -f build.xml \
		-Dlibdmotservices.version="$(VERSION)" \
		-Dproject.java.home="$(JAVA_HOME)" \
		-Dmvn="$(MVN)" \
		-Dbuild="$(ANT_BUILD_DIR)" \
		-Dmvn.skip.tests="-DskipTests"

all: dist

version:
	@cd tools && $(JAVA_HOME)/bin/java VersionManager.java ../..

dist: version
	$(ANT) $(ANT_ARGS) dist

tests: test-compile

test-compile: version
	$(ANT) $(ANT_ARGS) test-compile

test: version
	$(ANT) $(ANT_ARGS) test

example-demo: version
	$(ANT) $(ANT_ARGS) example-demo

clean:
	$(ANT) $(ANT_ARGS) clean
