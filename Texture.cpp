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

#include "Texture.h"
#include "SDL_image.h"

Texture::Texture(SDL_Surface* surface) {	
	int po2width = Texture::NextPO2(surface->w);
	int po2height = Texture::NextPO2(surface->h);
	
	if (Width != po2width or Height != po2height) {
		SDL_Surface* tmpsurface = SDL_CreateRGBSurface(SDL_SWSURFACE, po2width, po2height, 32,
		#if SDL_BYTEORDER == SDL_LIL_ENDIAN
			0x000000FF, 
			0x0000FF00, 
			0x00FF0000, 
			0xFF000000
		#else
			0xFF000000,
			0x00FF0000, 
			0x0000FF00, 
			0x000000FF
		#endif		
		);
		
		SDL_SetAlpha(surface, 0, 0);	
		SDL_BlitSurface(surface, NULL, tmpsurface, NULL);
		InitWithSurface(tmpsurface, surface->w, surface->h);
		SDL_FreeSurface(tmpsurface);		
	} else {
		InitWithSurface(surface, surface->w, surface->h);
	}
}

Texture::Texture(const char* filename, float actualwidth, float actualheight) {
	string file = filename;
	file.insert(0, "/opt/brainparty/Content/");
	file.append(".png");

	SDL_Surface *surface = IMG_Load(file.c_str());
	if ((surface->w & (surface->w - 1)) != 0) printf("warning: %s's width is not a power of 2\n", file.c_str());
	if ((surface->h & (surface->h - 1)) != 0) printf("warning: %s's height is not a power of 2\n", file.c_str());
	
	InitWithSurface(surface, actualwidth, actualheight);
	
	SDL_FreeSurface(surface);
}

void Texture::InitWithSurface(SDL_Surface* surface, float actualwidth, float actualheight) {		
	Width = surface->w;
	Height = surface->h;
	
	GLenum texture_format;
	int colors = surface->format->BytesPerPixel;

        if (colors == 4) {
                if (surface->format->Rmask == 0x000000ff) {
                        texture_format = GL_RGBA;
                } else {
                        cout << "BGRA unsupported :(" << endl;
                        texture_format = GL_RGBA;
                        //texture_format = GL_BGRA;
		}
        } else if (colors == 3) {
                if (surface->format->Rmask == 0x000000ff) {
                        texture_format = GL_RGB;
                } else {
                        cout << "BGR unsupported :(" << endl;
                        texture_format = GL_RGB;
                        //texture_format = GL_BGR;
		}
        } else {
		// this shouldn't ever happen
		cout << "Problem loading texture!" << endl;
	}

	glGenTextures(1, &this->Surface);
	
	glBindTexture(GL_TEXTURE_2D, Surface);
	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glTexImage2D(GL_TEXTURE_2D, 0, texture_format, Width, Height, 0, texture_format, GL_UNSIGNED_BYTE, surface->pixels);
		
	WidthRatio = actualwidth / Width;
	HeightRatio = actualheight / Height;
	
	// now store the real width and height away to be used later.
	Width = actualwidth;
	Height = actualheight;
	HalfWidth = actualwidth / 2;
	HalfHeight = actualheight / 2;
	
	TexCoords[0] = 0.0;
	TexCoords[1] = HeightRatio;
	
	TexCoords[2] = WidthRatio;
	TexCoords[3] = HeightRatio;
	
	TexCoords[4] = 0.0;
	TexCoords[5] = 0.0;
	
	TexCoords[6] = WidthRatio;
	TexCoords[7] = 0.0;
	
	Vertices[0] = 0.0f;
	Vertices[1] = actualheight;
	Vertices[2] = 0.0f;
	
	Vertices[3] = actualwidth;
	Vertices[4] = actualheight;
	Vertices[5] = 0.0f;
	
	Vertices[6] = 0.0f;
	Vertices[7] = 0.0f;
	Vertices[8] = 0.0f;
	
	Vertices[9] = actualwidth;
	Vertices[10] = 0.0f;
	Vertices[11] = 0.0f;
}

Texture::~Texture() {
	glDeleteTextures(1, &this->Surface);
}

void Texture::Draw(float x, float y) {
	if (y + Height < 0) return;
	if (y > 480) return;
		
	glBindTexture(GL_TEXTURE_2D, Surface);
	
	glLoadIdentity();
	
	glTranslatef(x, y, 0.0f);
	
	glVertexPointer(3, GL_FLOAT, 0, Vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, TexCoords);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Texture::Draw(float x, float y, Colour &col) {
	glBindTexture(GL_TEXTURE_2D, Surface);
	
	glLoadIdentity(); 
	
	glTranslatef(x, y, 0.0f);
	glColor4f(col.R, col.G, col.B, col.A);
	
	glVertexPointer(3, GL_FLOAT, 0, Vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, TexCoords);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void Texture::Draw(float x, float y, float width, float height) {
	glBindTexture(GL_TEXTURE_2D, Surface);
	
	glLoadIdentity(); 
	glTranslatef(x, y, 0.0f);
	
	float temp_vertices[] = { 0.0f, height, 0.0f,
					width, height, 0.0f,
					0.0f, 0.0f, 0.0f,
					width, 0.0f, 0.0f
	};
	
	glVertexPointer(3, GL_FLOAT, 0, temp_vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, TexCoords);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); 	
}

void Texture::Draw(float x, float y, float rotation, float scale, Colour &col) {
	float texwidth = Width * scale;
	float texheight = Height * scale;
	
	float originx = texwidth / 2;
	float originy = texheight / 2;
	
	glBindTexture(GL_TEXTURE_2D, Surface);
	
	glLoadIdentity(); 
	
	glTranslatef(x, y, 0.0f);
	glRotatef(rotation, 0, 0, 1);
	glColor4f(col.R, col.G, col.B, col.A);

	float temp_vertices[] = {
		-originx, texheight - originy, 0.0f,
		texwidth - originx, texheight - originy, 0.0f,
		-originx, -originy, 0.0f,
		texwidth - originx, -originy, 0.0f
	};
	
	glVertexPointer(3, GL_FLOAT, 0, temp_vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, TexCoords);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

int Texture::NextPO2(int num) {
	int value = 1;

	while (value < num) {
		value <<= 1;
	}
	return value;
}
