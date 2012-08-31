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

#ifndef __FLASHCOUNTING_H__
#define __FLASHCOUNTING_H__

#include "Minigame.h"

class BPMiniGame_FlashCounting_Ball {
public:
	double X;
	double Y;
	int Direction;
	int Type;
	
	BPMiniGame_FlashCounting_Ball() {
		X = 0;
		Y = 0;
	}
};

class BPMiniGame_FlashCounting : public BPMiniGame {
public:
	void CreateBall(int type);
	void LevelUp();
	void Tick();
	void Render();
	int GetWeight();
	void Start();
	void OnMouseUp();
	void OnMouseDown();
	void OnMouseMove();
	BPMiniGame_FlashCounting(BPGame* game);
	~BPMiniGame_FlashCounting();
protected:
	Texture* sfcBackground;
	int CurrentLevel;
	
	BPPList<BPMiniGame_FlashCounting_Ball*>* Balls;
	BPPList<Texture*>* BallPics;
	BPList<int>* BallIDs;
	BPList<int>* Directions;
	
	static const int Top = 98;
	static const int Bottom = 418;
	static const int BallWidth = 64;
	static const int BallHeight = 64;
	
	int Answer;
	int BallDirCount; // used to assign random directions to balls
	
	int LastStateChange;
	MiniGameStates GameState;
	
	int NumTries;
	int TimeStarted;
	int NumWrong;
};

#endif
