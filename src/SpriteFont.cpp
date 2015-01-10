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

#include "SpriteFont.h"

SpriteFont::SpriteFont() {

}

SpriteFont::~SpriteFont() {
	Surfaces.Clear();
}

void SpriteFont::drawAtPoint(float x, float y) {
	int height = 0;
	
	for (int i = 0; i < Surfaces.Count; ++i) {
		Texture* tex = Surfaces[i];
		
		switch (Align) {
			case LEFT:
				tex->Draw(x, y + height);
				break;
			
			case CENTRED:
				tex->Draw(x + (Width / 2.0f) - (tex->HalfWidth), y + height);
				break;
			
			case RIGHT:
				tex->Draw(x + Width - tex->Width, y + height);
				break;
		}

		height += tex->Height;
	}
}
