IDIR=include
BDIR=bin

SUFFIXES += .d
NODEPS:=clean tags svn
SOURCES:=$(shell find src/ -name "*.cpp")
DEPFILES:=$(patsubst %.cpp,%.d,$(SOURCES))

CXX=g++
CXXFLAGS=-I$(IDIR) -Wshadow -Wall -Wextra -DASIO_STANDALONE -std=c++17
OFLAGS=-O3 -march=native -flto 
LIBS=-lsfml-graphics -lsfml-window -lsfml-system

ODIR=obj

OBJS:=$(patsubst %.cpp, obj/%.o, $(shell find src/ -name '*.cpp' -type f -printf "%f "))

lsystem-test: $(OBJS)
	$(CXX) -o $(BDIR)/$@ $^ $(CXXFLAGS) $(LIBS)

include Rulefile

.PHONY: clean

doc:
	doxygen docs/Doxyfile

release: clean $(OBJS)
	clean
	$(CXX) -o $(BDIR)/ensicoin-node $(CXXFLAGS) $(OFLAGS) $^ $(LIBS)

clean:
	rm -f $(ODIR)/*.o $(BDIR)/*
