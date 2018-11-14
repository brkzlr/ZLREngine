##### Makefile for my test C++ codes. Berk Ozler

### Variables

# GCC for C, G++ for C++
CC=g++
CFLAGS= -g -pedantic-errors

# Bin release path
EXE=bin/release

# Build (object files)
OBJDIR=build
OBJ=$(SRC:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

# Libraries
LIBDIR=lib
# LIB=$(wildcard $(LIBDIR)/*.a)
LIB= -ldl -lSDL2

# Sources
SRCDIR=src
SRC=$(wildcard $(SRCDIR)/*.cpp)

# Headers
INCDIR=include

### Targets

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) $^ $(LIB) -I $(INCDIR) -o $@
	
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -I $(INCDIR) -o $@
	
clean:
	rm -rf $(EXE) $(OBJDIR)/*
