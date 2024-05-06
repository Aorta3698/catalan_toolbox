SRCS := main.cpp tree.cpp util.cpp dyck.cpp tri.cpp
H := ./headers
OBJS := main.o util.o tree.o dyck.o tri.o
DEPS := $(H)/xoshiro256.hpp $(H)/main.hpp $(H)/tree.hpp $(H)/tri.hpp $(H)/global.hpp $(H)/util.hpp $(H)/dyck.hpp
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

dyck.o: dyck.cpp $(H)/dyck.hpp $(H)/global.hpp
	$(CC) $(CFLAGS) $< -o $@

tri.o: tri.cpp $(H)/tri.hpp $(H)/global.hpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o tree
