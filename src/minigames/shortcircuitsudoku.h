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

#ifndef __SHORTCIRCUITSUDOKU_H__
#define __SHORTCIRCUITSUDOKU_H__

#include "Minigame.h"

class SudokuSquare {
public:
	int Across;
	int Down;
	int Region;
	int Value;
	int Index;
	
	int XPos;
	int YPos;
	
	SpriteFont* sfcStrValue;
	
	bool Showing;
	
	SudokuSquare() {
		Across = Down = Region = Value = Index = XPos = YPos = 0;
		sfcStrValue = NULL;
		Showing = true;
	}
	
	~SudokuSquare() {
		SAFE_DELETE(sfcStrValue);
	}
};


class BPMiniGame_ShortCircuitSudoku : public BPMiniGame {
public:
	BPMiniGame_ShortCircuitSudoku(BPGame* game);
	~BPMiniGame_ShortCircuitSudoku();
	void Start();
	int GetWeight();
	void Render();
	void Tick();
	void OnMouseDown();
	void OnMouseMove();
	void OnMouseUp();
	void LevelUp();
	void GenerateGrid();
	bool Conflicts(SudokuSquare** CurrentValues, SudokuSquare* test);
	SudokuSquare* Item(int n, int v);
	int GetAcrossFromNumber(int n);
	int GetDownFromNumber(int n);
	int GetRegionFromNumber(int n);
	void SubmitAnswer(int Answer);
	void SetMarathon();
		
protected:
	Texture* sfcBackground;
	
	SpriteFont* sfcQuestionMark;
	
	int LastStateChange;
	MiniGameStates GameState;
	int NumTries;
	int CurrentLevel;
	
	SudokuSquare* GuessSquare;
	
	int Score;
	
	int TimeStarted;
	BPPList<SudokuSquare*> Sudoku;
};

#endif
