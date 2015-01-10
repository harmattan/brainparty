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

#ifndef __PATCHMATCH_H__
#define __PATCHMATCH_H__

#include "Minigame.h"

class BPMiniGame_PatchMatch_Box {
public:
	int X;
	int Y;
	int DestY;
	int DrawX;
	int DrawY;
	int YSpeed;
	int Colour;
	int MatchTime;
	
	BPMiniGame_PatchMatch_Box() {
		X = Y = DestY = DrawX = DrawY = YSpeed = Colour = 0;
		MatchTime = -1;
	}
};

class BPMiniGame_PatchMatch : public BPMiniGame {
public:
	BPMiniGame_PatchMatch(BPGame* game);
	~BPMiniGame_PatchMatch();
	void OnMouseMove();
	void OnMouseDown();
	void OnMouseUp();
	void Start();
	int GetWeight();
	void Render();
	void Tick();
	void SetMarathon();
	void SetScore();
	
protected:
	Texture* sfcBackground;
	
	vector<vector<BPMiniGame_PatchMatch_Box*>*> Boxes;
	BPPList<Texture*> PatchPictures;	
	SpriteFont* sfcClock;
		
	int DisappearTime;
	BPPoint LastPos;
	
	BPMiniGame_PatchMatch_Box* SelectedBox;
	
	static const int BoxSize = 50;
	static const int HalfBoxSize = 25;
	
	bool Locked; // when true disallow movement
	
	int TimeStarted;
	
	int SuccessTime;
	
	int Score;
	SpriteFont* sfcScoreStr;
};

#endif
