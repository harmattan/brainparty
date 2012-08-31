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

#include "memoryblox.h"
#include "Minigame.h"

BPMiniGame_MemoryBlox::BPMiniGame_MemoryBlox(BPGame* game) : BPMiniGame(game) {
	IsShowing = true; // are all the tiles revealed?
	TimeStarted = 0;
	LastStateChange = -1;
	
	GameState = WAITING;
	
	GameTitle = "Memoryblox";
	GameHelp = "When you're ready, tap the screen to hide the colours of the blocks. Now find 6 blocks of the same colour to clear them, ie 6 red blocks or 6 blue blocks. How fast can you match them all and clear the whole board?";
	GameHelp2 = "To play, select at least six red OR blue squares. That is, the six squares you choose must be entirely red or entirely blue - if you select two red then choose a blue, you'll need to try again.";
	
	MiniGameType = PUZZLE;
	
	sfcBackground = TheGame->LoadBitmap("memoryblox", 320, 416);
	sfcUnknown = TheGame->LoadBitmap("block_grey", 48, 48);
	sfcRed = TheGame->LoadBitmap("block_red", 48, 48);
	sfcBlue = TheGame->LoadBitmap("block_blue", 48, 48);
	
	BPList<bool> colours;

	for (int i = 0; i < 24; ++i) {
		colours.Add(false);
		colours.Add(true);
	}
	
	colours.Shuffle();
	
	for (int i = 0; i < 48; ++i) {
		BPMiniGame_MemoryBlox_Tile* tile = new BPMiniGame_MemoryBlox_Tile();
		
		switch (colours[i]) {
			case true:
				tile->IsRed = true;
				break;
			case false:
				tile->IsRed = false;
				break;
		}
		
		TileSet.Add(tile);
	}
}

BPMiniGame_MemoryBlox::~BPMiniGame_MemoryBlox() {
	TileSet.Clear();
	SAFE_DELETE(sfcBackground);
	SAFE_DELETE(sfcRed);
	SAFE_DELETE(sfcBlue);
	SAFE_DELETE(sfcUnknown);
}

void BPMiniGame_MemoryBlox::Start() {
	TimeStarted = TheGame->TickCount;
}

int BPMiniGame_MemoryBlox::GetWeight() {
	float TimePassed = (TheGame->TickCount - TimeStarted) / 1000.0f;
	return MinMax(670 - round(TimePassed * 5));
}

void BPMiniGame_MemoryBlox::Render() {
	TheGame->DrawImage(sfcBackground, 0, 0);
	
	int xpos;
	int ypos;
	
	for (int i = 0; i < TileSet.Count; ++i) {
		ypos = DivRem(i, 6, xpos);
		
		if (IsShowing) {
			if (TileSet[i]->IsRed) {
				TheGame->DrawImage(sfcRed, 16 + (xpos * 48), 16 + (ypos * 48));
			} else {
				TheGame->DrawImage(sfcBlue, 16 + (xpos * 48), 16 + (ypos * 48));
			}
		} else {
			if (TileSet[i]->IsCleared) {
				// do nothing; leave a nice black hole so they know it's cleared
			} else {
				if (TileSet[i]->IsSelected) {
					if (TileSet[i]->IsRed) {
						TheGame->DrawImage(sfcRed, 16 + (xpos * 48), 16 + (ypos * 48));
					} else {
						TheGame->DrawImage(sfcBlue, 16 + (xpos * 48), 16 + (ypos * 48));
					}
				} else {
					TheGame->DrawImage(sfcUnknown, 16 + (xpos * 48), 16 + (ypos * 48));
				}
			}
		}
	}
	
	if (GameState == WRONG) {
		if (LastStateChange + 250 < TheGame->TickCount) {
			// wrong!
		}
		
		if (LastStateChange + 850 < TheGame->TickCount) {
			GameState = WAITING;
			LastStateChange = TheGame->TickCount;			
			
			// reset the board, excluding cleared pieces
			for (int i = 0; i < TileSet.Count; ++i) {
				if (!TileSet[i]->IsCleared) {
					TileSet[i]->IsSelected = false;
				}
			}
		}
	} else if (GameState == CORRECT) {
		if (LastStateChange + 250 < TheGame->TickCount) {
			for (int i = 0; i < TileSet.Count; ++i) {
				if (TileSet[i]->IsSelected) {
					TileSet[i]->IsSelected = false;
					TileSet[i]->IsCleared = true;
				}
			}
			

			int numleft = 0;
			
			for (int i = 0; i < TileSet.Count; ++i) {
				if (!TileSet[i]->IsCleared) {
					++numleft;
				}
			}
			
			if (numleft == 0) {
				GameState = SUCCESS;
				LastStateChange = TheGame->TickCount;				
			} else {
				GameState = WAITING;
				LastStateChange = TheGame->TickCount;
			}
		}
	}
}

void BPMiniGame_MemoryBlox::Tick() {
	if (GameState == SUCCESS && LastStateChange + 250 < TheGame->TickCount) {
		Success();
	}
}

void BPMiniGame_MemoryBlox::OnMouseDown() {
	
}

void BPMiniGame_MemoryBlox::OnMouseMove() {
	
}

void BPMiniGame_MemoryBlox::OnMouseUp() {
	if (IsShowing) {
		IsShowing = false;
		TheGame->PlaySound("swoosh_long");
		return;
	}
	
	if (GameState != WAITING) return; // don't let people make a move straight after a mistake or a correct answer
		
	if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 16, 16, 292, 390)) {
		// we slightly overlap the boxes in this game, which means the far right boxes
		// are slightly wider than the others. If we don't do anything special, clicking
		// on the right edge of those boxes will be detected as clicking on the far left
		// box, because it wraps over the size of the boxes.
		// So, if the click was >= 305, we need to subtract 10 to skip this problem.
		
		int xpos = TouchEvent.X;
		if (xpos > 305) xpos -= 10;
		
		int xoffset = (xpos - 17) / 48;
		int yoffset = (TouchEvent.Y - 17) / 48;
		
		int totaloffset = (yoffset * 6) + xoffset;
		
		// this shouldn't be possible, but just in case!
		if (totaloffset >= 48) return;
		
		// if the square has already been selected, ignore the click
		if (TileSet[totaloffset]->IsSelected) return;
		
		TileSet[totaloffset]->IsSelected = true;
		
		if (CheckMove()) {
			int numselected = 0;
			
			for (int i = 0; i < TileSet.Count; ++i) {
				if (TileSet[i]->IsSelected && !TileSet[i]->IsCleared) {
					++numselected;
				}
			}
			
			if (numselected >= 6) {
				LastStateChange = TheGame->TickCount;
				GameState = CORRECT;
			}			
		} else {
			LastStateChange = TheGame->TickCount;
			GameState = WRONG;
		}
	}
}

bool BPMiniGame_MemoryBlox::CheckMove() {
	bool firstsquare = true;
	bool isred = false;
	
	for (int i = 0; i < TileSet.Count; ++i) {
		if (TileSet[i]->IsSelected && !TileSet[i]->IsCleared) {
			if (firstsquare) {
				// set the initial selected colour, so we know what to compare against
				isred = TileSet[i]->IsRed;
				firstsquare = false;
				TheGame->PlaySound("gem_select");
			} else {
				if (TileSet[i]->IsRed != isred) {
					// they selected tiles with different colours!
					TheGame->PlaySound("wrong");
					return false;
				} else {
					TheGame->PlaySound("gem_select");
				}
			}
		}
	}
	
	// current selection is OK; return good move
	return true;
}
