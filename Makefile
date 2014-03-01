TARGET := brainparty

OBJFILES := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

PKGS := sdl2 SDL2_mixer SDL2_ttf SDL2_image audioresource egl glesv1_cm glib-2.0

CXXFLAGS += $(shell pkg-config --cflags $(PKGS))
LIBS += $(shell pkg-config --libs $(PKGS))

CXXFLAGS += -O2 -c -Wno-deprecated

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CXX) -o $@ $(OBJFILES) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TARGET) $(OBJFILES)
