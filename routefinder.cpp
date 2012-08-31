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

#include "routefinder.h"
#include "Minigame.h"

BPMiniGame_RouteFinder::BPMiniGame_RouteFinder(BPGame* game) : BPMiniGame(game) {
	sfcBackground = TheGame->LoadBitmap("routefinder", 320, 416);
	sfcSheep = TheGame->LoadBitmap("sheep_right", 38, 34);
	sfcRemember = TheGame->LoadBitmap("remember", 320, 416);
	
	sfcMoveUp = TheGame->LoadBitmap("arrow_up", 48, 48);
	sfcMoveDown = TheGame->LoadBitmap("arrow_down", 48, 48);
	sfcMoveLeft = TheGame->LoadBitmap("arrow_left", 48, 48);
	sfcMoveRight = TheGame->LoadBitmap("arrow_right", 48, 48);
		
	CurrentMove = -1;
	
	CurrentLevel = NumWrong = NumTries = SheepX = SheepY = LastStateChange = TimeStarted = 0;
	
	GameState = GUESSING;
		
	GameTitle = "Route Finder";
	GameHelp = "Remember the moves I'll show you, then tap once where the sheep will end up after it has made all those moves. Each arrow is one square of movement.";
	GameHelp2 = "You can only tap the screen once: in the FINAL position of the sheep. You can reduce the memory work by cancelling out moves. For example, if the moves are \"right, right, down, up, left, right\", then the down and up can be said to cancel each other out because the sheep ends up unmoved after the two moves have taken place.";
	
	MiniGameType = PUZZLE;
}

BPMiniGame_RouteFinder::~BPMiniGame_RouteFinder() {
	SAFE_DELETE(sfcBackground);
	SAFE_DELETE(sfcSheep);
	SAFE_DELETE(sfcRemember);
	
	SAFE_DELETE(sfcMoveUp);
	SAFE_DELETE(sfcMoveDown);
	SAFE_DELETE(sfcMoveLeft);
	SAFE_DELETE(sfcMoveRight);
		
	Moves.Clear();
}

void BPMiniGame_RouteFinder::Start() {
	LevelUp();
	
	TimeStarted = TheGame->TickCount;
}

int BPMiniGame_RouteFinder::GetWeight() {
	float TimePassed = (TheGame->TickCount - TimeStarted) / 1000.0f;
	return MinMax(560 - round(TimePassed * 1.5) - (NumWrong * 50));
}

void BPMiniGame_RouteFinder::Render() {
	switch (GameState) {
		case MOVES:
			TheGame->DrawImage(sfcRemember, 0, 0);
			
			for (int i = 0; i < Moves.Count; ++i) {
				switch (Moves[i]) {
					case MT_UP:
						TheGame->DrawImage(sfcMoveUp, 136, 53 + (i * 51));
						break;
					case MT_DOWN:
						TheGame->DrawImage(sfcMoveDown, 136, 53 + (i * 51));
						break;
					case MT_LEFT:
						TheGame->DrawImage(sfcMoveLeft, 136, 53 + (i * 51));
						break;
					case MT_RIGHT:
						TheGame->DrawImage(sfcMoveRight, 136, 53 + (i * 51));
						break;
				}
			}
			
			break;
			
		default:
			TheGame->DrawImage(sfcBackground, 0, 0);
			
			TheGame->DrawImage(sfcSheep, SheepX, SheepY);
			
			if (GameState == CORRECT) {
				RenderCorrect();
			} else if (GameState == WRONG) {
				RenderWrong();
			} 
			
			break;
	}
}

void BPMiniGame_RouteFinder::LevelUp() {
	GameState = MOVES;
	
	SheepX = 11 + (TheGame->RandomRange(0, 6) * 44);
	SheepY = 15 + (TheGame->RandomRange(0, 8) * 44);
	
	CurrentMove = -1;
	Moves.Clear();
	
	++CurrentLevel;
	
	switch (CurrentLevel) {
		case 1:
			AddMove();
			AddMove();
			break;
			
		case 2:
			AddMove();
			AddMove();
			AddMove();
			break;
			
		case 3:
			AddMove();
			AddMove();
			AddMove();
			AddMove();
			break;
			
		case 4:
			AddMove();
			AddMove();
			AddMove();
			AddMove();
			AddMove();
			break;
			
		default:
			AddMove();
			AddMove();
			AddMove();
			AddMove();
			AddMove();
			AddMove();
			break;
	}
}

