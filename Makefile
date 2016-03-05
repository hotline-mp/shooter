#
# Makefile
#

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -Werror -Wno-unused -g
LD = g++
INCLUDES =
LIBS = -lsfml-graphics -lsfml-window -lsfml-system
TARGET = shooter
OBJECTS = main.o Entity.o Player.o Game.o

%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(LD) $(INCLUDES) $^ -o $@ $(LIBS)


.PHONY: all clean

clean:
	rm -f $(OBJECTS) $(TARGET)

all: $(TARGET)

