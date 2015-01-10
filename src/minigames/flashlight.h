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

#ifndef __FLASHLIGHT_H__
#define __FLASHLIGHT_H__

#include "Minigame.h"

class BPMiniGame_Flashlight_Light {
public:
	int X;
	int Y;
};


class BPMiniGame_Flashlight : public BPMiniGame {
public:
	BPMiniGame_Flashlight(BPGame* game);
	~BPMiniGame_Flashlight();
	void ShowLayout();
	void Start();
	int GetWeight();
	void Render();
	void Tick();
	void OnMouseUp();
	void OnMouseDown();
	void OnMouseMove();
protected:
	Texture* sfcBackground;
	Texture* sfcLightOn;
	Texture* sfcLightOff;
	
	BPPList<BPPoint*> Positions;
	BPPList<BPMiniGame_Flashlight_Light*> Lights;
	
	int CurrentLevel;
	int NumTries;
	int SuccessTime;
	
	int NumWrong;
	
	int CurrentlyFlashing;
	int LastChange;
	int ChangeSpeed;
	
	int LastStateChange;
	MiniGameStates GameState;
};

#endif
