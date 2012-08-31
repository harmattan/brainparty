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

#include "flashcounting.h"
#include "Minigame.h"

BPMiniGame_FlashCounting::BPMiniGame_FlashCounting(BPGame* game) : BPMiniGame(game) {
	sfcBackground = TheGame->LoadBitmap("flashcounting", 320, 480);
	CurrentLevel = 0;
	
	Balls = new BPPList<BPMiniGame_FlashCounting_Ball*>();
	BallPics = new BPPList<Texture*>();
	BallIDs = new BPList<int>();
	Directions = new BPList<int>();
	
	Answer = 0;
	BallDirCount = 0; // used to assign random directions to balls
	
	LastStateChange = -1;
	GameState = WAITING;
	
	TimeStarted = NumWrong = NumTries = 0;
	
	GameTitle = "Flash Counting";
	GameHelp = "I'm going to fill up your screen with more and more gems. Can you count them all quickly, then tell me which gem you can see most of?";
	GameHelp2 = "This is a game that relies on your ability to count without thinking too much. Once you've counted how many of each gem there are, look at the list of gems at the top and tap the one there is most of.";

	MiniGameType = LIVELY;
	
	BallPics->Add(TheGame->LoadBitmap("gem1", 64, 64));
	BallPics->Add(TheGame->LoadBitmap("gem2", 64, 64));
	BallPics->Add(TheGame->LoadBitmap("gem3", 64, 64));
	BallPics->Add(TheGame->LoadBitmap("gem4", 64, 64));
	BallPics->Add(TheGame->LoadBitmap("gem5", 64, 64));
	
	for (int i = 0; i < BallPics->Count; ++i) {
		BallIDs->Add(i);
	}
	
	for (int i = 0; i < 360; i += 10) {
		Directions->Add(i);
	}
	
	LevelUp();
}

BPMiniGame_FlashCounting::~BPMiniGame_FlashCounting() {
	SAFE_DELETE(sfcBackground);
	
	Balls->Clear();
	SAFE_DELETE(Balls);
	
	BallPics->Clear();
	SAFE_DELETE(BallPics);
	
	BallIDs->Clear();
	SAFE_DELETE(BallIDs);
	
	Directions->Clear();
	SAFE_DELETE(Directions);
}

void BPMiniGame_FlashCounting::OnMouseDown() {
	
}

void BPMiniGame_FlashCounting::OnMouseMove() {
	
}

void BPMiniGame_FlashCounting::OnMouseUp() {
	if (GameState == CORRECT || GameState == WRONG) return;
	
	if (TouchEvent.Y < 100) {
		// a guess?
		for (int i = 0; i < BallIDs->Count; ++i) {
			if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, (i * 64), 28, 64, 64)) {
				// clicked!
				if (Answer == (*BallIDs)[i]) {
					TheGame->PlaySound("correct");
					GameState = CORRECT;
					LastStateChange = TheGame->TickCount;
				} else {
					TheGame->PlaySound("wrong");
					++NumWrong;
					GameState = WRONG;
					LastStateChange = TheGame->TickCount;
				}
				
				break;
			}
		}
	} else {
		MessageBox::Show("Count up all the gems you see here. Once you've figured out which gem is the most common, choose it from the list at the bottom of the screen.", GameTitle);
	}
}

void BPMiniGame_FlashCounting::Start() {
	TimeStarted = TheGame->TickCount;
}

int BPMiniGame_FlashCounting::GetWeight() {
	float TimePassed = (TheGame->TickCount - TimeStarted) / 1000.0f;
	return MinMax(600 - (NumWrong * 75) - floor(TimePassed * 3));
}

void BPMiniGame_FlashCounting::Render() {
	TheGame->DrawImage(sfcBackground, 0, 0);
	
	for (int i = 0; i < Balls->Count; ++i) {
		BPMiniGame_FlashCounting_Ball* ball = (*Balls)[i];
		TheGame->DrawImage((*BallPics)[ball->Type], round(ball->X), round(ball->Y));
	}
	
	for (int i = 0; i < BallIDs->Count; ++i) {
		TheGame->DrawImage((*BallPics)[(*BallIDs)[i]], (i * 64), 28);
	}
	
	if (GameState == CORRECT) {
		RenderCorrect();
	} else if (GameState == WRONG) {
		RenderWrong();
	}
}

