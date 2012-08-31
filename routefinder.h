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

#ifndef __ROUTEFINDER_H__
#define __ROUTEFINDER_H__

#include "Minigame.h"

enum MoveTypes { MT_UP, MT_DOWN, MT_LEFT, MT_RIGHT, MT_NONE };

class BPMiniGame_RouteFinder : public BPMiniGame {
public:
	BPMiniGame_RouteFinder(BPGame* game);
	~BPMiniGame_RouteFinder();
	void Start();
	int GetWeight();
	void Render();
	void LevelUp();
	void Tick();
	void CheckAnswer();
	void OnMouseUp();
	void OnMouseMove();
	void OnMouseDown();
	void AddMove();
	BPPoint* CalculateSheepPos();
protected:
	Texture* sfcBackground;
	Texture* sfcSheep;
	Texture* sfcRemember;
	
	Texture* sfcMoveUp;
	Texture* sfcMoveDown;
	Texture* sfcMoveLeft;
	Texture* sfcMoveRight;
		
	BPList<int> Moves;
	int CurrentMove;
	
	int CurrentLevel;
	int NumWrong;
	int NumTries;
	int SheepX;
	int SheepY;
	
	MiniGameStates GameState;
	int LastStateChange;
	BPPoint GuessedPosition;
	
	int TimeStarted;
};

#endif
