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

#ifndef __CUPSNBALLS_H__
#define __CUPSNBALLS_H__

#include "Minigame.h"

class BPMiniGame_CupsNBalls_Cup {
public:
	double X;
	double Y;
	int Type; // 0 for green, 1 for red
	double SinVal;
};

class BPMiniGame_CupsNBalls_Ball {
public:
	float X;
	float Y;
	float XSpeed;
	float YSpeed;
	bool GoingUp;
	int HitTime;
	
	BPMiniGame_CupsNBalls_Ball() {
		GoingUp = true;
		HitTime = X = Y = 0;
		XSpeed = YSpeed = 0;
	}
};

class BPMiniGame_CupsNBalls : public BPMiniGame {
public:
	bool MouseMoved; // set to true if mouse has moved after click and before release, to stop quick tapping to fire
	bool IsStringPulled;

	Texture* sfcBackground;
	Texture* sfcBall;
	Texture* sfcPuff;
	Texture* sfcSlingshot;

	Texture* sfcGreenCup;
	Texture* sfcRedCup;

	BPPoint LastMousePos;

	int Score;
	int Ammo;
	
	SpriteFont* sfcScore;
	SpriteFont* sfcAmmo;

	int CupFrontOffset;

	short SlingshotX;
	short SlingshotY;

	int BallHalfWidth;
	int BallHalfHeight;

	int CupHalfWidth;
	int CupHalfHeight;

	int SlingshotHalfWidth;

	BPPList<BPMiniGame_CupsNBalls_Cup*> Cups;
	BPPList<BPMiniGame_CupsNBalls_Ball*> Balls;

	int TimeStarted;
	
	BPMiniGame_CupsNBalls(BPGame* game);
	~BPMiniGame_CupsNBalls();
	void Start();
	int GetWeight();
	void Render();
	void Tick();
	void OnMouseDown();
	void OnMouseMove();
	void OnMouseUp();
	void FireBullet();
	void CreateCup(int sinval, int type);
	void SetMarathon();
	void SetScore();
	void SetAmmo();
};

#endif
