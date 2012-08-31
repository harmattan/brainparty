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

#ifndef __JEWELJAM_H__
#define __JEWELJAM_H__

#include "Minigame.h"

class BPMiniGame_JewelJam_Box {
public:
	int X;
	int Y;
	int DestY;
	int YSpeed;
	int Colour;
	int MatchTime;
	
	BPMiniGame_JewelJam_Box() {
		X = Y = DestY = YSpeed = Colour = 0;
		MatchTime = -1;
	}
};

class BPMiniGame_JewelJam : public BPMiniGame {
public:
	BPMiniGame_JewelJam(BPGame* game);
	~BPMiniGame_JewelJam();
	void OnMouseMove();
	void OnMouseDown();
	void OnMouseUp();
	void Start();
	int GetWeight();
	void Render();
	void Tick();
	void CheckForMatches();
	int MatchBox(BPMiniGame_JewelJam_Box* box, int row, int col);
	void SetMarathon();
	void SetScore();
	
protected:
	Texture* sfcBackground;
	
	vector<vector<BPMiniGame_JewelJam_Box*>*> Boxes;
	BPPList<Texture*> ColoursLo;
	BPPList<Texture*> ColoursHi;
	
	SpriteFont* sfcClock;
	
	vector<BPMiniGame_JewelJam_Box*>* SelectedRow;
	
	int DisappearTime;
	BPPoint LastPos;
	
	static const int BoxSize = 40;
	static const int HalfBoxSize = 20;
	
	bool Locked; // when true disallow movement
	
	int TimeStarted;
	
	int SuccessTime;
	
	int Score;
	SpriteFont* sfcScoreStr;
};

#endif
