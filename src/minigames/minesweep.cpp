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

#include "minesweep.h"
#include "Minigame.h"
		
BPMiniGame_MineSweep::BPMiniGame_MineSweep(BPGame* game) : BPMiniGame(game) {
	sfcBackground = TheGame->LoadBitmap("minesweep", 320, 416);
	sfcTile = TheGame->LoadBitmap("minesweep_tile", 50, 50);
	sfcBombTile = TheGame->LoadBitmap("minesweep_bombtile", 50, 50);
	
	TimeStarted = NumTaps = 0;
	WinTime = -1;
	
	GameTitle = "Minesweep";
	GameHelp = "Tap squares to find mines; squares with mines nearby will show you how many mines are in adjacent squares. There are eight mines, and you need to uncover them in the fewest moves possible - good luck!";
	GameHelp2 = "When you see a number in a square, it means there are that many mines in the eight adjacent squares. You need to use that information to figure out where all 8 mines are. When you have found all the mines, you win - the faster you are, the higher you score!";
	
	MiniGameType = LIVELY;
	
	GenerateMines();
}

BPMiniGame_MineSweep::~BPMiniGame_MineSweep() {
	SAFE_DELETE(sfcBackground);
	SAFE_DELETE(sfcTile);
	SAFE_DELETE(sfcBombTile);
	Tiles.Clear();
}

void BPMiniGame_MineSweep::GenerateMines() {
	Tiles.Clear();
	
	BPMiniGame_MineSweep_Tile* tile;
	
	int index = 0;
	
	for (int i = 0; i < 6; ++i) {
		for (int j = 0; j < 8; ++j) {
			tile = new BPMiniGame_MineSweep_Tile();
			tile->Index = index++;
			
			tile->X = 11 + (i * 50);
			tile->Y = 9 + (j * 50);
			
			tile->Row = j;
			tile->Col = i;
			
			Tiles.Add(tile);
		}
	}
	
	// pick which squares are going to have mines
	BPList<int> NumList;
	
	for (int i = 0; i < Tiles.Count; ++i) NumList.Add(i);
	NumList.Shuffle();
	
	for (int i = 0; i < 8; ++i) {
		if (NumList[i] == 40) NumList.SetValue(i, NumList[i + 10]);
		
		Tiles[NumList[i]]->HasMine = true;
		Tiles[NumList[i]]->NumMines = "Eek!"; // this just avoids us showing mines by accident when clicking empty squares
	}
	
	int num_mines;
	
	// now update all the other squares so they know how many mines are around them
	for (int i = 0; i < Tiles.Count; ++i) {
		tile = Tiles[i];
		
		if (tile->HasMine) continue;
		
		num_mines = 0;
		
		if (tile->Row > 0 && tile->Col > 0 && Tiles[i - 9]->HasMine == true) ++num_mines; // up-left one
		if (tile->Row > 0 && Tiles[i - 1]->HasMine == true) ++num_mines; // up one
		if (tile->Row > 0 && tile->Col < 5 && Tiles[i + 7]->HasMine == true) ++num_mines; // up-right one
		
		if (tile->Col > 0 && Tiles[i - 8]->HasMine == true) ++num_mines; // left one
		if (tile->Col < 5 && Tiles[i + 8]->HasMine == true) ++num_mines; // right one
		
		if (tile->Row < 7 && tile->Col > 0 && Tiles[i - 7]->HasMine == true) ++num_mines; // down-left one
		if (tile->Row < 7 && Tiles[i + 1]->HasMine == true) ++num_mines; // down one
		if (tile->Row < 7  && tile->Col < 5 && Tiles[i + 9]->HasMine == true) ++num_mines; // down-right one
		
		if (num_mines > 0) {
			ostringstream mines;
			mines << num_mines;
			string str = mines.str();
			tile->NumMines = str;
			TheGame->AllocString(&tile->sfcNumMines, tile->NumMines.c_str(), LARGE, 50, 35, CENTRED, true);
		} else {
			tile->NumMines = "";
		}
	}
	
	WinTime = -1;	
}

