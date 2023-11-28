#
# src/Makefile
#
# Integrated makefile to build the RevCPU library
#
# Copyright (C) 2017-2021 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

ifeq (, $(shell which sst-config))
 $(error "No sst-config in $(PATH), add `sst-config` to your PATH")
endif

CXX=$(shell sst-config --CXX)
CXXFLAGS=$(shell sst-config --ELEMENT_CXXFLAGS)
LDFLAGS=$(shell sst-config --ELEMENT_LDFLAGS)
CPPFLAGS=-I./
OPTIMIZE_FLAGS=-O2

COMPONENT_LIB=libsstdata.so

BASICCOMPONENT_SOURCES := $(wildcard *.cc)

BASICCOMPONENT_HEADERS := $(wildcard *.h)

BASICCOMPONENT_OBJS := $(patsubst %.cc,%.o,$(wildcard *.cc))

BASICCOMPONENT_INCS := $(patsubst %.py,%.inc,$(wildcard *.py))

all: $(COMPONENT_LIB)

debug: CXXFLAGS += -DDEBUG -g -DENABLE_SSTDBG
debug: $(COMPONENT_LIB)

$(COMPONENT_LIB): $(BASICCOMPONENT_INCS) $(BASICCOMPONENT_OBJS)
	$(CXX) $(OPTIMIZE_FLAGS) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $@ *.o
%.o:%.cc $(SIMPLEEXAMPLE_HEADERS)
	$(CXX) $(OPTIMIZE_FLAGS) $(CXXFLAGS) $(CPPFLAGS) -c $<
%.inc:%.py
	od -v -t x1 < $< | sed -e 's/^[^ ]*[ ]*//g' -e '/^\s*$$/d' -e 's/\([0-9a-f]*\)[ $$]*/0x\1,/g' > $@
install: $(COMPONENT_LIB)
	sst-register sstdata sstdata_LIBDIR=$(CURDIR)
clean:
	rm -Rf *.o *.inc $(COMPONENT_LIB)

#-- EOF
