SRCS := main.cpp tree.cpp util.cpp dyck.cpp poly.cpp chords.cpp arcs.cpp coin.cpp
H := ./headers
OBJS := main.o util.o tree.o dyck.o poly.o chords.o arcs.o coin.cpp
DEPS := $(H)/xoshiro256.hpp $(H)/main.hpp $(H)/tree.hpp $(H)/poly.hpp $(H)/global.hpp $(H)/util.hpp $(H)/dyck.hpp $(H)/chords.hpp $(H)/arcs.hpp $(H)/coin.hpp
CFLAGS := -Wall -c -std=c++23 -O3
CC := g++ -Iheaders

## https://makefiletutorial.com/

all: $(OBJS)
	$(CC) -std=c++23 -O3 -Wall $(OBJS) -o tree

main.o: main.cpp $(DEPS)
	$(CC) $(CFLAGS) $< -o $@

util.o: util.cpp $(H)/util.hpp
	$(CC) $(CFLAGS) $< -o $@

tree.o: tree.cpp $(H)/tree.hpp $(H)/global.hpp
	$(CC) $(CFLAGS) $< -o $@

dyck.o: dyck.cpp $(H)/dyck.hpp $(H)/global.hpp $(H)/tree.hpp
	$(CC) $(CFLAGS) $< -o $@

poly.o: poly.cpp $(H)/poly.hpp $(H)/global.hpp $(H)/tree.hpp
	$(CC) $(CFLAGS) $< -o $@

chords.o: chords.cpp $(H)/chords.hpp $(H)/global.hpp $(H)/tree.hpp
	$(CC) $(CFLAGS) $< -o $@

arcs.o: arcs.cpp chords.cpp $(H)/arcs.hpp $(H)/global.hpp $(H)/tree.hpp
	$(CC) $(CFLAGS) $< -o $@

coin.o: coin.cpp dyck.cpp $(H)/dyck.hpp $(H)/global.hpp $(H)/tree.hpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o tree
