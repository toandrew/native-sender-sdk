# Makefile for fling native Sender SDK

# Targets start here.
mytarget: all
.PHONY: mytarget clean all

all clean:
	@cd src && $(MAKE) $@
