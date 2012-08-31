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

#ifndef __UNDERTHEHAT_H__
#define __UNDERTHEHAT_H__

#include "Minigame.h"

class BPMiniGame_UnderTheHat_Hat {
public:
	int X;
	int Y;
	int StartX;
	int StartY;
	int DestX;
	int DestY;
};




class BPMiniGame_UnderTheHat : public BPMiniGame {
public:
	BPMiniGame_UnderTheHat(BPGame* game);
	~BPMiniGame_UnderTheHat();
	void Start();
	int GetWeight();
	void Render();
	void Tick();
	void OnMouseDown();
	void OnMouseMove();
	void OnMouseUp();
	void MakeMove();
	void LevelUp();
	void GenerateMoves();
protected:
	Texture* sfcBackground;
	Texture* sfcBackground2;
	Texture* sfcHat;
	Texture* sfcBadHat;
	Texture* sfcItem;
		
	BPPList<BPMiniGame_UnderTheHat_Hat*> Hats;
	BPPList<BPPoint*> Points;
	
	BPMiniGame_UnderTheHat_Hat* ChosenHat;
	
	int CurrentLevel;
	SpriteFont* sfcCurrentLevel;
	int Tries;
	int MoveSpeed;
	int MovesLeft;
	int LastStateChange;
	
	float BackgroundFade;
	bool BackgroundUp;
	
	int NumWrong;
	int SuccessTime;
	int TimeStarted;
	
	float MoveAmount;
	
	MiniGameStates State;
};

#endif
