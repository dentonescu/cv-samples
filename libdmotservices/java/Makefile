#################################################
## libdmotservices/java/Makefile
#################################################

.PHONY: all clean dist example-demo test tests

VERSION          := $(strip $(shell cat ../VERSION))
ANT              ?= ant
MVN              ?= mvn
JAVA_HOME        ?= /usr/lib/jvm/java-17-openjdk-amd64
ANT_BUILD_DIR    ?= $(CURDIR)/build-ant

ANT_ARGS = -f build.xml \
		-Dlibdmotservices.version="$(VERSION)" \
		-Dproject.java.home="$(JAVA_HOME)" \
		-Dmvn="$(MVN)" \
		-Dbuild="$(ANT_BUILD_DIR)"

all: dist

dist:
	$(ANT) $(ANT_ARGS) dist

tests: test-compile

test-compile:
	$(ANT) $(ANT_ARGS) test-compile

test:
	$(ANT) $(ANT_ARGS) test

example-demo:
	$(ANT) $(ANT_ARGS) example-demo

clean:
	$(ANT) $(ANT_ARGS) clean
