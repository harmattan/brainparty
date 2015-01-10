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

#ifndef __BPSAYS_H__
#define __BPSAYS_H__

#include "Minigame.h"

class BPMiniGame_BPSays : public BPMiniGame {
public:
	~BPMiniGame_BPSays();
	BPMiniGame_BPSays(BPGame* game);
	void Start();
	int GetWeight();
	void Render();
	void GenerateSequence();
	void Tick();
	void OnMouseUp();
	void OnMouseDown();
	void OnMouseMove();
	void LevelUp();
	void Wrong();
	void SetMarathon();
protected:
	Texture* sfcBackground;
	Texture* sfcBackgroundChoosing;
	Texture* sfcCircle;
	
	BPList<Colour> ColourList;
	BPPList<SpriteFont*> ColourWords;
	BPList<int>* Sequence;
	BPPList<Texture*>* Numbers;
	
	int GameState;
	int Speed;
	int Countdown;
	int NumCorrect;
	int NumErrors;
	int SequenceCounter;
	int SelectedNumber;
	
	int NumTries;
	
	bool Repeating;
	
	int TimeStarted;
};

#endif
