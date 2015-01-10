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

#ifndef __JEWELFLIP_H__
#define __JEWELFLIP_H__

#include "Minigame.h"

class BPMiniGame_JewelFlip_Jewel {
public:
	int GemType;
	int X;
	int Y;
	int DestX;
	int DestY;
	int Speed;
	
	bool Removed;
};

class BPMiniGame_JewelFlip : public BPMiniGame {
public:
	BPMiniGame_JewelFlip(BPGame* game);
	~BPMiniGame_JewelFlip();
	void Start();
	int GetWeight();
	void Render();
	void Tick();
	bool CanSwitch(BPMiniGame_JewelFlip_Jewel* gem1, BPMiniGame_JewelFlip_Jewel* gem2);
	BPMiniGame_JewelFlip_Jewel* AutoSubstituteGem(int position, BPMiniGame_JewelFlip_Jewel* match, BPMiniGame_JewelFlip_Jewel* replace);
	bool GemMatches(BPMiniGame_JewelFlip_Jewel* gem1, BPMiniGame_JewelFlip_Jewel* gem2);
	void OnMouseUp();
	void OnMouseDown();
	void OnMouseMove();
	bool GemsAreAdjacent(BPMiniGame_JewelFlip_Jewel* gem1, BPMiniGame_JewelFlip_Jewel* gem2);
	void SwapGems(BPMiniGame_JewelFlip_Jewel* gem1, BPMiniGame_JewelFlip_Jewel* gem2);
	int RemoveGem(BPMiniGame_JewelFlip_Jewel* gem);
protected:
	Texture* sfcBackground;
	Texture* sfcBottomBar;
	BPPList<Texture*> GemTypes;
	BPPList<BPMiniGame_JewelFlip_Jewel*> Gems;
	
	Texture* sfcSelected;
	
	BPMiniGame_JewelFlip_Jewel* SelectedGem;
	
	int GameOverCounter;
	int GameOverTimer;
	bool GameOver;
	int TotalScore;
	SpriteFont* sfcTotalScore;
	int TimeStarted;
};

#endif
