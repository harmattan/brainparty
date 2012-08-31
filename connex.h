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

#ifndef __CONNEX_H__
#define __CONNEX_H__

#include "Minigame.h"

class BPMiniGame_Connex_Object {
public:
	int X;
	int Y;
	const char* Text;
	SpriteFont* sfcText;
	
	BPMiniGame_Connex_Object() {
		Text = NULL;
		sfcText = NULL;
	}
	
	~BPMiniGame_Connex_Object() {
		SAFE_DELETE(sfcText);
	}
};


class BPMiniGame_Connex : public BPMiniGame {
public:
	BPMiniGame_Connex(BPGame* game);
	~BPMiniGame_Connex();
	void Start();
	int GetWeight();
	void Render();
	void Tick();
	void OnMouseMove();
	void OnMouseDown();
	void OnMouseUp();
	int GetLetter(BPPoint e);
	
protected:
	Texture* sfcBackground;
	Texture* sfcCard;
	
	BPList<const char*>* Letters;
	BPPList<BPMiniGame_Connex_Object*>* Objects;

	int CompleteLetters;
	int LastLetter;
	int TimeStarted;
};

#endif
