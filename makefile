TARGET ?= brainparty
PLATFORM ?= desktop

DESTDIR ?=
PREFIX ?= /usr

SOURCES := $(wildcard src/*.cpp src/minigames/*.cpp)
OBJECTS := $(patsubst %.cpp,%.o,$(SOURCES))
PKGS := sdl2 SDL2_mixer SDL2_ttf SDL2_image

all: $(TARGET)

include platform/$(PLATFORM)/platform.mk

CXXFLAGS += $(shell pkg-config --cflags $(PKGS))
LIBS += $(shell pkg-config --libs $(PKGS))

CXXFLAGS += -O2 -Wno-deprecated
CXXFLAGS += -Isrc -Isrc/minigames -Iplatform/$(PLATFORM)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) $(LIBS)

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(OBJECTS)

distclean: clean
	rm -f $(TARGET)

.PHONY: all clean
.DEFAULT: all
