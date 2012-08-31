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

#include "oddoneout.h"
#include "Minigame.h"

BPMiniGame_OddOneOut::BPMiniGame_OddOneOut(BPGame* game) : BPMiniGame(game) {
	Level = NumWrong = TimeStarted = NumTries = 0;
	
	GameTitle = "Odd One Out";
	GameHelp = "I'm going to show you lots of different items, but one of them only appears once. Can you spot which one?";
	GameHelp2 = "You'll be shown pictures of food, of which all the pictures except one appear more than once. For example, you might see five apples, seven bananas, two oranges and a hamburger. Because there's only one hamburger, that's the right answer.";
	
	MiniGameType = PUZZLE;
	
	GameState = WAITING;
	LastStateChange = 0;
	
	sfcBackground = TheGame->LoadBitmap("oddoneout", 320, 416);
	
	CardTypes.Add(TheGame->LoadBitmap("card_1_48", 48, 48));
	CardTypes.Add(TheGame->LoadBitmap("card_2_48", 48, 48));
	CardTypes.Add(TheGame->LoadBitmap("card_3_48", 48, 48));
	CardTypes.Add(TheGame->LoadBitmap("card_4_48", 48, 48));
	CardTypes.Add(TheGame->LoadBitmap("card_5_48", 48, 48));
	CardTypes.Add(TheGame->LoadBitmap("card_6_48", 48, 48));
	CardTypes.Add(TheGame->LoadBitmap("card_7_48", 48, 48));
	CardTypes.Add(TheGame->LoadBitmap("card_8_48", 48, 48));
	
	for (int i = 0; i < 7; ++i) {
		for (int j = 0; j < 9; ++j) {
			Coordinates.Add(new BPPoint(-2 + (i * 46), 0 + (j * 46)));
		}
	}
}

BPMiniGame_OddOneOut::~BPMiniGame_OddOneOut() {
	SAFE_DELETE(sfcBackground);
	CardTypes.Clear();
	Coordinates.Clear();
	Items.Clear();
}

void BPMiniGame_OddOneOut::Start() {
	LevelUp();
	
	TimeStarted = TheGame->TickCount;
}

int BPMiniGame_OddOneOut::GetWeight() {
	float TimePassed = (TheGame->TickCount - TimeStarted) / 1000.0f;
	return MinMax(500 - (NumWrong * 40) - round(TimePassed));
}

void BPMiniGame_OddOneOut::Render() {
	TheGame->DrawImage(sfcBackground, 0, 0);
	
	for (int i = 0; i < Items.Count; ++i) {
		BPMiniGame_OddOneOut_Item* item = Items[i];
		TheGame->DrawImage(CardTypes[item->CardType], item->X, item->Y);
	}
	
	if (GameState == CORRECT) {
		RenderCorrect();
	} else if (GameState == WRONG) {
		RenderWrong();
	}
}

void BPMiniGame_OddOneOut::Tick() {
	if (GameState != WAITING) {
		if (LastStateChange + 500 < TheGame->TickCount) {
			if (GameState == CORRECT) {
				LevelUp();
			} else {
				LevelDown();
			}
			
			LastStateChange = TheGame->TickCount;
			GameState = WAITING;
		}
	}
}

void BPMiniGame_OddOneOut::OnMouseDown() {
	
}

void BPMiniGame_OddOneOut::OnMouseMove() {
	
}

void BPMiniGame_OddOneOut::OnMouseUp() {
	if (GameState == CORRECT || GameState == WRONG) return;
	
	for (int i = 0; i < Items.Count; ++i) {
		BPMiniGame_OddOneOut_Item* item = Items[i];

		if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, item->X, item->Y, 48, 48)) {
			// clicked this item!
			if (item->CardType == 0) {
				TheGame->PlaySound("correct");
				GameState = CORRECT;
				LastStateChange = TheGame->TickCount;
			} else {
				TheGame->PlaySound("wrong");
				GameState = WRONG;
				LastStateChange = TheGame->TickCount;
			}
			
			break;
		}
	}
}

void BPMiniGame_OddOneOut::LevelUp() {
	++Level;
	CreateLevel();
}

void BPMiniGame_OddOneOut::LevelDown() {
	--Level;
	if (Level < 1) Level = 1;
	++NumWrong;
	CreateLevel();
}

void BPMiniGame_OddOneOut::CreateLevel() {
	++NumTries;
	
	if (NumTries > 12) Success();
	
	switch (Level) {
		case 0:
		case 1:
			GenerateLayout(5);
			break;
			
		case 2:
			GenerateLayout(9);
			break;
			
		case 3:
			GenerateLayout(15);
			break;
			
		case 4:
			GenerateLayout(21);
			break;
			
		case 5:
			GenerateLayout(27);
			break;
			
		case 6:
			GenerateLayout(37);
			break;
			
		case 7:
			GenerateLayout(49);
			break;
			
		case 8:
			GenerateLayout(63);
			break;
			
		case 9:
			Success();
			break;
	}
}

void BPMiniGame_OddOneOut::GenerateLayout(int num) {
	Items.Clear();
	CardTypes.Shuffle();
	Coordinates.Shuffle();
	
	// we need to add at least two of every object
	int item_count = 1;
	int num_used = 0;
	
	// make the odd one out first
	BPPoint* odd = Coordinates[0];
	
	BPMiniGame_OddOneOut_Item* odd_item = new BPMiniGame_OddOneOut_Item();
	odd_item->X = odd->X;
	odd_item->Y = odd->Y;
	odd_item->CardType = 0;
	Items.Add(odd_item);
	
	for (int i = 1; i < num; ++i) {
		BPMiniGame_OddOneOut_Item* item = new BPMiniGame_OddOneOut_Item();
		item->X = Coordinates[i]->X;
		item->Y = Coordinates[i]->Y;
		item->CardType = item_count;
		++num_used;
		
		if (num_used == 2) {
			num_used = 0;
			++item_count;
		}
		
		if (item_count >= CardTypes.Count) {
			item_count = 1;
		}
		
		Items.Add(item);
	}
}
