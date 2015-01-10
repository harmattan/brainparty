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

#include "memorymaths.h"
#include "Minigame.h"	

BPMiniGame_MemoryMaths::BPMiniGame_MemoryMaths(BPGame* game) : BPMiniGame(game) {
	sfcBackground = TheGame->LoadBitmap("memorymaths", 320, 416);
	sfcRemember = TheGame->LoadBitmap("memorymaths_remember", 320, 416);
		
	TimeStarted = Result = NumWrong = NumTries = CurrentLevel = LastOp = 0;
	LastStateChange = -1;
	SuccessTime = -1;
	
	sfcCurrentAnswer = NULL;
	CurrentAnswer = new string();
	
	LastWrong = false; // when set to be true, re-show the "Remember" screen regardless of the score and don't increment the level counter
	
	GameTitle = "Memory Maths";
	GameHelp = "I'm going to assign a number to different food, and you need to remember how much each food item is worth, then answer some simple questions. Tap OK to submit your answer, or tap the answer box to clear it.";
	GameHelp2 = "You'll be shown a list of foods, and each food will have a number assigned to it. When you're ready, tap the screen, and a question will be asked based around those food numbers, eg \"What is banana multiplied by banana?\" Remember: tap the answer box if you need to clear.";
	
	MiniGameType = PUZZLE;
	
	Food.Add(TheGame->LoadBitmap("card_1", 64, 64));
	Food.Add(TheGame->LoadBitmap("card_2", 64, 64));
	Food.Add(TheGame->LoadBitmap("card_3", 64, 64));
	Food.Add(TheGame->LoadBitmap("card_4", 64, 64));
	Food.Add(TheGame->LoadBitmap("card_5", 64, 64));
	Food.Add(TheGame->LoadBitmap("card_6", 64, 64));
	Food.Add(TheGame->LoadBitmap("card_7", 64, 64));
	Food.Add(TheGame->LoadBitmap("card_8", 64, 64));
	
	sfcNumbers.Add(NULL);
	
	for (int i = 1; i < 10; ++i) {
		Numbers.Add(i);
		
		SpriteFont* tex = NULL;
		ostringstream number;
		number << "= " << i;
		TheGame->AllocString(&tex, number.str().c_str(), XLARGE, 64, 64, CENTRED, true);
		sfcNumbers.Add(tex);
	}
	
	Food.Shuffle();
	Numbers.Shuffle();
	
	SpriteFont* tex1 = NULL;
	TheGame->AllocString(&tex1, "+", XLARGE, 30, 40, CENTRED, true);
	sfcOperators["+"] = tex1;


	SpriteFont* tex2 = NULL;
	TheGame->AllocString(&tex2, "-", XLARGE, 30, 40, CENTRED, true);
	sfcOperators["-"] = tex2;


	SpriteFont* tex3 = NULL;
	TheGame->AllocString(&tex3, "x", XLARGE, 30, 40, CENTRED, true);
	sfcOperators["X"] = tex3;
	
	LevelUp();
}

BPMiniGame_MemoryMaths::~BPMiniGame_MemoryMaths() {
	SAFE_DELETE(sfcBackground);
	SAFE_DELETE(sfcRemember);
	Food.Clear();
	Numbers.Clear();
	FoodNumbers.Clear();
	EquationFood.Clear();
	EquationOps.Clear();
	sfcNumbers.Clear();
	
	for(map<const char*, SpriteFont*>::iterator it = sfcOperators.begin(); it != sfcOperators.end(); ++it) {
		SAFE_DELETE(it->second);
	}
	
	SAFE_DELETE(CurrentAnswer);
	SAFE_DELETE(sfcCurrentAnswer);
}

void BPMiniGame_MemoryMaths::OnMouseDown() {
	
}

void BPMiniGame_MemoryMaths::OnMouseMove() {
	
}

