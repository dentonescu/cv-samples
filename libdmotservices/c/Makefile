#################################################
## libdmotservices/c/Makefile
#################################################

.PHONY: all clean example-demo linux prebuild test

all: prebuild linux

prebuild:
	
linux:
	@$(MAKE) -C linux all

tests:
	@$(MAKE) -C linux tests

test:
	@$(MAKE) -C linux test

examples:
	@$(MAKE) -C linux examples

example-demo:
	@$(MAKE) -C linux example-demo

clean:
	@$(MAKE) -C linux clean
