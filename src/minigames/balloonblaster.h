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

#ifndef __BALLOONBLASTER_H__
#define __BALLOONBLASTER_H__

#include "Minigame.h"

class BPMiniGame_BalloonBlaster_Balloon {
public:
	int X;
	int Y;
	int DestY;
	int YSpeed;
	int Colour;
	int MatchTime;
	
	BPMiniGame_BalloonBlaster_Balloon() {
		MatchTime = -1;
		YSpeed = 0;
	}
	
	~BPMiniGame_BalloonBlaster_Balloon() {
		YSpeed = 0;
	}
};



class BPMiniGame_BalloonBlaster : public BPMiniGame {
public:
	BPMiniGame_BalloonBlaster(BPGame* game);
	~BPMiniGame_BalloonBlaster();	
private:
	Texture* sfcBackground;
	
	vector<vector<BPMiniGame_BalloonBlaster_Balloon*>*>* Balloons;
	BPPList<Texture*>* BalloonTypes;
	Texture* sfcBalloonBlack;
	Texture* sfcPuff;
	
	int TimeStarted;
	int TimePassed;
	
	int SmartBombTime;
	
	int SuccessTime;
	int Score;
	float DisappearTime;

	SpriteFont* sfcScoreStr;
	
	SpriteFont* sfcClock;
			
	void SetScore();
	void OnMouseUp();
	void OnMouseMove();
	void OnMouseDown();
	int SmartBomb(BPMiniGame_BalloonBlaster_Balloon* balloon, int row, int col);
	void ModifyScore(int adjust);
	void Start();
	int GetWeight();
	void Render();
	void Tick();
	int MatchBalloon(BPMiniGame_BalloonBlaster_Balloon* balloon, int row, int col);
	void SetMarathon();
};

#endif
