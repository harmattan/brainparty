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

#include "underthehat.h"
#include "Minigame.h"
		
BPMiniGame_UnderTheHat::BPMiniGame_UnderTheHat(BPGame* game) : BPMiniGame(game) {
	sfcBackground = TheGame->LoadBitmap("checkers1", 320, 416);
	sfcBackground2 = TheGame->LoadBitmap("checkers2", 320, 416);
	sfcHat = TheGame->LoadBitmap("propeller", 94, 71);
	sfcBadHat = TheGame->LoadBitmap("propeller_bad", 94, 71);
	sfcItem = TheGame->LoadBitmap("gem6_small", 64, 46);
	
	ChosenHat = NULL;
	sfcCurrentLevel = NULL;
	
	CurrentLevel = Tries = MovesLeft = LastStateChange = TimeStarted = NumWrong = 0;
	
	SuccessTime = -1;
	MoveSpeed = 7;
	
	BackgroundFade = 0.0f;
	BackgroundUp = false;
	
	MoveAmount = 0.0f;
	
	State = SHOWING;
	
	GameTitle = "Under the Hat";
	GameHelp = "One hat has a gem underneath. When you're ready, tap the screen then watch the hats. When they stop, can you guess which hat covers the gem?";
	GameHelp2 = "Whether you get it right or wrong, the gem won't move to be under a different hat, so keep your eyes peeled!";
	
	MiniGameType = PUZZLE;
	
	Points.Add(new BPPoint(7, 11));
	Points.Add(new BPPoint(112, 11));
	Points.Add(new BPPoint(218, 11));
	
	Points.Add(new BPPoint(60, 91));
	Points.Add(new BPPoint(166, 91));
	
	Points.Add(new BPPoint(7, 171));
	Points.Add(new BPPoint(112, 171));
	Points.Add(new BPPoint(218, 171));
	
	Points.Add(new BPPoint(60, 251));
	Points.Add(new BPPoint(166, 251));
	Points.Shuffle();
	
	for (int i = 0; i < 5; ++i) {
		BPPoint* point = Points[i];
		
		BPMiniGame_UnderTheHat_Hat* hat = new BPMiniGame_UnderTheHat_Hat();
		hat->X = point->X;
		hat->Y = point->Y;
		hat->DestX = point->X;
		hat->DestY = point->Y;
		
		Hats.Add(hat);
	}
}

BPMiniGame_UnderTheHat::~BPMiniGame_UnderTheHat() {
	SAFE_DELETE(sfcBackground);
	SAFE_DELETE(sfcBackground2);
	SAFE_DELETE(sfcHat);
	SAFE_DELETE(sfcBadHat);
	SAFE_DELETE(sfcItem);
	
	Hats.Clear();
	Points.Clear();
	
	SAFE_DELETE(ChosenHat);
	
	SAFE_DELETE(sfcCurrentLevel);
}

void BPMiniGame_UnderTheHat::Start() {
	LevelUp();
	
	TimeStarted = TheGame->TickCount;
}

int BPMiniGame_UnderTheHat::GetWeight() {
	return MinMax(500 - (NumWrong * 150));
}

void BPMiniGame_UnderTheHat::Render() {
	TheGame->DrawImage(sfcBackground, 0, 0);
	
	glColor4f(1.0f, 1.0f, 1.0f, TheGame->SmoothStep(0, 1.0f, BackgroundFade));
	TheGame->DrawImage(sfcBackground2, 0, 0);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	
	for (int i = 0; i < Hats.Count; ++i) {
		BPMiniGame_UnderTheHat_Hat* hat = Hats[i];
		
		if (State == WRONG && hat == ChosenHat) {
			TheGame->DrawImage(sfcBadHat, hat->X, hat->Y);
		} else {
			TheGame->DrawImage(sfcHat, hat->X, hat->Y);			
		}
	}
	
	if (State == SHOWING || State == CORRECT || State == WRONG) {
		TheGame->DrawImage(sfcItem, Hats[0]->X + 21, Hats[0]->Y + 25);
	}
	
	if (State == CORRECT) {
		if (LastStateChange + 1000 > TheGame->TickCount) {
			RenderCorrect();
		}
	} else if (State == WRONG) {
		if (LastStateChange + 1000 > TheGame->TickCount) {
			RenderWrong();
		}
	}
	
	TheGame->DrawString(sfcCurrentLevel, WHITE, 0, 360);
}

