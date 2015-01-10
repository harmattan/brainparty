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

#ifndef __SHUFFLEPUZZLER_H__
#define __SHUFFLEPUZZLER_H__

#include "Minigame.h"

class BPMiniGame_ShufflePuzzler_Item {
public:
	int ActualPosition;
	int ProperPosition;
	Texture* Pic;
	
	BPMiniGame_ShufflePuzzler_Item() {
		ActualPosition = ProperPosition = 0;
		Pic = NULL;
	}
};



class BPMiniGame_ShufflePuzzler : public BPMiniGame {
public:
	BPMiniGame_ShufflePuzzler(BPGame* game);
	~BPMiniGame_ShufflePuzzler();
	void OnMouseUp();
	void OnMouseMove();
	void OnMouseDown();
	void SwapPiece(int pos);
	void TrySwap(int clicked, int pos1, int pos2, int pos3, int pos4);
	BPMiniGame_ShufflePuzzler_Item* FindImageWithPos(int pos);
	void Start();
	int GetWeight();
	void Render();
	void Tick();
	void Shuffle();
	
protected:	
	bool ShowResult;
	
	Texture* sfcBackground;
	Texture* sfcPicture;
	
	SpriteFont* sfcClock;
	
	int TimeStarted;
	
	int SuccessTime;
	bool Loaded;
	
	BPPList<Texture*> ImageBits;
	BPPList<BPMiniGame_ShufflePuzzler_Item*> Items;
};

#endif
