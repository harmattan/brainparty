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

#include "nextinline.h"
#include "Minigame.h"	

BPMiniGame_NextInLine::BPMiniGame_NextInLine(BPGame* game) : BPMiniGame(game) {
	sfcBackground = TheGame->LoadBitmap("nextinline", 320, 416);
	
	TimeStarted = Result = NumWrong = NumTries = 0;
	LastStateChange = -1;
	SuccessTime = -1;
	
	sfcCurrentQuestion = sfcCurrentAnswer = NULL;
	
	CurrentAnswer = new string();
	
	GameTitle = "Next in Line";
	GameHelp = "If 2 becomes 4 and 4 becomes 8, then what does 8 become? 16, of course - but it gets harder! Tap OK to submit your answer, or tap the answer box to clear it. Good luck!";
	GameHelp2 = "Have the numbers been multiplied by themselves? Have they been multiplied by the previous number? Think hard - there's a rule that matches them all...";

	for (int i = 0; i < 12; ++i) {
		QuestionOrder.Add(i);
	}
	
	QuestionOrder.Shuffle();
	
	MiniGameType = PUZZLE;
		
	LevelUp();
}

BPMiniGame_NextInLine::~BPMiniGame_NextInLine() {
	SAFE_DELETE(sfcBackground);
	
	SAFE_DELETE(CurrentAnswer);
	SAFE_DELETE(sfcCurrentQuestion);
	SAFE_DELETE(sfcCurrentAnswer);
}

void BPMiniGame_NextInLine::OnMouseDown() {
	
}

void BPMiniGame_NextInLine::OnMouseMove() {
	
}

void BPMiniGame_NextInLine::OnMouseUp() {
	switch (GameState) {
		case GUESSING:
			if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 10, 351, 96, 50)) {
				ButtonClicked("0");
			} else if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 10, 290, 96, 50)) {
				ButtonClicked("1");
			} else if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 113, 290, 96, 50)) {
				ButtonClicked("2");
			} else if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 215, 290, 96, 50)) {
				ButtonClicked("3");
			} else if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 10, 230, 96, 50)) {
				ButtonClicked("4");
			} else if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 113, 230, 96, 50)) {
				ButtonClicked("5");
			} else if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 215, 230, 96, 50)) {
				ButtonClicked("6");
			} else if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 10, 169, 96, 50)) {
				ButtonClicked("7");
			} else if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 113, 169, 96, 50)) {
				ButtonClicked("8");
			} else if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 215, 169, 96, 50)) {
				ButtonClicked("9");
			} else if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 113, 351, 96, 50)) {
				FlipSign();
			} else if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 215, 351, 96, 50)) {
				if (CurrentAnswer->length() == 0) return;
				if (CurrentAnswer->compare("-") == 0) return;
				CheckAnswer();
			} else if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 10, 107, 300, 50)) {
				CurrentAnswer->clear();
				AnswerChanged();
				TheGame->PlaySound("wrong");
			}
			
			break;
	}
}

void BPMiniGame_NextInLine::ButtonClicked(const char* btn) {
	if (CurrentAnswer->length() > 8) return;
	
	CurrentAnswer->append(btn);
	AnswerChanged();
	
	TheGame->PlaySound("mouse_click");
}

void BPMiniGame_NextInLine::FlipSign() {
	if (CurrentAnswer->length() == 0) {
		CurrentAnswer->append("-");
		AnswerChanged();
		return;
	}
	
	if (CurrentAnswer->substr(0, 1) == "-") {
		CurrentAnswer->erase(0, 1);
	} else {
		CurrentAnswer->insert(0, "-");
	}
	
	AnswerChanged();
}

void BPMiniGame_NextInLine::CheckAnswer() {
	if (atoi(CurrentAnswer->c_str()) == Result) {
		TheGame->PlaySound("correct");
		GameState = CORRECT;
	} else {
		TheGame->PlaySound("wrong");
		GameState = WRONG;
		++NumWrong;
	}

	LastStateChange = TheGame->TickCount;
}

