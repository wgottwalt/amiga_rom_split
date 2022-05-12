CXX = $(CROSS_COMPILE)g++

BINARIES = AmigaRomSplit
ASOURCES = $(wildcard *.S)
CSOURCES = $(wildcard *.c)
CXXSOURCES = $(wildcard *.cxx)
OBJECTS = $(ASOURCES:.S=.o) $(CSOURCES:.c=.o) $(CXXSOURCES:.cxx=.o)

ARCH_FLAGS = -pthread -flto -Os -s
FLAGS = -W -Wall -Wextra
CXXFLAGS = -std=c++17 $(ARCH_FLAGS) $(FLAGS)

all: $(BINARIES)

%.o: %.cxx
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(BINARIES): %: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(filter $@.o,$(OBJECTS)) $(filter-out $(BINARIES:=.o),$(OBJECTS))

clean:
	$(RM) *.o

distclean: clean
	$(RM) $(BINARIES)

.PHONY: all clean distclean
