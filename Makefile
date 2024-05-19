SRC_DIR := src
OBJ_DIR := obj

EXE := catalan_toolbox
SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

CPPFLAGS := -Iinclude -MMD -MP -std=c++23
CXXFLAGS := -Wall
LDFLAGS  := -Llib
LDLIBS   := -lgmp

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ)
	$(CXX) -std=c++23 $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(OBJ_DIR)

-include $(OBJ:.o=.d)

## 
# See: https://stackoverflow.com/questions/71457686/undefined-refenence-error-when-compiling-with-c11
# See: https://stackoverflow.com/questions/30573481/how-to-write-a-makefile-with-separate-source-and-header-directories
# See: https://www.gnu.org/software/make/manual/html_node/Implicit-Variables.html
# See: https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html


## -----------------
##SRCS := main.cpp tree.cpp util.cpp dyck_pre.cpp poly.cpp chords.cpp arcs.cpp coin.cpp dyck_mirrored.cpp
##H := ./includes
##OBJS := main.o util.o tree.o dyck_pre.o poly.o chords.o arcs.o coin.o dyck_mirrored.o
##DEPS := $(H)/xoshiro256.hpp $(H)/main.hpp $(H)/tree.hpp $(H)/poly.hpp $(H)/global.hpp $(H)/util.hpp $(H)/dyck_pre.hpp $(H)/dyck_mirrored.hpp $(H)/chords.hpp $(H)/arcs.hpp $(H)/coin.hpp $(H)/catalan.hpp
##CFLAGS := -Wall -c -std=c++23 -O3 -lgmp
##CC := g++ -Iincludes
##
#### https://makefiletutorial.com/
##
##all: $(OBJS)
	##$(CC) -std=c++23 -O3 -Wall -lgmp $(OBJS) -o catalan_toolbox
	##
##main.o: main.cpp $(DEPS)
	##$(CC) $(CFLAGS) $< -o $@
	##
##util.o: util.cpp $(H)/util.hpp
	##$(CC) $(CFLAGS) $< -o $@
	##
##tree.o: tree.cpp $(H)/tree.hpp $(H)/global.hpp $(H)/catalan.hpp
	##$(CC) $(CFLAGS) $< -o $@
	##
##dyck_pre.o: dyck_pre.cpp $(H)/dyck_pre.hpp $(H)/global.hpp $(H)/tree.hpp $(H)/catalan.hpp
	##$(CC) $(CFLAGS) $< -o $@
	##
##poly.o: poly.cpp $(H)/poly.hpp $(H)/global.hpp $(H)/tree.hpp $(H)/catalan.hpp
	##$(CC) $(CFLAGS) $< -o $@
	##
##chords.o: chords.cpp $(H)/chords.hpp $(H)/global.hpp $(H)/tree.hpp $(H)/catalan.hpp
	##$(CC) $(CFLAGS) $< -o $@
	##
##arcs.o: arcs.cpp chords.cpp $(H)/arcs.hpp $(H)/global.hpp $(H)/tree.hpp $(H)/catalan.hpp
	##$(CC) $(CFLAGS) $< -o $@
	##
##coin.o: coin.cpp dyck_pre.cpp $(H)/dyck_pre.hpp $(H)/global.hpp $(H)/tree.hpp $(H)/catalan.hpp
	##$(CC) $(CFLAGS) $< -o $@
	##
##dyck_mirrored.o: dyck_mirrored.cpp $(H)/dyck_mirrored.hpp $(H)/global.hpp $(H)/tree.hpp $(H)/catalan.hpp
	##$(CC) $(CFLAGS) $< -o $@
	##
##clean:
	##rm *.o catalan_toolbox
