#ifndef BRAINPARTY_CONFIG_H
#define BRAINPARTY_CONFIG_H

#if defined(USE_OPENGL_ES)
#  include <GLES/gl.h>
#else
#  if defined(__APPLE__)
#    include <OpenGL/gl.h>
#  else
#    include <GL/gl.h>
#  endif
#  define glOrthof glOrtho
#endif

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h" 

#include <glib.h>

#endif /* BRAINPARTY_CONFIG_H */
