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

#ifndef __SPRITEFONT_H__
#define __SPRITEFONT_H__

enum Alignments { CENTRED, LEFT, RIGHT };

#include "BPGame.h"
#include "BPList.h"
#include "Texture.h"

class SpriteFont {
public:
	BPPList<Texture*> Surfaces;
	int Width;
	int Height;
	Alignments Align;
	
	SpriteFont();
	~SpriteFont();
	void drawAtPoint(float x, float y);
};

#endif