void BPMiniGame_MineSweep::OnMouseDown() {
	
}

void BPMiniGame_MineSweep::OnMouseMove() {
	
}

void BPMiniGame_MineSweep::OnMouseUp() {
	if (WinTime != -1) return; // we've lost; can't click any more squares
	
	for (int i = 0; i < Tiles.Count; ++i) {
		BPMiniGame_MineSweep_Tile* tile = Tiles[i];
		if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, tile->X, tile->Y, 50, 50)) {
			if (tile->Showing) continue;
			
			++NumTaps;
			
			if (tile->NumMines.size() == 0) {
				// this square is empty - are there any other empty squares around?
				RevealSquare(tile);
				
				TheGame->PlaySound("swoosh_long");
			} else {
				tile->Showing = true;
				
				if (tile->HasMine) {				
					TheGame->PlaySound("explosion");
				} else {
					TheGame->PlaySound("card_flip");					
				}
			}
			
			break;
		}
	}
	
	CheckWin();
}

void BPMiniGame_MineSweep::CheckWin() {
	int num_found = 0;
	
	for (int i = 0; i < Tiles.Count; ++i) {
		if (Tiles[i]->HasMine && Tiles[i]->Showing) {
			++num_found;
		}
	}
	
	if (num_found == 8) {
		WinTime = TheGame->TickCount;
	}
}

void BPMiniGame_MineSweep::RevealSquare(BPMiniGame_MineSweep_Tile* tile) {
	if (tile->Showing) return;
	
	tile->Showing = true;
		
	if (tile->NumMines.size() == 0) {
		if (tile->Col > 0 && tile->Row > 0 && !Tiles[tile->Index - 9]->HasMine) RevealSquare(Tiles[tile->Index - 9]); // mine above left
		if (tile->Row > 0 && !Tiles[tile->Index - 1]->HasMine) RevealSquare(Tiles[tile->Index - 1]); // mine above
		if (tile->Col < 5 && tile->Row > 0 && !Tiles[tile->Index + 7]->HasMine) RevealSquare(Tiles[tile->Index + 7]); // mine above right
		
		if (tile->Col > 0 && !Tiles[tile->Index - 8]->HasMine) RevealSquare(Tiles[tile->Index - 8]); // mine to the left
		if (tile->Col < 5 && !Tiles[tile->Index + 8]->HasMine) RevealSquare(Tiles[tile->Index + 8]); // mine to the right
		
		if (tile->Col > 0 && tile->Row < 7 && !Tiles[tile->Index - 7]->HasMine) RevealSquare(Tiles[tile->Index - 7]); // mine below left
		if (tile->Row < 7 && !Tiles[tile->Index + 1]->HasMine) RevealSquare(Tiles[tile->Index + 1]); // mine below
		if (tile->Col < 5 && tile->Row < 7 && !Tiles[tile->Index + 9]->HasMine) RevealSquare(Tiles[tile->Index + 9]); // mine below right
		
	}
}

int BPMiniGame_MineSweep::GetWeight() {
	return MinMax(950 - (NumTaps * 30));
}

void BPMiniGame_MineSweep::Start() {
	TimeStarted = TheGame->TickCount;
}

void BPMiniGame_MineSweep::Render() {
	TheGame->DrawImage(sfcBackground, 0, 0);
	
	for (int i = 0; i < Tiles.Count; ++i) {
		BPMiniGame_MineSweep_Tile* tile = Tiles[i];
		
		if (tile->Showing) {
			if (tile->HasMine) {
				TheGame->DrawImage(sfcBombTile, tile->X, tile->Y);
			} else {
				if (tile->NumMines.size() != 0) {
					TheGame->DrawString(tile->sfcNumMines, WHITE, tile->X, tile->Y + 7);
				}
			}
		} else {
			TheGame->DrawImage(sfcTile, tile->X, tile->Y);
		}
	}
}

void BPMiniGame_MineSweep::Tick() {
	if (WinTime != -1 && WinTime + 500 < TheGame->TickCount) {
		Success();
		return;
	}
}
