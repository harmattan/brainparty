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

#include "rps.h"
#include "Minigame.h"

BPMiniGame_RPS::BPMiniGame_RPS(BPGame* game) : BPMiniGame(game) {
	sfcBackground = TheGame->LoadBitmap("rockpaperscissors", 320, 416);
	sfcLose = TheGame->LoadBitmap("rockpaperscissors_loses", 320, 64);
	
	sfcRock = TheGame->LoadBitmap("rock", 90, 90);
	sfcPaper = TheGame->LoadBitmap("paper", 90, 90);
	sfcScissors = TheGame->LoadBitmap("scissors", 90, 90);
		
	LastStateChange = -1;
	GameState = GUESSING;
	
	SuccessTime = -1;
	
	CurrentPosition = NULL;
	PositionNum = MustWin = Tries = NumWrong = TimeStarted = 0;
	
	GameTitle = "Rock vs Paper";
	GameHelp = "Paper beats rock, scissors beats paper and rock beats scissors - choose the right play each time. But here's the twist: sometimes you have to lose!";
	GameHelp2 = "You either have to WIN or you have to LOSE, so read the on-screen instructions carefully. When you have to win, your opponent will make a move (eg scissors) and you have to beat it. But when you have to lose, you need to choose whichever move would lose against your opponent's move.";
	
	MiniGameType = PUZZLE;
	
	Positions.Add(new BPMiniGame_RPS_Position("Rock", "Scissors", "Paper", sfcRock));
	Positions.Add(new BPMiniGame_RPS_Position("Paper", "Rock", "Scissors", sfcPaper));
	Positions.Add(new BPMiniGame_RPS_Position("Scissors", "Paper", "Rock", sfcScissors));
		
	Play();
}

BPMiniGame_RPS::~BPMiniGame_RPS() {
	SAFE_DELETE(sfcBackground);
	SAFE_DELETE(sfcLose);
	SAFE_DELETE(sfcRock);
	SAFE_DELETE(sfcPaper);
	SAFE_DELETE(sfcScissors);
	
	Positions.Clear();
}

void BPMiniGame_RPS::Play() {
	if (Tries >= 10) return;
	
	Positions.Shuffle();
	
	if (TheGame->RandomRange(0, 7) % 2 == 0) {
		MustWin = true;
	} else {
		MustWin = false;
	}
	
	int newpos;
	
	if (PositionNum == -1) {
		newpos = TheGame->RandomRange(0, 2);
	} else {
		newpos = TheGame->RandomRangeExcept(0, 2, PositionNum);
	}
	
	PositionNum = newpos;
	CurrentPosition = Positions[newpos];
	
	GameState = GUESSING;
}

void BPMiniGame_RPS::Start() {
	TimeStarted = TheGame->TickCount;
}

int BPMiniGame_RPS::GetWeight() {
	int TimePassed = (TheGame->TickCount - TimeStarted) / 1000.0f;
	return MinMax(500 - (NumWrong * 60) - round(TimePassed * 1.55f));
}

void BPMiniGame_RPS::Render() {
	TheGame->DrawImage(sfcBackground, 0, 0);
	
	if (!MustWin) TheGame->DrawImage(sfcLose, 0, 229);
	
	TheGame->DrawImage(CurrentPosition->Sfc, 110, 89);
	
	TheGame->DrawImage(Positions[0]->Sfc, 15, 307);
	TheGame->DrawImage(Positions[1]->Sfc, 115, 307);
	TheGame->DrawImage(Positions[2]->Sfc, 215, 307);
	
	if (GameState == CORRECT) {
		RenderCorrect();
	} else if (GameState == WRONG) {
		RenderWrong();
	}
}

void BPMiniGame_RPS::Tick() {
	if (SuccessTime != -1 && SuccessTime + 250 < TheGame->TickCount) {
		Success();
	}
	
	if (LastStateChange != -1 && LastStateChange + 750 < TheGame->TickCount) {
		LastStateChange = -1;
		Play();
	}
}

void BPMiniGame_RPS::OnMouseDown() {
	
}

void BPMiniGame_RPS::OnMouseMove() {
	
}

void BPMiniGame_RPS::OnMouseUp() {
	if (GameState == CORRECT || GameState == WRONG) return;
	
	BPMiniGame_RPS_Position* chosenposition = NULL;
		
	if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 15, 307, 90, 90)) {
		chosenposition = Positions[0];
	} else if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 115, 307, 90, 90)) {
		chosenposition = Positions[1];
	} else if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 215, 307, 90, 90)) {
		chosenposition = Positions[2];
	} else if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 110, 89, 90, 90)) {
		if (MustWin) {
			MessageBox::Show("This shows the move an opponent has played. You have been asked to choose the move that beats your opponent, so you need to tap one of the three pictures below.", GameTitle);
		} else {
			MessageBox::Show("This shows the move an opponent has played. You have been asked to choose the move that loses to your opponent, so you need to tap one of the three pictures below.", GameTitle);
		}
	} else if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 0, 229, 320, 64)) {
		if (MustWin) {
			MessageBox::Show("You need to choose the right move to beat your opponent - which of the three below is the correct answer?", GameTitle);
		} else {
			MessageBox::Show("You need to choose the right move to lose to your opponent - which of the three below is the correct answer?", GameTitle);
		}
	}
	
	if (chosenposition == NULL) return;
	
	++Tries;
	
	if (MustWin) {
		if (CurrentPosition->BeatenBy == chosenposition->Position) {
			TheGame->PlaySound("correct");
			GameState = CORRECT;
			LastStateChange = TheGame->TickCount;
		} else {
			TheGame->PlaySound("wrong");
			++NumWrong;
			GameState = WRONG;
			LastStateChange = TheGame->TickCount;
		}
	} else {
		if (CurrentPosition->Beats == chosenposition->Position) {
			TheGame->PlaySound("correct");
			GameState = CORRECT;
			LastStateChange = TheGame->TickCount;
		} else {
			TheGame->PlaySound("wrong");
			++NumWrong;
			GameState = WRONG;
			LastStateChange = TheGame->TickCount;
		}
	}
	
	UpdateScore();
}

void BPMiniGame_RPS::UpdateScore() {
	if (Tries < 0) Tries = 0;
	
	if (Tries >= 10 && SuccessTime == -1) {
		SuccessTime = TheGame->TickCount ;
	}
}
