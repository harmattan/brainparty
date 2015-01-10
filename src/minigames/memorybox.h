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

#ifndef __MEMORYBOX_H__
#define __MEMORYBOX_H__

#include "Minigame.h"

class BPMiniGame_MemoryBox_Box {
public:
	int Number;
	int X;
	int Y;
	bool Showing;
	
	BPMiniGame_MemoryBox_Box() {
		Number = X = Y = 0;
		Showing = false;
	}
};


class BPMiniGame_MemoryBox : public BPMiniGame {
public:
	BPMiniGame_MemoryBox(BPGame* game);
	~BPMiniGame_MemoryBox();
	void Start();
	int GetWeight();
	void Render();
	void Tick();
	void OnMouseUp();
	void OnMouseDown();
	void OnMouseMove();
	void SetGameState(MiniGameStates gs);
	void GenerateBoxes();
	
protected:
	Texture* sfcBackground;
	
	map<int, Texture*> Numbers;
	Texture* sfcUnknownBox;
	
	map<int, BPList<BPPoint> > Layouts;
	BPPList<BPMiniGame_MemoryBox_Box*> ActiveBoxes;
	
	MiniGameStates GameState;
	int LastStateChange;
	float Level;
	int OriginalLowestNumber; // the lowest number of all the boxes on the screen; this doesn't change for any given sequence of boxes, which is important because boxes are re-shown when the player makes a mistake
	int LowestNumber;
	int ShowDelay;
	
	int TimeStarted;
	int NumWrong;
	int NumTries;
};

#endif
