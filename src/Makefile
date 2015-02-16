# Makefile for building fing native sdk

CC= g++

#add -std=gnu++0x for regex in C++
CXXFLAGS= -O2 -Wall

AR= ar rcu
RANLIB= ranlib
RM= rm -f

TARGET=flint-sdk

DEP=depend

CORE_FILES = $(wildcard *.cpp ./ssdp/*.cpp ./utils/*.cpp ./service/*.cpp ./ssdp/utils/* ./http/*.cpp)

CORE_OBJS = $(subst .cpp,.o,$(CORE_FILES))

INCLUDE = -I../include/ -I../include/ssdp/ -I../include/service -I../include/utils -I../include/ssdp/utils -I./http -I./http/include -I../include/flint/
LIBS= -lpthread

# Targets start here.
all: $(DEP) $(TARGET)

$(DEP):
	$(CC) $(INCLUDE) $(CXXFLAGS) -MM $(CORE_FILES)  > .depend

-include .depend

$(TARGET): $(CORE_OBJS)
	$(CC) $(INCLUDE) $(CXXFLAGS) -o $@ $^ $(LIBS)

$(CORE_OBJS): %.o: %.cpp
	$(CC) $(INCLUDE) -c $(CXXFLAGS) $< -o $@

.PHONY: clean
clean:
	@$(RM) -rf $(CORE_OBJS)
	@$(RM) -rf $(TARGET)
	@$(RM) -rf .depend


