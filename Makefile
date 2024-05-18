SRCS := main.cpp tree.cpp util.cpp dyck_pre.cpp poly.cpp chords.cpp arcs.cpp coin.cpp dyck_mirrored.cpp
H := ./headers
OBJS := main.o util.o tree.o dyck_pre.o poly.o chords.o arcs.o coin.o dyck_mirrored.o
DEPS := $(H)/xoshiro256.hpp $(H)/main.hpp $(H)/tree.hpp $(H)/poly.hpp $(H)/global.hpp $(H)/util.hpp $(H)/dyck_pre.hpp $(H)/dyck_mirrored.hpp $(H)/chords.hpp $(H)/arcs.hpp $(H)/coin.hpp $(H)/catalan.hpp
CFLAGS := -Wall -c -std=c++23 -O3 -lgmp
CC := g++ -Iheaders

## https://makefiletutorial.com/

all: $(OBJS)
	$(CC) -std=c++23 -O3 -Wall -lgmp $(OBJS) -o tree

main.o: main.cpp $(DEPS)
	$(CC) $(CFLAGS) $< -o $@

util.o: util.cpp $(H)/util.hpp
	$(CC) $(CFLAGS) $< -o $@

tree.o: tree.cpp $(H)/tree.hpp $(H)/global.hpp $(H)/catalan.hpp
	$(CC) $(CFLAGS) $< -o $@

dyck_pre.o: dyck_pre.cpp $(H)/dyck_pre.hpp $(H)/global.hpp $(H)/tree.hpp $(H)/catalan.hpp
	$(CC) $(CFLAGS) $< -o $@

poly.o: poly.cpp $(H)/poly.hpp $(H)/global.hpp $(H)/tree.hpp $(H)/catalan.hpp
	$(CC) $(CFLAGS) $< -o $@

chords.o: chords.cpp $(H)/chords.hpp $(H)/global.hpp $(H)/tree.hpp $(H)/catalan.hpp
	$(CC) $(CFLAGS) $< -o $@

arcs.o: arcs.cpp chords.cpp $(H)/arcs.hpp $(H)/global.hpp $(H)/tree.hpp $(H)/catalan.hpp
	$(CC) $(CFLAGS) $< -o $@

coin.o: coin.cpp dyck_pre.cpp $(H)/dyck_pre.hpp $(H)/global.hpp $(H)/tree.hpp $(H)/catalan.hpp
	$(CC) $(CFLAGS) $< -o $@

dyck_mirrored.o: dyck_mirrored.cpp $(H)/dyck_mirrored.hpp $(H)/global.hpp $(H)/tree.hpp $(H)/catalan.hpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o tree
