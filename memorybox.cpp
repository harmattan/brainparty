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

#include "memorybox.h"
#include "Minigame.h"
		
BPMiniGame_MemoryBox::BPMiniGame_MemoryBox(BPGame* game) : BPMiniGame(game) {
	sfcBackground = TheGame->LoadBitmap("memorybox", 320, 416);
	sfcUnknownBox = TheGame->LoadBitmap("box_question", 80, 80);
	
	LastStateChange = OriginalLowestNumber = LowestNumber = TimeStarted = NumWrong = NumTries = 0;
	Level = 4.0f;
	ShowDelay = 1000;
		
	GameTitle = "Memory Box";
	GameHelp = "Look at the numbers on the screen, then, when they disappear, tap on the boxes in the order of their numbers, lowest to highest. It might start easy, but I'll be adding more numbers over time!";
	GameHelp2 = "This game requires you to look at numbers very quickly, and remember their positions. When they disappear, you need to tap them in the correct, ascending order. For example, if you see the numbers 5, 9, 1 and 3, you need to tap them in the order 1, 3, 5, 9.";
	
	MiniGameType = PUZZLE;
	
	Numbers[1] = TheGame->LoadBitmap("box_1", 80, 80);
	Numbers[2] = TheGame->LoadBitmap("box_2", 80, 80);
	Numbers[3] = TheGame->LoadBitmap("box_3", 80, 80);
	Numbers[4] = TheGame->LoadBitmap("box_4", 80, 80);
	Numbers[5] = TheGame->LoadBitmap("box_5", 80, 80);
	Numbers[6] = TheGame->LoadBitmap("box_6", 80, 80);
	Numbers[7] = TheGame->LoadBitmap("box_7", 80, 80);
	Numbers[8] = TheGame->LoadBitmap("box_8", 80, 80);
	Numbers[9] = TheGame->LoadBitmap("box_9", 80, 80);
	Numbers[10] = TheGame->LoadBitmap("box_10", 80, 80);
	Numbers[11] = TheGame->LoadBitmap("box_11", 80, 80);
	Numbers[12] = TheGame->LoadBitmap("box_12", 80, 80);
	Numbers[13] = TheGame->LoadBitmap("box_13", 80, 80);
	Numbers[14] = TheGame->LoadBitmap("box_14", 80, 80);
	Numbers[15] = TheGame->LoadBitmap("box_15", 80, 80);
	Numbers[16] = TheGame->LoadBitmap("box_16", 80, 80);
	
	BPList<BPPoint> boxplaces1;
	boxplaces1.Add(BPPoint(70, 118));
	boxplaces1.Add(BPPoint(170, 118));
	boxplaces1.Add(BPPoint(70, 218));
	boxplaces1.Add(BPPoint(170, 218));
	Layouts[4] = boxplaces1;
	
	BPList<BPPoint> boxplaces2;
	boxplaces2.Add(BPPoint(20, 13));
	boxplaces2.Add(BPPoint(220, 13));
	boxplaces2.Add(BPPoint(120, 168));
	boxplaces2.Add(BPPoint(20, 323));
	boxplaces2.Add(BPPoint(220, 323));
	Layouts[5] = boxplaces2;
	
	BPList<BPPoint> boxplaces3;
	boxplaces3.Add(BPPoint(20, 113));
	boxplaces3.Add(BPPoint(120, 113));
	boxplaces3.Add(BPPoint(220, 113));
	boxplaces3.Add(BPPoint(20, 194));
	boxplaces3.Add(BPPoint(120, 194));
	boxplaces3.Add(BPPoint(220, 194));
	Layouts[6] = boxplaces3;
	
	BPList<BPPoint> boxplaces4;
	boxplaces4.Add(BPPoint(20, 13));
	boxplaces4.Add(BPPoint(220, 13));
	boxplaces4.Add(BPPoint(120, 113));
	boxplaces4.Add(BPPoint(120, 213));
	boxplaces4.Add(BPPoint(20, 313));
	boxplaces4.Add(BPPoint(120, 313));
	boxplaces4.Add(BPPoint(220, 313));
	Layouts[7] = boxplaces4;
	
	BPList<BPPoint> boxplaces5;
	boxplaces5.Add(BPPoint(120, 13));
	boxplaces5.Add(BPPoint(120, 113));
	boxplaces5.Add(BPPoint(120, 213));
	boxplaces5.Add(BPPoint(120, 313));
	boxplaces5.Add(BPPoint(20, 113));
	boxplaces5.Add(BPPoint(20, 213));
	boxplaces5.Add(BPPoint(220, 113));
	boxplaces5.Add(BPPoint(220, 213));
	Layouts[8] = boxplaces5;
}

BPMiniGame_MemoryBox::~BPMiniGame_MemoryBox() {
	SAFE_DELETE(sfcBackground);
	SAFE_DELETE(sfcUnknownBox);
	
	for(map<int, Texture*>::iterator it = Numbers.begin(); it != Numbers.end(); ++it) {
		SAFE_DELETE(it->second);
	}
	
	Numbers.clear();

	Layouts.clear();
	ActiveBoxes.Clear();
}

