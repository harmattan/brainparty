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

#include "jewelflip.h"
#include "Minigame.h"

BPMiniGame_JewelFlip::BPMiniGame_JewelFlip(BPGame* game) : BPMiniGame(game) {
	sfcBackground = TheGame->LoadBitmap("jewelflip", 320, 416);
	sfcBottomBar = TheGame->LoadBitmap("jewelflip_bottombar", 320, 48);
	sfcSelected = TheGame->LoadBitmap("selected48", 48, 48);
	
	SelectedGem = NULL;
	sfcTotalScore = NULL;
	
	TheGame->AllocString(&sfcTotalScore, "Score: 0", LARGE, 320, 37, CENTRED, true);
	
	GameOverCounter = GameOverTimer = TotalScore = TimeStarted = 0;
	GameOver = false;
	
	GameTitle = "Jewel Flip";
	GameHelp = "Tap adjacent gems to switch them, but only if one of the gems ends next to another of the same type. Try to group gems as best you can, then tap \"Ready\" once you're done - bigger groups get more points!";
	GameHelp2 = "To swap a gem, select it then select another gem above, below, left or right of it. You can swap gems only if one of them ends up next to another one of its own type. For example, if you want to swap a white gem and a red gem, then either the white gem must end up next to another white gem or the red gem must end up next to another red gem.";
	
	MiniGameType = PUZZLE;
	
	GemTypes.Add(TheGame->LoadBitmap("gem1_small", 48, 48));
	GemTypes.Add(TheGame->LoadBitmap("gem2_small", 48, 48));
	GemTypes.Add(TheGame->LoadBitmap("gem3_small", 48, 48));
	GemTypes.Add(TheGame->LoadBitmap("gem4_small", 48, 48));
	GemTypes.Add(TheGame->LoadBitmap("gem5_small", 48, 48));
	GemTypes.Add(TheGame->LoadBitmap("gem6_small", 48, 48));
	GemTypes.Add(TheGame->LoadBitmap("gem7_small", 48, 48));
	GemTypes.Add(TheGame->LoadBitmap("gem8_small", 48, 48));
	
	BPList<int> JewelList;
	
	// generate an equal distribution of gem types so that the scores aren't too uneven
	for (int i = 0; i < 42; ++i) {
		JewelList.Add(i % GemTypes.Count);
	}
	
	JewelList.Shuffle();
	
	int n = 0;
	
	for (int i = 0; i < 6; ++i) {
		for (int j = 0; j < 7; ++j) {
			BPMiniGame_JewelFlip_Jewel* gem = new BPMiniGame_JewelFlip_Jewel();
			gem->X = 11 + (i * 50);
			gem->Y = 11 + (j * 50);
			gem->GemType = JewelList[n];
			gem->DestX = gem->X;
			gem->DestY = gem->Y;
			
			Gems.Add(gem);
			
			++n;
		}
	}
}

BPMiniGame_JewelFlip::~BPMiniGame_JewelFlip() {
	SAFE_DELETE(sfcBackground);
	SAFE_DELETE(sfcBottomBar);
	SAFE_DELETE(sfcSelected);
	
	GemTypes.Clear();
	
	Gems.Clear();
	
	SAFE_DELETE(SelectedGem);
	
	SAFE_DELETE(sfcTotalScore);
}

void BPMiniGame_JewelFlip::Start() {
	TimeStarted = TheGame->TickCount;
}

int BPMiniGame_JewelFlip::GetWeight() {
	float TimePassed = (TheGame->TickCount - TimeStarted) / 1000.0f;
	return MinMax((TotalScore + TotalScore + TotalScore) - round(TimePassed / 4.0f));
}

void BPMiniGame_JewelFlip::Render() {
	TheGame->DrawImage(sfcBackground, 0, 0);
	
	for (int i = 0; i < Gems.Count; ++i) {
		BPMiniGame_JewelFlip_Jewel* gem = Gems[i];
		
		if (gem->Removed) continue;
		
		TheGame->DrawImage(GemTypes[gem->GemType], gem->X, gem->Y);
		
		if (SelectedGem == gem) {
			TheGame->DrawImage(sfcSelected, gem->X, gem->Y);
		}
	}
	
	if (GameOver) {
		TheGame->DrawImage(sfcBottomBar, 0, 368);
		TheGame->DrawString(sfcTotalScore, WHITE, 0, 374);
	}
}

