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

#ifndef __MOONJUMP_H__
#define __MOONJUMP_H__

#include "Minigame.h"

class BPMiniGame_MoonJump_Sheep {
public:
	float X;
	float Y;
	float XSpeed;
	float YSpeed;
	bool MovingRight;
	bool OnScreen;
	
	BPMiniGame_MoonJump_Sheep() {
		X = Y = XSpeed = YSpeed = 0.0f;
		MovingRight = true;
		OnScreen = false;
	}
};

class BPMiniGame_MoonJump : public BPMiniGame {
public:
	~BPMiniGame_MoonJump();
	BPMiniGame_MoonJump(BPGame* game);
	void Start();
	int GetWeight();
	void Render();
	void Tick();
	void CreateSheep();
	void OnMouseDown();
	void OnMouseMove();
	void OnMouseUp();
	
protected:
	Texture* sfcBackground1;
	Texture* sfcBackground2;
	Texture* sfcBackground3;
	Texture* sfcBackground4;
	Texture* sfcBackground5;
	Texture* sfcBackground6;
	
	Texture* sfcSheepLeft;
	Texture* sfcSheepRight;
	
	BPPList<BPMiniGame_MoonJump_Sheep*> Sheep;
	
	float Y;
	int LastCreateTime;
	int SheepCreateDelay;
	
	int SuccessTime;
};

#endif
