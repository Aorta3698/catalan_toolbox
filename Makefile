SRCS := main.cpp tree.cpp util.cpp dyck.cpp tri.cpp
OBJS := main.o util.o tree.o dyck.o tri.o
DEPS := xoshiro256.hpp main.hpp tree.hpp tri.hpp global.hpp util.hpp dyck.hpp
CFLAGS := -Wall -c -std=c++23 -O3
CC := g++

## https://makefiletutorial.com/

all: $(OBJS)
	$(CC) -std=c++23 -O3 -Wall $(OBJS) -o tree

main.o: main.cpp $(DEPS)
	$(CC) $(CFLAGS) $< -o $@

util.o: util.cpp util.hpp
	$(CC) $(CFLAGS) $< -o $@

tree.o: tree.cpp tree.hpp global.hpp
	$(CC) $(CFLAGS) $< -o $@

dyck.o: dyck.cpp dyck.hpp global.hpp
	$(CC) $(CFLAGS) $< -o $@

tri.o: tri.cpp tri.hpp global.hpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o tree
