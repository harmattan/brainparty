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

#ifndef __SETFINDER_H__
#define __SETFINDER_H__

#include "Minigame.h"

enum FillTypes { EMPTY, STRIPED, FILLED };
enum ShapeTypes { CIRCLE, SQUARE, TRIANGLE };
enum ColourTypes { BLUE, GREEN, RED };

class BPMiniGame_SetFinder_Item {
public:
	ColourTypes Col;
	FillTypes FillType;
	ShapeTypes ShapeType;
	int X;
	int Y;
	bool Selected;
	
	int MatchTime;
	
	BPMiniGame_SetFinder_Item() {
		MatchTime = -1;
		Selected = false;
	}
};

class BPMiniGame_SetFinder : public BPMiniGame {
	Texture* sfcBackground;
	Texture* sfcSelected;
	Texture* sfcSelectedHi;
	
	BPPList<BPMiniGame_SetFinder_Item*> CurrentItems;
	
	Texture* sfcCircleBlueEmpty;
	Texture* sfcCircleBlueFilled;
	Texture* sfcCircleBlueStripy;
	Texture* sfcCircleGreenEmpty;
	Texture* sfcCircleGreenFilled;
	Texture* sfcCircleGreenStripy;
	Texture* sfcCircleRedEmpty;
	Texture* sfcCircleRedFilled;
	Texture* sfcCircleRedStripy;
	
	Texture* sfcSquareBlueEmpty;
	Texture* sfcSquareBlueFilled;
	Texture* sfcSquareBlueStripy;
	Texture* sfcSquareGreenEmpty;
	Texture* sfcSquareGreenFilled;
	Texture* sfcSquareGreenStripy;
	Texture* sfcSquareRedEmpty;
	Texture* sfcSquareRedFilled;
	Texture* sfcSquareRedStripy;
	
	Texture* sfcTriangleBlueEmpty;
	Texture* sfcTriangleBlueFilled;
	Texture* sfcTriangleBlueStripy;
	Texture* sfcTriangleGreenEmpty;
	Texture* sfcTriangleGreenFilled;
	Texture* sfcTriangleGreenStripy;
	Texture* sfcTriangleRedEmpty;
	Texture* sfcTriangleRedFilled;
	Texture* sfcTriangleRedStripy;
	
	int MatchCount;
	int SuccessTime;
	
	SpriteFont* sfcClock;
	int TimeStarted;
	
public:
	BPMiniGame_SetFinder(BPGame* game);
	~BPMiniGame_SetFinder();
	void Start();
	int GetWeight();
	void Render();
	void Tick();
	void CleanMatches();
	void OnMouseUp();
	void OnMouseMove();
	void OnMouseDown();
	void CheckForMatch();
	bool ItemsAreDifferent(BPMiniGame_SetFinder_Item* item1, BPMiniGame_SetFinder_Item* item2);
};

#endif
