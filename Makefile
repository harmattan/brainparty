MACHINE= $(shell uname -s)
OBJFILES := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

ifeq ($(MACHINE),Darwin)
	INCLUDES = -I/Library/Frameworks/SDL.framework/Headers -I/Library/Frameworks/SDL_image.framework/Headers -I/Library/Frameworks/SDL_mixer.framework/Headers -I/Library/Frameworks/SDL_ttf.framework/Headers -I/System/Library/Frameworks/OpenGL.framework/Headers
	LIBS = -m32 -framework SDL -framework SDL_image -framework SDL_mixer -framework SDL_ttf -framework Cocoa -framework OpenGL

	# SDL isn't in a great state on 64-bit Macs, so force 32-bit for now
	CXXFLAGS = -g -c -m32 -Wno-deprecated

	# to compile on OS X you need to include this Objective C file
	OSXCOMPAT = SDLMain.m
else
	INCLUDES = `sdl-config --cflags` -I/usr/X11R6/include
	LIBS = `sdl-config --libs` -lGLES_CM -lSDL_gles -lSDL_mixer -lSDL_ttf -lSDL_gfx -lSDL_image 
	CXXFLAGS = -O2 -c -Wno-deprecated
	OSXCOMPAT = 
endif

# object files have corresponding source files
CXX = g++

all: brainparty

brainparty: $(OBJFILES)
	$(CXX) -o brainparty $(INCLUDES) $(OSXCOMPAT) $(OBJFILES) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $<

clean:
	rm -f brainparty *.o
