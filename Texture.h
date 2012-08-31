// Brain Party
// Copyright (C) 2010 Paul Hudson (http://www.tuxradar.com/brainparty)

// Brain Party is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <string>
#include <cmath>
#include <iostream>

#include <unistd.h>
#include "SDL.h"

#include <SDL_gles.h>
#include <GLES/gl.h>

#include "Colour.h"

using namespace std;

class Texture {
public:
	GLuint Surface;
	float Width;
	float Height;
	float HalfWidth;
	float HalfHeight;
	float WidthRatio;
	float HeightRatio;
	
	GLfloat TexCoords[8];
	GLfloat Vertices[12];
	
	Texture(SDL_Surface* surface);
	Texture(const char* filename, float actualwidth, float actualheight);
	~Texture();
	void InitWithSurface(SDL_Surface* surface, float actualwidth, float actualheight);
	void Draw(float x, float y);
	void Draw(float x, float y, Colour &col);
	void Draw(float x, float y, float width, float height);
	void Draw(float x, float y, float rotation, float scale, Colour &col);
	static int NextPO2(int num);
};

#endif