void BPMiniGame_NextInLine::LevelUp() {
	CurrentAnswer->clear();
	AnswerChanged();
	++NumTries;
	GameState = GUESSING;
	
	if (NumTries >= 10) {
		if (SuccessTime == -1) SuccessTime = TheGame->TickCount;
		return;
	}
	
	ostringstream question;
	
	int first, second, third, fourth;

	first = TheGame->RandomRange(2, 4);
	second = TheGame->RandomRange(first + 1, first + 3);
	third = TheGame->RandomRange(second + 1, second + 3);
	fourth = TheGame->RandomRange(third + 1, third + 3);	
	
	switch (QuestionOrder[NumTries]) {
		case 0:
		{
			
			question << "If " << first << " is " << first + first << ", " << second << " is " << second + second << ", and " << third << " is " << third + third << ", what is " << fourth << "?";
			Result = fourth + fourth;
			break;
		}
			
		case 1:
		{
			question << "If " << first << " is " << first * first << ", " << second << " is " << second * second << ", and " << third << " is " << third * third << ", what is " << fourth << "?";
			Result = fourth * fourth;				
			break;
		}
		case 2:
		{
			question << "If " << first << " is " << first * 3 << ", " << second << " is " << second * 3 << ", and " << third << " is " << third * 3 << ", what is " << fourth << "?";
			Result = fourth * 3;
			break;
		}			
		case 3:
		{
			question << "If " << first << " is " << first + first - 1 << ", " << second << " is " << second + second - 1 << ", and " << third << " is " << third + third - 1 << ", what is " << fourth << "?";
			Result = fourth + fourth - 1;
			
			break;
		}
		case 4:
		{
			question << "If " << first << " is " << first + first + 1 << ", " << second << " is " << second + second + 1 << ", and " << third << " is " << third + third + 1 << ", what is " << fourth << "?";
			Result = fourth + fourth + 1;
			break;
		}
		case 5:
		{
			question << "If " << first << " is " << first * 4 << ", " << second << " is " << second * 4 << ", and " << third << " is " << third * 4 << ", what is " << fourth << "?";
			Result = fourth * 4;
			
			break;
		}
		case 6:
		{
			question << "If " << first * first << " is " << first << ", " << second * second << " is " << second << ", and " << third * third << " is " << third << ", what is " << fourth * fourth << "?";
			Result = fourth;
			break;
		}
		case 7:
		{
			question << "If " << first << " is " << first * (first - 1) << ", " << second << " is " << second * (second - 1) << ", and " << third << " is " << third * (third - 1) << ", what is " << fourth << "?";
			Result = fourth * (fourth - 1);
			
			break;
		}
		case 8:
		{
			question << "If " << first << " is " << first << ", " << second << " is " << second + first << ", and " << third << " is " << third + second + first << ", what is " << fourth << "?";
			Result = fourth + third + second + first;
			
			break;
		}
		case 9:
		{
			question << "If " << first << " is " << first * fourth << ", " << second << " is " << second * fourth << ", and " << third << " is " << third * fourth << ", what is " << fourth << "?";
			Result = fourth * fourth;
			
			break;
		}
		case 10:
		{
			question << "If " << first << " is " << first + 20 << ", " << second << " is " << second + 40 << ", and " << third << " is " << third + 60 << ", what is " << fourth << "?";
			Result = fourth + 80;
			
			break;
		}
		case 11:
		{
			question << "If " << first << " is " << first - 10 << ", " << second << " is " << second - 10 << ", and " << third << " is " << third - 10 << ", what is " << fourth << "?";
			Result = fourth - 10;
			
			break;
		}
	}
	
	TheGame->AllocString(&sfcCurrentQuestion, question.str().c_str(), NORMAL, 300, 80, LEFT, true);	
	TheGame->AllocString(&sfcCurrentAnswer, CurrentAnswer->c_str(), XLARGE, 284, 64, RIGHT, true);
	LastStateChange = TheGame->TickCount;

}

void BPMiniGame_NextInLine::Start() {
	TimeStarted = TheGame->TickCount;
}

int BPMiniGame_NextInLine::GetWeight() {
	float TimePassed = (TheGame->TickCount - TimeStarted) / 1000.0f;
	return MinMax((550 - (NumWrong * 75))  - round(TimePassed));
}

void BPMiniGame_NextInLine::Render() {
	TheGame->DrawImage(sfcBackground, 0, 0);
	
	// this stops an equation being shown once we've won the game
	if (SuccessTime == -1) {
		TheGame->DrawString(sfcCurrentQuestion, WHITE, 10, 16);
		
		TheGame->DrawString(sfcCurrentAnswer, WHITE, 18, 110);
		
		if (GameState == CORRECT) {
			RenderCorrect();
		} else if (GameState == WRONG) {
			RenderWrong();
		}
	}
}

void BPMiniGame_NextInLine::Tick() {
	if (SuccessTime != -1 && SuccessTime + 500 < TheGame->TickCount) {
		Success();
		return;
	}
	
	if (GameState == CORRECT) {
		if (LastStateChange + 500 < TheGame->TickCount) {
			LevelUp();
		}
	}
	
	if (GameState == WRONG) {
		if (LastStateChange + 500 < TheGame->TickCount) {
			LevelUp();
		}
	}
}

void BPMiniGame_NextInLine::AnswerChanged() {
	TheGame->AllocString(&sfcCurrentAnswer, CurrentAnswer->c_str(), XLARGE, 284, 64, RIGHT, true);
}