void BPMiniGame_RouteFinder::Tick() {
	if (GameState == MOVING) {
		if (LastStateChange + 400 < TheGame->TickCount) {
			// we've waited a little, go ahead and move
			LastStateChange = TheGame->TickCount;
			++CurrentMove;
			
			if (CurrentMove == Moves.Count) {
				// all moves done - were they correct?
				CheckAnswer();
			} else {
				TheGame->PlaySound("soft_boom");
				
				switch (Moves[CurrentMove]) {
					case MT_UP:
						SheepY -= 44;
						break;
						
					case MT_DOWN:
						SheepY += 44;
						break;
						
					case MT_LEFT:
						SheepX -= 44;
						break;
						
					case MT_RIGHT:
						SheepX += 44;
						break;
				}
			}
		}
	}
	
	if (GameState == CORRECT && LastStateChange + 500 < TheGame->TickCount) {
		LevelUp();
	}
	
	if (GameState == WRONG && LastStateChange + 1000 < TheGame->TickCount) {
		--CurrentLevel;
		LevelUp();
	}
	
	if (GameState == SUCCESS && LastStateChange + 250 < TheGame->TickCount) {
		Success();
	}
	
	if (GameState == FAILURE && LastStateChange + 250 < TheGame->TickCount) {
		Failure();
	}
}

void BPMiniGame_RouteFinder::CheckAnswer() {
	if (TheGame->PointOverRect(GuessedPosition.X, GuessedPosition.Y, SheepX, SheepY, 64, 64)) {
		if (NumTries < 7) {
			TheGame->PlaySound("correct");
			GameState = CORRECT;
		} else {
			GameState = SUCCESS;
		}
	} else {
		TheGame->PlaySound("wrong");
		GameState = WRONG;
		++NumWrong;
	}
	
	++NumTries;
	LastStateChange = TheGame->TickCount;
}

void BPMiniGame_RouteFinder::OnMouseUp() {
	
}

void BPMiniGame_RouteFinder::OnMouseMove() {
	
}

void BPMiniGame_RouteFinder::OnMouseDown() {
	switch (GameState) {
		case MOVES:
			TheGame->PlaySound("card_flip2");
			GameState = GUESSING;
			break;
			
		case GUESSING:
			GuessedPosition = TouchEvent;
			GameState = MOVING;
			LastStateChange = TheGame->TickCount;
			break;
	}
}

void BPMiniGame_RouteFinder::AddMove() {
	int sheepx;
	int sheepy;
	
	if (Moves.Count == 0) {
		sheepx = SheepX;
		sheepy = SheepY;
	} else {
		BPPoint* sheeppos = CalculateSheepPos();
		sheepx = sheeppos->X;
		sheepy = sheeppos->Y;
	}
	
	MoveTypes excluded1 = MT_NONE;
	MoveTypes excluded2 = MT_NONE;
	
	if (sheepx <= 11) excluded1 = MT_LEFT;
	if (sheepx >= 275) excluded1 = MT_RIGHT;
	if (sheepy <= 15) excluded2 = MT_UP;
	if (sheepy >= 367) excluded2 = MT_DOWN;
	
	
	MoveTypes chosenmove = MT_NONE;
	
	do {
		switch (TheGame->RandomRange(1, 4)) {
			case 1:
				chosenmove = MT_UP;
				break;
			case 2:
				chosenmove = MT_DOWN;
				break;
			case 3:
				chosenmove = MT_LEFT;
				break;
			case 4:
				chosenmove = MT_RIGHT;
				break;
		}
	} while (chosenmove == excluded1 || chosenmove == excluded2);
	
	Moves.Add(chosenmove);
}

BPPoint* BPMiniGame_RouteFinder::CalculateSheepPos() {
	BPPoint* ret = new BPPoint(SheepX, SheepY);
	
	for (int i = 0; i < Moves.Count; ++i) {
		MoveTypes move = (MoveTypes)Moves[i];
		switch (move) {
			case MT_UP:
				ret->Y -= 44;
				break;
				
			case MT_DOWN:
				ret->Y += 44;
				break;
				
			case MT_LEFT:
				ret->X -= 44;
				break;
				
			case MT_RIGHT:
				ret->X += 44;
				break;
		}
	}
	
	return ret;
}
