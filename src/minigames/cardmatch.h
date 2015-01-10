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

#ifndef __CARDMATCH_H__
#define __CARDMATCH_H__

#include "Minigame.h"

class BPMiniGame_CardMatch_Card {
public:
	int X;
	int Y;
	int Type;
	bool IsShowing;
	int MatchTime;
	int ShowStartTime;
	
	BPMiniGame_CardMatch_Card() {
		X = Y = Type = MatchTime = ShowStartTime = 0;
		IsShowing = false;
	}
};

class BPMiniGame_CardMatch : public BPMiniGame {
public:
	~BPMiniGame_CardMatch();
	BPMiniGame_CardMatch(BPGame* game);
	void Start();
	int GetWeight();
	void Render();
	void Tick();
	void CleanUpCards(bool force);
	void OnMouseDown();
	void OnMouseMove();
	void OnMouseUp();
	void CheckSelectedCards();
private:
	Texture* sfcBackground;
	Texture* sfcCardBack;
	
	float RemovalSpeed;
	
	BPPList<Texture*>* CardTypes;
	BPPList<BPMiniGame_CardMatch_Card*>* Cards;
	
	bool Locked; // true to stop the player making any more moves
	int NumErrors;
	int TimeStarted;
};

#endif