void BPMiniGame_MemoryMaths::OnMouseUp() {
	switch (GameState) {
		case REMEMBER:
			CurrentAnswer->clear();
			AnswerChanged();
			GameState = GUESSING;
			break;
			
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

void BPMiniGame_MemoryMaths::ButtonClicked(const char* btn) {
	if (CurrentAnswer->length() > 8) return;
		
	CurrentAnswer->append(btn);
	AnswerChanged();
	
	TheGame->PlaySound("mouse_click");
}

void BPMiniGame_MemoryMaths::FlipSign() {
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

void BPMiniGame_MemoryMaths::CheckAnswer() {

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

void BPMiniGame_MemoryMaths::LevelUp() {
	if (!LastWrong) ++CurrentLevel;
	CurrentAnswer->clear();
	AnswerChanged();
	++NumTries;
	
	if (NumTries >= 10) {
		if (SuccessTime == -1) SuccessTime = TheGame->TickCount;
		return;
	}
	
	EquationFood.Clear();
	EquationOps.Clear();
	
	if (LastWrong) {
		// the player got the answer wrong; don't add any more food regardless of the level,
		// but re-show the current food numbers instead to give them a reminder
		GameState = REMEMBER;
		LastOp = TheGame->RandomRange(0, 2); // start with a random operator
	} else {
		switch (CurrentLevel) {
			case 1:
				FoodNumbers.Add(0);
				FoodNumbers.Add(1);
				GameState = REMEMBER;
				LastOp = TheGame->RandomRange(0, 2); // start with a random operator
				
				break;
				
			case 4:
				FoodNumbers.Add(2);
				GameState = REMEMBER;
				LastOp = TheGame->RandomRange(0, 2); // start with a random operator
				break;
				
			case 7:
				FoodNumbers.Add(3);
				GameState = REMEMBER;
				LastOp = TheGame->RandomRange(0, 2); // start with a random operator
				break;
				
			case 9:
				FoodNumbers.Add(4);
				GameState = REMEMBER;
				LastOp = TheGame->RandomRange(0, 2); // start with a random operator
				break;
				
			default:
				GameState = GUESSING;
				break;
		}
	}
	
	if (CurrentLevel < 6) {
		AddFood();
		AddOp();
		AddFood();
	} else {
		AddFood();
		AddOp();
		AddFood();
		AddOp();
		AddFood();
	}
	
	Solve();
	
	LastStateChange = TheGame->TickCount;
	LastWrong = false; // clear this flag
}

void BPMiniGame_MemoryMaths::AddFood() {
	EquationFood.Add(TheGame->RandomRange(0, FoodNumbers.Count - 1));
}

void BPMiniGame_MemoryMaths::AddOp() {
	switch (LastOp) {
		case 0:
			EquationOps.Add("+");
			break;
			
		case 1:
			EquationOps.Add("-");
			break;
			
		case 2:
			if (EquationOps.Count > 0) {
				// the multiply operator has a higher precedent than addition or subtraction
				// yet most people will solve the equation left to right and thus get the wrong answer
				// if multiply is used as the second operator. To solve this, never allow the
				// multiply operator to be used if it isn't the first operator
				EquationOps.Add("+");
			} else {
				EquationOps.Add("X");
			}
			break;
	}
	
	++LastOp;
	if (LastOp > 2) LastOp = 0;
}

void BPMiniGame_MemoryMaths::Solve() {
	Result = 0;
	
	switch (EquationFood.Count) {
		case 2:
			if (EquationOps[0] == "+") {
				Result = Numbers[EquationFood[0]] + Numbers[EquationFood[1]];
			} else if (EquationOps[0] == "-") {
				Result = Numbers[EquationFood[0]] - Numbers[EquationFood[1]];
			} else {
				Result = Numbers[EquationFood[0]] * Numbers[EquationFood[1]];				
			}
			
			break;
		case 3:
			if (EquationOps[0] == "X") {
				Result = Numbers[EquationFood[0]] * Numbers[EquationFood[1]];
			} else if (EquationOps[0] == "+") {
				Result = Numbers[EquationFood[0]] + Numbers[EquationFood[1]];
			} else {
				Result = Numbers[EquationFood[0]] - Numbers[EquationFood[1]];
			}
			
			if (EquationOps[1] == "+") {
				Result += Numbers[EquationFood[2]];
			} else {
				Result -= Numbers[EquationFood[2]];
			}

			break;
	}
}

void BPMiniGame_MemoryMaths::Start() {
	TimeStarted = TheGame->TickCount;
}

int BPMiniGame_MemoryMaths::GetWeight() {
	float TimePassed = (TheGame->TickCount - TimeStarted) / 1000.0f;
	return MinMax((550 - (NumWrong * 75))  - round(TimePassed));
}

void BPMiniGame_MemoryMaths::Render() {
	switch (GameState) {
		case REMEMBER:
			TheGame->DrawImage(sfcRemember, 0, 0);
			
			for (int i = 0; i < FoodNumbers.Count; ++i) {
				int foodnum = FoodNumbers[i];
				TheGame->DrawImage(Food[foodnum], 98, 54 + (70 * i));
				TheGame->DrawString(sfcNumbers[Numbers[foodnum]], WHITE, 168, 61 + (70 * i));
				
				if (i == 5) {
					break;
				}
			}
			
			break;
			
		default:
			TheGame->DrawImage(sfcBackground, 0, 0);
			
			// this stops an equation being shown once we've won the game
			if (SuccessTime == -1) {
				switch (EquationFood.Count) {
					case 2:
						TheGame->DrawImage(Food[EquationFood[0]], 73, 16);
						TheGame->DrawString(sfcOperators[EquationOps[0]], WHITE, 144, 25);
						TheGame->DrawImage(Food[EquationFood[1]], 183, 16);
						break;
						
					case 3:
						TheGame->DrawImage(Food[EquationFood[0]], 18, 16);
						TheGame->DrawString(sfcOperators[EquationOps[0]], WHITE, 90, 23);
						TheGame->DrawImage(Food[EquationFood[1]], 128, 16);
						TheGame->DrawString(sfcOperators[EquationOps[1]], WHITE, 200, 23);
						TheGame->DrawImage(Food[EquationFood[2]], 238, 16);
						break;
				}
				
				TheGame->DrawString(sfcCurrentAnswer, WHITE, 18, 110);
				
				if (GameState == CORRECT) {
					RenderCorrect();
				} else if (GameState == WRONG) {
					RenderWrong();
				}
			}
			
			break;
	}
}

void BPMiniGame_MemoryMaths::Tick() {
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
			LastWrong = true;
			LevelUp();
		}
	}
}

void BPMiniGame_MemoryMaths::AnswerChanged() {
	TheGame->AllocString(&sfcCurrentAnswer, CurrentAnswer->c_str(), XLARGE, 284, 64, RIGHT, true);
}
