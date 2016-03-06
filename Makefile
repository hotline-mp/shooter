#
# Makefile
#

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -Werror -Wno-unused -g
LD = g++
INCLUDES =
LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lconfig++
TARGET = shooter
OBJECTS = main.o Entity.o Player.o Game.o Menu.o config.o

%.o : %.cpp
	$(CXX) -MT $@ -MMD -MP $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(LD) $(INCLUDES) $^ -o $@ $(LIBS)


.PHONY: all clean

clean:
	rm -f $(OBJECTS) $(TARGET) *.d

all: $(TARGET)

-include $(OBJECTS:.o=.d)