void BPMiniGame_UnderTheHat::Tick() {
	if (SuccessTime != -1 && SuccessTime + 250 < TheGame->TickCount) {
		Success();
		return;
	}
	
	if (BackgroundUp) {
		BackgroundFade += 0.0025f;
		if (BackgroundFade >= 1.0f) {
			BackgroundUp = false;
		}
	} else {
		BackgroundFade -= 0.0025f;
		
		if (BackgroundFade <= 0.0f) {
			BackgroundUp = true;
		}
	}
	
	switch (State) {
		case SHOWING:
			break;
			
		case WAITING:
			if (LastStateChange + 1000 < TheGame->TickCount) {
				State = MOVING;
				MoveAmount = 0.0f;
			}
			break;
			
		case CORRECT:
			if (LastStateChange + 1750 < TheGame->TickCount) {
				LevelUp();
				State = MOVING;
			}
			
			break;
			
		case WRONG:
			if (LastStateChange + 1750 < TheGame->TickCount) {
				GenerateMoves();
				State = MOVING;
			}
			
			break;
			
		case MOVING:
			if (MoveAmount < 1.0f) {
				MoveAmount += 0.1f * TheGame->ElapsedSeconds * MoveSpeed;
				
				for (int i = 0; i < Hats.Count; ++i) {
					BPMiniGame_UnderTheHat_Hat* hat = Hats[i];
					hat->X = TheGame->SmoothStep(hat->StartX, hat->DestX, MoveAmount);
					hat->Y = TheGame->SmoothStep(hat->StartY, hat->DestY, MoveAmount);
				}
			} else {
				if (MovesLeft > 0) {
					// finished a previous move, but we have more moves to do
					MakeMove();
				} else {
					// finished moving - time to guess!
					State = GUESSING;
					ChosenHat = NULL;
				}
			}			
			break;
	}
}

void BPMiniGame_UnderTheHat::OnMouseDown() {
	
}

void BPMiniGame_UnderTheHat::OnMouseMove() {
	
}

void BPMiniGame_UnderTheHat::OnMouseUp() {
	switch (State) {
		case SHOWING:
			LastStateChange = TheGame->TickCount;
			State = WAITING;
			break;
			
		case WAITING:
			State = MOVING;
			break;
			
		case GUESSING:
			++Tries;
			
			if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, Hats[0]->X, Hats[0]->Y, 75, 75)) {
				TheGame->PlaySound("correct");
				State = CORRECT;
			} else {
				TheGame->PlaySound("wrong");
				State = WRONG;
				++NumWrong;
				--CurrentLevel;
				if (CurrentLevel < 1) CurrentLevel = 1;
			}
			
			LastStateChange = TheGame->TickCount;
			
			if (Tries >= 10 && SuccessTime == -1) {
				SuccessTime = TheGame->TickCount;
				return;
			}
			
			break;
	}
}

void BPMiniGame_UnderTheHat::MakeMove() {
	--MovesLeft;
	
	Points.Shuffle();
	
	for (int i = 0; i < Hats.Count; ++i) {
		BPMiniGame_UnderTheHat_Hat* hat = Hats[i];
		
		hat->StartX = hat->X;
		hat->StartY = hat->Y;
		hat->DestX = Points[i]->X;
		hat->DestY = Points[i]->Y;
	}
	
	MoveAmount = 0.0f;
}

void BPMiniGame_UnderTheHat::LevelUp() {
	if (Tries >= 10 && SuccessTime == -1) {
		SuccessTime = TheGame->TickCount;
		return;
	}
	
	++CurrentLevel;
	GenerateMoves();
	MakeMove();
	
	ostringstream level;
	level << "Level: " << CurrentLevel << "/10";
	TheGame->AllocString(&sfcCurrentLevel, level.str().c_str(), LARGE, 320, 35, CENTRED, true);
}

void BPMiniGame_UnderTheHat::GenerateMoves() {
	MovesLeft = CurrentLevel + 4;
	MoveSpeed = 9 + CurrentLevel;
}