void BPMiniGame_FlashCounting::Tick() {
	if (NumTries >= 12 && LastStateChange + 400 < TheGame->TickCount) {
		Success();
		return;
	}
	
	switch (GameState) {
		case CORRECT:
			if (LastStateChange != -1 && LastStateChange + 400 < TheGame->TickCount) {
				LevelUp();
			}
			
			break;
			
		case WRONG:			
			if (LastStateChange != -1 && LastStateChange + 400 < TheGame->TickCount) {
				--CurrentLevel;
				LevelUp();
			}
			
			break;
			
		case WAITING:
			for (int i = 0; i < Balls->Count; ++i) {
				BPMiniGame_FlashCounting_Ball* ball = (*Balls)[i];
				
				double xmove = cos(ball->Direction * M_PI / 180) * TheGame->ElapsedSeconds * 100;
				double ymove = sin(ball->Direction * M_PI / 180) * TheGame->ElapsedSeconds * 100;
				
				ball->X += xmove;
				ball->Y += ymove;
				
				if (ball->X > MiniGameWidth - BallWidth) {
					ball->X = MiniGameWidth - BallWidth;
					ball->Direction = 180 - ball->Direction;
				}
				
				if (ball->X < -1) {
					ball->X = -1;
					ball->Direction = 180 - ball->Direction;
				}
				
				if (ball->Y < Top) {
					ball->Y = Top;
					ball->Direction = 360 - ball->Direction;
				}
				
				if (ball->Y > Bottom - BallHeight) {
					ball->Y = Bottom - BallHeight;
					ball->Direction = 360 - ball->Direction;
				}
				
				if (ball->Direction >= 360) ball->Direction = ball->Direction - 360;
				if (ball->Direction < 0) ball->Direction = 360 - abs(ball->Direction);
			}
			
			break;
	}
	
}

void BPMiniGame_FlashCounting::LevelUp() {
	++NumTries;
	
	if (NumTries >= 12) {
		return;
	}
	
	LastStateChange = -1;
	++CurrentLevel;
	BallDirCount = 0;
	
	Balls->Clear();
	BallIDs->Shuffle();
	
	Answer = (*BallIDs)[0];
	
	switch (CurrentLevel) {
		case 1:
			CreateBall(0);
			CreateBall(0);
			CreateBall(0);
			CreateBall(1);
			CreateBall(1);
			CreateBall(2);
			break;
			
		case 2:
			for (int i = 0; i < 4; ++i) CreateBall(0);
			for (int i = 0; i < 3; ++i) CreateBall(1);
			break;
			
		case 3:
			for (int i = 0; i < 4; ++i) CreateBall(0);
			for (int i = 0; i < 2; ++i) CreateBall(1);
			for (int i = 0; i < 2; ++i) CreateBall(2);
			break;
			
		case 4:
			for (int i = 0; i < 5; ++i) CreateBall(0);
			for (int i = 0; i < 4; ++i) CreateBall(1);
			break;
			
		case 5:
			for (int i = 0; i < 4; ++i) CreateBall(0);
			for (int i = 0; i < 3; ++i) CreateBall(1);
			for (int i = 0; i < 3; ++i) CreateBall(2);
			break;
			
		case 6:
			for (int i = 0; i < 5; ++i) CreateBall(0);
			for (int i = 0; i < 4; ++i) CreateBall(1);
			for (int i = 0; i < 3; ++i) CreateBall(2);
			break;
			
		case 7:
			for (int i = 0; i < 5; ++i) CreateBall(0);
			for (int i = 0; i < 4; ++i) CreateBall(1);
			for (int i = 0; i < 4; ++i) CreateBall(2);
			break;
			
		case 8:
			for (int i = 0; i < 6; ++i) CreateBall(0);
			for (int i = 0; i < 5; ++i) CreateBall(1);
			for (int i = 0; i < 5; ++i) CreateBall(2);
			break;
			
		case 9:
			for (int i = 0; i < 6; ++i) CreateBall(0);
			for (int i = 0; i < 5; ++i) CreateBall(1);
			for (int i = 0; i < 5; ++i) CreateBall(2);
			for (int i = 0; i < 2; ++i) CreateBall(3);
			for (int i = 0; i < 2; ++i) CreateBall(4);
			break;
			
		case 10:
			for (int i = 0; i < 6; ++i) CreateBall(0);
			for (int i = 0; i < 5; ++i) CreateBall(1);
			for (int i = 0; i < 5; ++i) CreateBall(2);
			for (int i = 0; i < 3; ++i) CreateBall(3);
			for (int i = 0; i < 3; ++i) CreateBall(4);
			break;
			
		default:
			for (int i = 0; i < 6; ++i) CreateBall(0);
			for (int i = 0; i < 5; ++i) CreateBall(1);
			for (int i = 0; i < 5; ++i) CreateBall(2);
			for (int i = 0; i < 5; ++i) CreateBall(3);
			for (int i = 0; i < 5; ++i) CreateBall(4);
			break;
	}
	
	BallIDs->Shuffle();
	
	GameState = WAITING;
}

void BPMiniGame_FlashCounting::CreateBall(int type) {
	BPMiniGame_FlashCounting_Ball* ball = new BPMiniGame_FlashCounting_Ball();
	ball->X = TheGame->RandomRange(0, 208);
	ball->Y = TheGame->RandomRange(Top, Bottom - BallHeight);
	ball->Direction = (*Directions)[BallDirCount];
	ball->Type = (*BallIDs)[type];
	
	Balls->Add(ball);
	++BallDirCount;
}
