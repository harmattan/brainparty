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

#ifndef __UNTANGLER_H__
#define __UNTANGLER_H__

#include "Minigame.h"

class BPMiniGame_Untangler_Point {
public:
	BPPoint Pos;
	BPMiniGame_Untangler_Point* Connect1;
	BPMiniGame_Untangler_Point* Connect2;
};


class BPMiniGame_Untangler : public BPMiniGame {
public:
	BPMiniGame_Untangler(BPGame* game);
	~BPMiniGame_Untangler();
	void Start();
	int GetWeight();
	void Render();
	void Tick();
	void OnMouseUp();
	void OnMouseDown();
	void OnMouseMove();
	float abs2(float x);
	int LinesCross(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3);
	void LevelUp();
	void SetMarathon();
	bool AnyCollisions();
	
protected:
	BPPList<BPMiniGame_Untangler_Point*> TanglePoints;
	BPMiniGame_Untangler_Point* SelectedPoint;
	Texture* sfcCircleHandle;
	Texture* sfcBackground;
	BPPList<BPMiniGame_BGStar*> Stars;
	
	int LastMoveTime;
	int CurrentLevel;
	
	static const int HalfSize = 8;
	static const int Size = 16;
	
	int TimeStarted;
	int SuccessTime;
};

#endif
