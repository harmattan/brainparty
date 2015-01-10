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

#ifndef __SHARPSHOOTER_H__
#define __SHARPSHOOTER_H__

#include "Minigame.h"

class BPMiniGame_Sharpshooter_Target {
public:
	float X;
	float Y;
	int HitTime;
	int CreatedTime;
	int TargetType; // set to -1 if it's a plain old target
	bool Hit;
	
	BPMiniGame_Sharpshooter_Target() {
		HitTime = CreatedTime = -1;
		X = Y = 0;
		TargetType = -1;
		Hit = false;
	}
};


class BPMiniGame_Sharpshooter : public BPMiniGame {
	Texture* sfcBackground;
	Texture* sfcTarget;
	Texture* sfcDontShoot;
	BPPList<BPMiniGame_Sharpshooter_Target*> Targets;
	BPPList<BPPoint*> Positions;
	
	int LastCreatedTime;
	int PauseTime;
	int CreateDelay;
	int TargetLife;
	
	int NumTargets;
	int SuccessTime;
	int NumHit;
	
public:
	BPMiniGame_Sharpshooter(BPGame* game);
	~BPMiniGame_Sharpshooter();
	void Start();
	int GetWeight();
	void Render();
	void Tick();
	void OnMouseDown();
	void OnMouseMove();
	void OnMouseUp();
	void CreateTarget(int type);
};

#endif
