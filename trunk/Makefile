# Project: Darimasen-0.0.5
# Makefile created by Dev-C++ 4.9.9.1

CPP  = g++
CC   = gcc
OBJ  = src/main.o src/supplimental.o src/darimasen.o src/iconmodes.o src/dirtree.o src/mimehandler.o src/safestring.o 
LINKOBJ = $(OBJ)

BIN  = darimasen
#LIBS = `pkg-config gtkmm-2.4 --libs`
#CFLAGS = `pkg-config gtkmm-2.4 --cflags`
LIBS = `pkg-config gtkmm-2.4 gnome-vfsmm-2.6 --libs`
CFLAGS = `pkg-config gtkmm-2.4 gnome-vfsmm-2.6 --cflags`

.PHONY: all all-before all-after clean clean-custom

all: all-before darimasen all-after

clean: clean-custom
	rm -f $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o "darimasen" $(LIBS) 


.c.o:
	$(CPP) -c $< -o $@ $(CFLAGS)


.cpp.o: 
	$(CPP) -c $< -o $@ $(CFLAGS)