void BPMiniGame_MemoryBox::Start() {
	GenerateBoxes();
	
	TimeStarted = TheGame->TickCount;
}

int BPMiniGame_MemoryBox::GetWeight() {
	float TimePassed = (TheGame->TickCount - TimeStarted) / 1000.0f;
	return MinMax(550 - (NumWrong * 50) - round(TimePassed * 1.2));
}

void BPMiniGame_MemoryBox::Render() {
	TheGame->DrawImage(sfcBackground, 0, 0);
	
	for (int i = 0; i < ActiveBoxes.Count; ++i) {
		BPMiniGame_MemoryBox_Box* box = ActiveBoxes[i];
		
		if (GameState == SHOWING || box->Showing) {
			TheGame->DrawImage(Numbers[box->Number], box->X, box->Y);
		} else {
			TheGame->DrawImage(sfcUnknownBox, box->X, box->Y);
		}
	}
	
	if (GameState == CORRECT) {
		// note that we use a 150ms delay before showing the answer, so players can see the results of their actions
		if (LastStateChange + 150 < TheGame->TickCount) {
			RenderCorrect();
		}
	} else if (GameState == WRONG) {
		if (LastStateChange + 150 < TheGame->TickCount) {
			RenderWrong();
		}
	}
}

void BPMiniGame_MemoryBox::Tick() {
	if (GameState == SHOWING) {
		if (LastStateChange + ShowDelay < TheGame->TickCount) {
			SetGameState(GUESSING);
		}
	} else if (GameState == CORRECT) {
		if (LastStateChange + 800 < TheGame->TickCount) {
			if (Level == 9.0f || NumTries >= 12) {
				Success();
			} else {
				GenerateBoxes();
			}
		}
	} else if (GameState == WRONG) {
		if (LastStateChange + 800 < TheGame->TickCount) {
			if (NumTries >= 12) {
				Success();
			} else {
				for (int i = 0; i < ActiveBoxes.Count; ++i) {
					ActiveBoxes[i]->Showing = false;
				}
			
				ShowDelay += 155;
				++NumTries;
				SetGameState(SHOWING);
				LowestNumber = OriginalLowestNumber;
			}
		}
	}
	
}

void BPMiniGame_MemoryBox::OnMouseDown() {
	
}

void BPMiniGame_MemoryBox::OnMouseMove() {
	
}

void BPMiniGame_MemoryBox::OnMouseUp() {
	if (GameState == SHOWING) {
		// they've tapped the screen - hurry up!
		SetGameState(GUESSING);
	}
	
	if (GameState != GUESSING) return;
	
	for (int i = 0; i < ActiveBoxes.Count; ++i) {
		BPMiniGame_MemoryBox_Box* box = ActiveBoxes[i];
		
		if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, box->X, box->Y, 80, 80)) {
			// this box was clicked
			if (box->Showing) continue; // don't let people tap on a showing box twice
			
			if (box->Number == LowestNumber) {
				// this is the correct box!
				box->Showing = true;
			
				// find the next lowest number
				LowestNumber = 999;
				
				for (int i = 0; i < ActiveBoxes.Count; ++i) {
					BPMiniGame_MemoryBox_Box* box2 = ActiveBoxes[i];
					
					if (!box2->Showing) {
						if (box2->Number < LowestNumber) LowestNumber = box2->Number;
					}
				}
				
				if (LowestNumber == 999) {
					// no more boxes to reveal - player has won!
					SetGameState(CORRECT);
					TheGame->PlaySound("correct");
					Level += 0.5f;
				} else {
					// more boxes to reveal - play normal reveal sound
					TheGame->PlaySound("card_flip");
				}
			} else {
				// oops - wrong box!
				TheGame->PlaySound("wrong");
				box->Showing = true;
				SetGameState(WRONG);
				++NumWrong;
			}
		}
	}
}

void BPMiniGame_MemoryBox::SetGameState(MiniGameStates gs) {
	GameState = gs;
	LastStateChange = TheGame->TickCount;
}

void BPMiniGame_MemoryBox::GenerateBoxes() {
	// show the boxes for longer each time; helps bad players and makes the harder levels easier
	ShowDelay += 155;
	++NumTries;
	
	ActiveBoxes.Clear();
	
	// the level count is stored as a float so we can add 0.5 each success and go up a level every two successes
	int ilevel = floor(Level);
	
	Layouts[ilevel].Shuffle();
	
	int num = TheGame->RandomRange(1, 4);
	OriginalLowestNumber = num;
	LowestNumber = num;
	
	for (int i = 0; i < ilevel; ++i) {
		BPMiniGame_MemoryBox_Box* box = new BPMiniGame_MemoryBox_Box();
		box->Number = num;
		num += TheGame->RandomRange(1, 2); // this is the number for the next box, to ensure we don't have the same number twice
		
		box->X = Layouts[ilevel][i].X;
		box->Y = Layouts[ilevel][i].Y;
		
		ActiveBoxes.Add(box);
	}
	
	SetGameState(SHOWING);
}
