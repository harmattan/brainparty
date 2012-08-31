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

#ifndef __MARBLEDROP_H__
#define __MARBLEDROP_H__

#include "Minigame.h"

class BPMiniGame_MarbleDrop : public BPMiniGame {
public:
	BPMiniGame_MarbleDrop(BPGame* game);
	~BPMiniGame_MarbleDrop();
	void OnMouseUp();
	void OnMouseDown();
	void OnMouseMove();
	bool CheckAnswer();
	void MakeMove(BPList<int>& playerlist);
	void Start();
	int GetWeight();
	void Render();
	void Tick();
	void GenerateBoard();
	
protected:
	Texture* sfcBackground;
	
	BPPList<Texture*> MarbleTypes;
	
	BPList<int> MarbleOrder;
	
	BPList<int> OriginalFirstCol;
	BPList<int> OriginalSecondCol;
	BPList<int> OriginalThirdCol;
	BPList<int> OriginalFourthCol;
	
	BPList<int> PlayerFirstCol;
	BPList<int> PlayerSecondCol;
	BPList<int> PlayerThirdCol;
	BPList<int> PlayerFourthCol;
	
	int FinishTime;
	bool IsSuccess;
	
	int TimeStarted;
};

#endif
