# Project: Darimasen-0.0.5
# Makefile created by Dev-C++ 4.9.9.1

CPP  = g++
CC   = gcc
OBJ  = src/main.o src/sidecon.o src/supplimental.o src/darimasen.o src/iconmodes.o src/dirtree.o
LINKOBJ  = src/main.o src/sidecon.o src/supplimental.o src/darimasen.o src/iconmodes.o src/dirtree.o
BIN  = darimasen
LIBS = `pkg-config gtkmm-2.4 --libs`
CFLAGS = `pkg-config gtkmm-2.4 --cflags`

.PHONY: all all-before all-after clean clean-custom

all: all-before darimasen all-after

clean: clean-custom
	rm -f $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o "darimasen" $(LIBS) 

src/main.o: src/main.cpp
	$(CPP) -c src/main.cpp -o src/main.o $(CFLAGS)

src/sidecon.o: src/sidecon.cpp
	$(CPP) -c src/sidecon.cpp -o src/sidecon.o $(CFLAGS)

src/supplimental.o: src/supplimental.cpp
	$(CPP) -c src/supplimental.cpp -o src/supplimental.o $(CFLAGS)

src/darimasen.o: src/darimasen.cpp
	$(CPP) -c src/darimasen.cpp -o src/darimasen.o $(CFLAGS)

src/iconmodes.o: src/iconmodes.cpp
	$(CPP) -c src/iconmodes.cpp -o src/iconmodes.o $(CFLAGS)

src/dirtree.o: src/dirtree.cpp
	$(CPP) -c src/dirtree.cpp -o src/dirtree.o $(CFLAGS)
