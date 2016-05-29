#
# Makefile
#

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -Werror -Wno-unused -g
LD = g++
INCLUDES =
LIBS = -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system -lconfig++
TARGET = shooter
OBJDIR = obj
OBJECTS = $(addprefix $(OBJDIR)/, main.o Entity.o Player.o \
	  Game.o Bullet.o Enemy.o Menu.o config.o keyMenu.o \
	  mapEditor.o vector.o mainMenu.o pauseMenu.o Particle.o \
	  Grenade.o Knife.o Magazine.o )

all: $(TARGET)

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(OBJDIR)/%.o : %.cpp | $(OBJDIR)
	$(CXX) -MT $@ -MMD -MP $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(LD) $(INCLUDES) $^ -o $@ $(LIBS)

clean:
	rm -f $(OBJECTS) $(TARGET) $(OBJDIR)/*.d

.PHONY: all clean

-include $(OBJECTS:.o=.d)
