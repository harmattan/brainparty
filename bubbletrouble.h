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

#ifndef __BUBBLETROUBLE_H__
#define __BUBBLETROUBLE_H__

#include "Minigame.h"

class BPMiniGame_BubbleTrouble_Bubble {
public:
	float X;
	float Y;
	float XMove;
	float YMove;
	int Direction;
	int Speed;
	int Type;
	int Number;
	SpriteFont* sfcNumberStr;
	
	int LastWrong;
	int CreationTime;
	
	BPMiniGame_BubbleTrouble_Bubble() {
		X = Y = XMove = YMove = 0;
		Direction = Speed = Type = Number = 0;
		LastWrong = -1;
		sfcNumberStr = NULL;
	}
	
	~BPMiniGame_BubbleTrouble_Bubble() {
		SAFE_DELETE(sfcNumberStr);
	}
};


class BPMiniGame_BubbleTrouble : public BPMiniGame {
public:
	BPMiniGame_BubbleTrouble(BPGame* game);
	~BPMiniGame_BubbleTrouble();
protected:
	void Tick();
	void OnMouseUp();
	void OnMouseDown();
	void OnMouseMove();
	void Render();
	int GetWeight();
	void Start();
	void ChooseBubbleDirection(BPMiniGame_BubbleTrouble_Bubble* bubble);
	void SetBubbleDirection(BPMiniGame_BubbleTrouble_Bubble* bubble);
private:
	Texture* sfcBackground;
	Texture* sfcRedCross;
	
	BPPList<Texture*> BubbleTypes;
	BPPList<BPMiniGame_BubbleTrouble_Bubble*> Bubbles;
	BPList<int> Directions;
	int DirectionPos;
	int MaxNumber;
	
	int TimeStarted;
	int LastCreatedTime;
	int SuccessTime;
	
	int NumWrong;
	
	void ClickBubble(BPMiniGame_BubbleTrouble_Bubble* clicked);
	void CreateBubble(bool initial);
};

#endif
