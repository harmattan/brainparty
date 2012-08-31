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

#ifndef __DICEOFF_H__
#define __DICEOFF_H__

#include "Minigame.h"

enum Ownership { OWNER_NONE, OWNER_PLAYER, OWNER_AI };

class BPMiniGame_DiceOff_Die {
public:
	int Value;
	int NumNeighbours;
	
	Ownership Owner;
	BPPoint Pos;
	
	int Index;
	int X;
	int Y;
	
	Colour Col;
	int LastStateChange;
	
	BPMiniGame_DiceOff_Die() {
		Owner = OWNER_NONE;
		LastStateChange = -5000;
	}
};



class BPMiniGame_DiceOff : public BPMiniGame {
	static const int NumRows = 8;
	static const int NumCols = 6;
	static const int DiceSize = 50;
	static const int ChainSpeed = 150;
	static const int ChangeSpeed = 300;
	static const int AIThinkSpeed = 500;
	
	bool OurTurn;
	bool FirstAITurn; // used to stop the AI from choosing edge squares in its first turn, as this can be fatal if they player started next to them!
	
	Colour NeutralColor;
	Colour PlayerColor;
	Colour AIColor;

	BPPList<Texture*> DicePics;
	
	BPPList<BPMiniGame_DiceOff_Die*> Dice;
	BPList<BPMiniGame_DiceOff_Die*> AIClosedList; // this must be a BPList otherwise we delete dice in the Dice list!
	BPList<BPMiniGame_DiceOff_Die*> CheckChangeList; // this must be a BPList otherwise we delete dice in the Dice list!
	
	bool PlaySound;
	
	int ChangeStartTime; // allows us to kill changes if they enter a loop
	int LastStateChange;
	MiniGameStates State;
	
public:
	BPMiniGame_DiceOff(BPGame* game);
	~BPMiniGame_DiceOff();
	
	void Start();
	int GetWeight();
	void Tick();
	void Render();
	
	void OnMouseDown();
	void OnMouseUp();
	void OnMouseMove();
	
	int CountNeighbours(int x, int y);
	void BumpDie(BPMiniGame_DiceOff_Die* die, Ownership newowner);
	void AITurn();
	void AICheck(BPMiniGame_DiceOff_Die* die);
	void SetGameState(MiniGameStates state);
	void UpdateScore();
};

#endif