void BPMiniGame_JewelFlip::Tick() {
	if (GameOver && GameOverTimer + 250 < TheGame->TickCount) {
		GameOverTimer = TheGame->TickCount;
		
		if (GameOverCounter == Gems.Count) {
			Success();
			return;
		}
		
		while (Gems[GameOverCounter]->Removed) {
			++GameOverCounter;
			
			if (GameOverCounter == Gems.Count) {
				Success();
				return;
			}
		}
		
		TotalScore += (int)pow(2.0f, RemoveGem(Gems[GameOverCounter]));
		
		ostringstream totalscore;
		totalscore << "Score: " << TotalScore;
		TheGame->AllocString(&sfcTotalScore, totalscore.str().c_str(), LARGE, 320, 37, CENTRED, true);
	} else {
		
		for (int i = 0; i < Gems.Count; ++i) {
			BPMiniGame_JewelFlip_Jewel* gem = Gems[i];
			if (gem->Speed != 0) {
				// this gem needs to move
				
				if (gem->Y != gem->DestY) {
					if (gem->Y < gem->DestY) {
						++gem->Speed;
					} else {
						--gem->Speed;
					}
					
					gem->Y += gem->Speed;
					
					if (gem->Speed > 0 && gem->Y >= gem->DestY) {
						gem->Y = gem->DestY;
						gem->Speed = 0;
					} else if (gem->Speed < 0 && gem->Y <= gem->DestY) {
						gem->Y = gem->DestY;
						gem->Speed = 0;
					}
				} else if (gem->X != gem->DestX) {
					if (gem->X < gem->DestX) {
						++gem->Speed;
					} else {
						--gem->Speed;
					}
					
					gem->X += gem->Speed;
					
					if (gem->Speed > 0 && gem->X >= gem->DestX) {
						gem->X = gem->DestX;
						gem->Speed = 0;
					} else if (gem->Speed < 0 && gem->X <= gem->DestX) {
						gem->X = gem->DestX;
						gem->Speed = 0;
					}
				}
			}
		}
	}
}

bool BPMiniGame_JewelFlip::CanSwitch(BPMiniGame_JewelFlip_Jewel* gem1, BPMiniGame_JewelFlip_Jewel* gem2) {
	// if these two gems were swapped, would they match anything?
	
	if (GemMatches(gem1, gem2)) return true;
	if (GemMatches(gem2, gem1)) return true;
	
	MessageBox::Show("You can't swap these two gems, because at least one of them must end up next to a gem of the same type.", GameTitle);
	SelectedGem = NULL;
	
	return false;
}

BPMiniGame_JewelFlip_Jewel* BPMiniGame_JewelFlip::AutoSubstituteGem(int position, BPMiniGame_JewelFlip_Jewel* match, BPMiniGame_JewelFlip_Jewel* replace) {
	BPMiniGame_JewelFlip_Jewel* gem = Gems[position];
	if (gem == match) return replace;
	return gem;
}

bool BPMiniGame_JewelFlip::GemMatches(BPMiniGame_JewelFlip_Jewel* gem1, BPMiniGame_JewelFlip_Jewel* gem2) {
	int type = gem1->GemType;
	int position = Gems.IndexOf(gem2);
	
	int Row;
	int Col = DivRem(position, 7, Row);
	
	// try searching horizontally
	if (Col > 0) {
		// gem to the left
		if (AutoSubstituteGem(position - 7, gem1, gem2)->GemType == type) return true;
	}
	
	if (Col < 5) {
		// one to the right
		if (AutoSubstituteGem(position + 7, gem1, gem2)->GemType == type) return true;
	}
	
	// try searching vertically
	if (Row > 0) {
		// gem above
		if (AutoSubstituteGem(position - 1, gem1, gem2)->GemType == type) return true;
	}
	
	if (Row < 6) {
		// gem below
		if (AutoSubstituteGem(position + 1, gem1, gem2)->GemType == type) return true;
	}
	
	return false;
}

void BPMiniGame_JewelFlip::OnMouseDown() {
	
}

void BPMiniGame_JewelFlip::OnMouseMove() {
	
}

