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

#ifndef __PERFECTPATHS_H__
#define __PERFECTPATHS_H__

#include "Minigame.h"

class BPMiniGame_PerfectPaths_Square {
public:
	int X;
	int Y;
	int XPos; // XPos and YPos are pixel co-ordinates to save calculations when drawing
	int YPos;
	Colour* Col;
	int Difficulty;
	const char* DifficultyStr;
	
	int MoveCalc; // used for flood fill best move calculation; set high so that any path is better
	int Pos; // the position of the square in the Squares list; saves looking it up when doing flood fills
	
	BPMiniGame_PerfectPaths_Square() {
		X = Y = XPos = YPos = Difficulty = Pos = 0;
		MoveCalc = 9999;
		DifficultyStr = NULL;
	}
	
	~BPMiniGame_PerfectPaths_Square() {

	}
};



class BPMiniGame_PerfectPaths : public BPMiniGame {
public:
	BPMiniGame_PerfectPaths(BPGame* game);
	~BPMiniGame_PerfectPaths();
	void Start();
	int GetWeight();
	void Render();
	void Tick();
	void OnMouseUp();
	void OnMouseMove();
	void OnMouseDown();
	void CalculateOurScore();
	void CheckResult();
	void LevelUp();
	void CalculateBestMove();
	void FloodFill(BPMiniGame_PerfectPaths_Square* square);
	bool CanMove(BPMiniGame_PerfectPaths_Square* square1, BPMiniGame_PerfectPaths_Square* square2);
	void SetScore();
	void RenderPerfect();
	void RenderOK();
protected:	
	Texture* sfcBackground;
	
	Texture* sfcPerfect;
	Texture* sfcOK;
	
	BPList<int> StartPositions;
	BPList<int> EndPositions;
	BPPList<Texture*> LoNumbers;
	BPPList<Texture*> HiNumbers;
	
	BPMiniGame_PerfectPaths_Square* StartPos;
	BPMiniGame_PerfectPaths_Square* EndPos;
	
	BPPList<BPMiniGame_PerfectPaths_Square*> Squares;
	BPList<BPMiniGame_PerfectPaths_Square*> Moves;
	BPList<BPMiniGame_PerfectPaths_Square*> MoveSquares; // used for flood fill best move calculation
	
	int CurrentScore;
	int BestScore;
	
	SpriteFont* sfcScore;
	
	int LastDiff;
	int TotalDiff;
	
	int TimeStarted;
	int CurrentLevel;
	
	MiniGameStates GameState;
	int LastStateChange;
};

#endif
