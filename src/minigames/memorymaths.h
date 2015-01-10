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

#ifndef __MEMORYMATHS__
#define __MEMORYMATHS__

#include "Minigame.h"

class BPMiniGame_MemoryMaths : public BPMiniGame {
public:
	BPMiniGame_MemoryMaths(BPGame* game);
	~BPMiniGame_MemoryMaths();
	void OnMouseDown();
	void OnMouseMove();
	void OnMouseUp();
	void ButtonClicked(const char* btn);
	void FlipSign();
	void CheckAnswer();
	void LevelUp();
	void AddFood();
	void AddOp();
	void Solve();
	void Start();
	int GetWeight();
	void Render();
	void Tick();
	void AnswerChanged();
protected:
	Texture* sfcBackground;
	Texture* sfcRemember;
		
	int TimeStarted;
	
	MiniGameStates GameState;
	
	BPPList<Texture*> Food;
	BPList<int> Numbers;
	BPList<int> FoodNumbers;
	BPPList<SpriteFont*> sfcNumbers;
	
	BPList<int> EquationFood;
	BPList<const char*> EquationOps;
	int LastOp; // store the last operation used, so we always use a different one
	
	int Result;
	int NumWrong;
	int NumTries;
	
	int CurrentLevel;
	int LastStateChange;
	int SuccessTime;
	
	bool LastWrong; // when set to be true, re-show the "Remember" screen regardless of the score and don't increment the level counter
	
	string* CurrentAnswer;
	SpriteFont* sfcCurrentAnswer;
	
	map<const char*, SpriteFont*> sfcOperators;
};

#endif
