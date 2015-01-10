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

#ifndef __MINESWEEP_H__
#define __MINESWEEP_H__

#include "Minigame.h"

class BPMiniGame_MineSweep_Tile {
public:
	int Index; // our position in the tile array
	int X;
	int Y;
	int Row;
	int Col;
	bool HasMine;
	bool Showing;
	string NumMines;
	SpriteFont* sfcNumMines;
	
	BPMiniGame_MineSweep_Tile() {
		Showing = HasMine = false;
		sfcNumMines = NULL;
	}
	
	~BPMiniGame_MineSweep_Tile() {
		SAFE_DELETE(sfcNumMines);
	}
};


class BPMiniGame_MineSweep : public BPMiniGame {
public:
	BPMiniGame_MineSweep(BPGame* game);
	~BPMiniGame_MineSweep();
	void GenerateMines();
	void OnMouseUp();
	void OnMouseDown();
	void OnMouseMove();
	void CheckWin();
	void RevealSquare(BPMiniGame_MineSweep_Tile* tile);
	int GetWeight();
	void Start();
	void Render();
	void Tick();
	
protected:
	Texture* sfcBackground;
	Texture* sfcTile;
	Texture* sfcBombTile;
	BPPList<BPMiniGame_MineSweep_Tile*> Tiles;
	
	int TimeStarted;
	int WinTime;
	int NumTaps; // used to track the score
};

#endif