void BPMiniGame_JewelFlip::OnMouseUp() {
	if (GameOver == true) return;
	
	if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 110, 370, 102, 47)) {
		GameOver = true;
		GameOverTimer = TheGame->TickCount;
		return;
	}
	
	for (int i = 0; i < Gems.Count; ++i) {
		BPMiniGame_JewelFlip_Jewel* gem = Gems[i];
		
		if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, gem->X, gem->DestY, 48, 48)) {
			// we've clicked this gem!
			if (gem->Removed) break;
						
			if (SelectedGem == gem) {
				// if we've selected it already, deselect it
				SelectedGem = NULL;
			} else {
				if (SelectedGem == NULL) {
					SelectedGem = gem;
				} else if (GemsAreAdjacent(SelectedGem, gem)) {
					SwapGems(SelectedGem, gem);
				} else {
					MessageBox::Show("These two gems can't be swapped because they are not next to each other.", GameTitle);
					SelectedGem = NULL;
				}
			}
			
			break;
		}
	}
}

bool BPMiniGame_JewelFlip::GemsAreAdjacent(BPMiniGame_JewelFlip_Jewel* gem1, BPMiniGame_JewelFlip_Jewel* gem2) {
	return (abs(gem1->DestX - gem2->DestX) + abs(gem1->DestY - gem2->DestY)) == 50;
}

void BPMiniGame_JewelFlip::SwapGems(BPMiniGame_JewelFlip_Jewel* gem1, BPMiniGame_JewelFlip_Jewel* gem2) {
	if (!CanSwitch(gem1, gem2)) return;
	
	TheGame->PlaySound("slide");
	
	SelectedGem = NULL;
	
	gem1->DestX = gem2->X;
	gem1->DestY = gem2->Y;
	
	gem2->DestX = gem1->X;
	gem2->DestY = gem1->Y;
	
	if (gem1->DestX < gem1->X || gem1->DestY < gem1->Y) {
		gem1->Speed = -1;
	} else {
		gem1->Speed = 1;
	}
	
	if (gem2->DestX < gem2->X || gem2->DestY < gem2->Y) {
		gem2->Speed = -1;
	} else {
		gem2->Speed = 1;
	}
	
	// we need to swap positions in the Gems array, otherwise CanSwap() won't work
	int Gem1Pos = Gems.IndexOf(gem1);
	int Gem2Pos = Gems.IndexOf(gem2);
	
	// remove and insert the higher-positioned gem first
	if (Gem1Pos > Gem2Pos) {
		Gems.RemoveAt(Gem1Pos);
		Gems.RemoveAt(Gem2Pos);
		
		Gems.Insert(Gem2Pos, gem1);
		Gems.Insert(Gem1Pos, gem2);
	} else {
		Gems.RemoveAt(Gem2Pos);
		Gems.RemoveAt(Gem1Pos);
		
		Gems.Insert(Gem1Pos, gem2);
		Gems.Insert(Gem2Pos, gem1);
	}
}

int BPMiniGame_JewelFlip::RemoveGem(BPMiniGame_JewelFlip_Jewel* gem) {
	// remove this gem, then remove any others that touch it
	if (gem->Removed) return 0; // this has been removed already!
	
	gem->Removed = true;
	if (SelectedGem == gem) SelectedGem = NULL;
	
	int position = Gems.IndexOf(gem);
	
	int Row;
	int Col = DivRem(position, 7, Row);
	
	int num_removed = 1; // we've removed ourselves
	
	// remove the one above
	if (Row > 0) {
		if (Gems[position - 1]->GemType == gem->GemType) num_removed += RemoveGem(Gems[position - 1]);
	}
	
	// remove the one below
	if (Row < 6) {
		if (Gems[position + 1]->GemType == gem->GemType) num_removed += RemoveGem(Gems[position + 1]);
	}
	
	// remove the one to the left
	if (Col > 0) {
		if (Gems[position - 7]->GemType == gem->GemType) num_removed += RemoveGem(Gems[position - 7]);
	}
	
	// remove the one to the right
	if (Col < 5) {
		if (Gems[position + 7]->GemType == gem->GemType) num_removed += RemoveGem(Gems[position + 7]);
	}
	
	return num_removed;
}
