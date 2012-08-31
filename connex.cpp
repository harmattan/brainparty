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

#include "connex.h"
#include "Minigame.h"

BPMiniGame_Connex::BPMiniGame_Connex(BPGame* game) : BPMiniGame(game) {
	sfcBackground = TheGame->LoadBitmap("connex", 320, 480);
	sfcCard = TheGame->LoadBitmap("card_blank", 64, 64);

	Letters = new BPList<const char*>();
	Objects = new BPPList<BPMiniGame_Connex_Object*>();
	CompleteLetters = TimeStarted = 0;
	
	LastLetter = -1;

	GameTitle = "Connex";
	GameHelp = "Find and tap on boxes to connect them together. But here's the catch - you need to connect A to 1, 1 to B, B to 2, 2 to C and so on, alternating between letters and numbers. It's harder than it sounds!";
	GameHelp2 = "This game tests your ability to remember the positions of boxes as you play. That is, while you're looking for the first box (\"A\"), you need to try to remember what the other boxes are that are around A so that when you have to find them you know where to look.";
	
	MiniGameType = PUZZLE;
	
	Letters->Add("A");
	Letters->Add("1");
	Letters->Add("B");
	Letters->Add("2");
	Letters->Add("C");
	Letters->Add("3");
	Letters->Add("D");
	Letters->Add("4");
	Letters->Add("E");
	Letters->Add("5");
	Letters->Add("F");
	Letters->Add("6");
	Letters->Add("G");
	Letters->Add("7");
	Letters->Add("H");
	Letters->Add("8");
	Letters->Add("I");
	Letters->Add("9");
	Letters->Add("J");
	Letters->Add("10");
	
	CompleteLetters = 0;
	
	BPList<const char*>* shuffledletters = new BPList<const char*>();
	
	for (int i = 0; i < Letters->Count; ++i) {
		shuffledletters->Add((*Letters)[i]);
	}
	
	shuffledletters->Shuffle();
	
	for (int i = 0; i < shuffledletters->Count; ++i) {
		BPMiniGame_Connex_Object* obj = new BPMiniGame_Connex_Object();
		obj->Text = (*shuffledletters)[i];
		TheGame->AllocString(&obj->sfcText, obj->Text, XLARGE, 64, 64, CENTRED, true);
		
		int xpos;
		int ypos;
		
		ypos = TheGame->DivRem(i, 4, &xpos);
		
		obj->X = 8 + (xpos * 80);
		obj->Y = 13 + (ypos * 80);
		
		Objects->Add(obj);
	}
	
	SAFE_DELETE(shuffledletters);
}

BPMiniGame_Connex::~BPMiniGame_Connex() {
	SAFE_DELETE(sfcBackground);
	SAFE_DELETE(sfcCard);
	
	Letters->Clear();
	SAFE_DELETE(Letters);
	
	Objects->Clear();
	SAFE_DELETE(Objects);
}

void BPMiniGame_Connex::Start() {
	TimeStarted = TheGame->TickCount;
}

int BPMiniGame_Connex::GetWeight() {
	float TimePassed = (TheGame->TickCount - TimeStarted) / 1000.0f;
	return MinMax(560 - floor(TimePassed * 6));
}

void BPMiniGame_Connex::Render() {
	TheGame->DrawImage(sfcBackground, 0, 0);

	for (int i = 0; i < Objects->Count; ++i) {
		BPMiniGame_Connex_Object* obj = (*Objects)[i];
		TheGame->DrawImage(sfcCard, obj->X, obj->Y);
		
		if ((*Letters)[LastLetter] == obj->Text) {
			// this is the current box - highlight it!
			TheGame->FillRectangle((*TheGame->Yellow), obj->X + 3, obj->Y + 3, 58, 58);
		}
	}
	
	for (int i = 0; i < CompleteLetters; ++i) {
		BPMiniGame_Connex_Object* from;
		BPMiniGame_Connex_Object* to;
		
		for (int n = 0; n < Objects->Count; ++n) {
			if ((*Objects)[n]->Text == (*Letters)[i]) {
				from = (*Objects)[n];
				break;
			}
		}

		for (int n = 0; n < Objects->Count; ++n) {
			if ((*Objects)[n]->Text == (*Letters)[i + 1]) {
				to = (*Objects)[n];
				break;
			}
		}
		
		TheGame->DrawLine(from->X + 32, from->Y + 32, to->X + 32, to->Y + 32, TheGame->ConnexGreen, 4.0f);
	}
	
	for (int i = 0; i < Objects->Count; ++i) {
		BPMiniGame_Connex_Object* obj = (*Objects)[i];
		TheGame->DrawString(obj->sfcText, BLACK, obj->X, obj->Y + 11);
	}

	// no room for the timer!
//	int TimePassed = TheGame->TickCount - TimeStarted;
//	TheGame->DrawString(TheGame->TicksToTime(TimePassed), NORMAL, WHITE, 0, 267, 320, 40, CENTRED);
}

void BPMiniGame_Connex::Tick() {
	if (CompleteLetters == Letters->Count - 1) {
		Success();
	}
}

void BPMiniGame_Connex::OnMouseDown() {
	
}

void BPMiniGame_Connex::OnMouseMove() {
	
}

void BPMiniGame_Connex::OnMouseUp() {
	if (CompleteLetters == Letters->Count - 1) {
		// this game is finished!
		return;
	}
	
	if (LastLetter == -1) {
		// this was their first click
		LastLetter = GetLetter(TouchEvent);
		
		// must start with the very first letter
		if (LastLetter != 0) {
			MessageBox::Show("That's not the right box! Remember, you need to alternate between letters and numbers, which means you're looking to find A to begin with, then 1, then B, etc.", GameTitle);
			LastLetter = -1;
		} else {
			TheGame->PlaySound("card_flip");
		}
		
		return;
	}
	
	// if we're still here, they clicked another letter already - is it the right one?
	int CurrentLetter = GetLetter(TouchEvent);
	
	if (CurrentLetter == LastLetter + 1) {
		TheGame->PlaySound("slide");
		++CompleteLetters;
		LastLetter = CurrentLetter;
	}
	
	if (CompleteLetters == Letters->Count - 1) {
		// this game is finished!
		return;
	}
}

int BPMiniGame_Connex::GetLetter(BPPoint e) {
	for (int i = 0; i < Objects->Count; ++i) {
		BPMiniGame_Connex_Object* obj = (*Objects)[i];

		if (TheGame->PointOverRect(e.X, e.Y, obj->X, obj->Y, 64, 64)) {
			return Letters->IndexOf(obj->Text);
		}
	}
	
	return -1;
}
