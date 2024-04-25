SRCS := main.cpp
OBJS := main.o
DEPS := xoshiro256.hpp main.hpp
CFLAGS := -Wall -c -std=c++23 -O3
CC := g++

## https://makefiletutorial.com/

all: $(OBJS)
	$(CC) -std=c++23 -O3 -Wall $(OBJS) -o tree

main.o: main.cpp $(DEPS)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o tree
