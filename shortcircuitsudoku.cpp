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

#include "shortcircuitsudoku.h"
#include "Minigame.h"

BPMiniGame_ShortCircuitSudoku::BPMiniGame_ShortCircuitSudoku(BPGame* game) : BPMiniGame(game) {
	sfcBackground = TheGame->LoadBitmap("shortcircuitsudoku", 320, 416);
	
	LastStateChange = NumTries = CurrentLevel = Score = TimeStarted = 0;
	GameState = WAITING;
	GuessSquare = NULL;
	sfcQuestionMark = NULL;

	GameTitle = "Short Sudoku";
	GameHelp = "Can you figure out what number goes in the yellow squares in these Sudoku grids? If you get stuck, tap one of the question marks for a hint!";
	GameHelp2 = "If the square is the only missing number in its row or column, then the answer is easy - you just need to see which number is missing. Alternatively, if the box above has a 2 to the left and the box below has a 2 to the right, then the current box must have a 2 in the middle column.";
	
	MiniGameType = PUZZLE;
	
	TheGame->AllocString(&sfcQuestionMark, "?", NORMAL, 24, 24, CENTRED);
}

BPMiniGame_ShortCircuitSudoku::~BPMiniGame_ShortCircuitSudoku() {
	SAFE_DELETE(sfcBackground);
	
	Sudoku.Clear();
	
	SAFE_DELETE(sfcQuestionMark);
}

void BPMiniGame_ShortCircuitSudoku::Start() {
	TimeStarted = TheGame->TickCount;
	LevelUp();
}

int BPMiniGame_ShortCircuitSudoku::GetWeight() {
	float TimePassed = (TheGame->TickCount - TimeStarted) / 1000.0f;
	return MinMax(Score - round(TimePassed / 4));
}

void BPMiniGame_ShortCircuitSudoku::Render() {
	TheGame->DrawImage(sfcBackground, 0, 0);
	
	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 9; ++j) {
			SudokuSquare* square = Sudoku[(i * 9) + j];
			
			if (square == GuessSquare) {
				TheGame->FillRectangle((*TheGame->Yellow), square->XPos, square->YPos, 32 , 32);
				
				if (GameState != WAITING) {
					TheGame->DrawString(square->sfcStrValue, BLACK, square->XPos + 5, square->YPos + 2);
				}
			} else {
				if (square->Showing) {
					TheGame->DrawString(square->sfcStrValue, BLACK, square->XPos + 5, square->YPos + 2);
				} else {
					TheGame->DrawString(sfcQuestionMark, BLACK, square->XPos + 5, square->YPos + 2);
				}
			}
			
			
		}
	}
	
	if (GameState == CORRECT) {
		RenderCorrect();
	} else if (GameState == WRONG) {
		RenderWrong();
	}
}

void BPMiniGame_ShortCircuitSudoku::Tick() {
	if (LastStateChange != -1 && LastStateChange + 500 < TheGame->TickCount) {
		LastStateChange = -1;
		
		if (NumTries >= 8 && !MarathonMode) {
			Success();
		} else {
			LevelUp();
		}
	}
}

void BPMiniGame_ShortCircuitSudoku::OnMouseDown() {
	
}

void BPMiniGame_ShortCircuitSudoku::OnMouseMove() {
	
}

void BPMiniGame_ShortCircuitSudoku::OnMouseUp() {
	switch (GameState) {
		case CORRECT:
		case WRONG:
			break;
			
		case WAITING:
			if (TouchEvent.Y < 95) {
				if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 32, 1, 52, 44)) {
					SubmitAnswer(1);
				} else if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 100, 1, 52, 44)) {
					SubmitAnswer(2);
				} else if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 167, 1, 52, 44)) {
					SubmitAnswer(3);
				} else if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 237, 1, 52, 44)) {
					SubmitAnswer(4);
				} else if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 0, 49, 52, 44)) {
					SubmitAnswer(5);
				} else if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 66, 49, 52, 44)) {
					SubmitAnswer(6);
				} else if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 134, 49, 52, 44)) {
					SubmitAnswer(7);
				} else if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 202, 49, 52, 44)) {
					SubmitAnswer(8);
				} else if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 268, 49, 52, 44)) {
					SubmitAnswer(9);
				}
			} else {
				// asking for a hint!
				for (int i = 0; i < Sudoku.Count; ++i) {
					SudokuSquare* square = Sudoku[i];
					if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, square->XPos, square->YPos, 32, 32)) {
						// clicked this square!
						if (!square->Showing && square != GuessSquare) {
							square->Showing = true;
							Score -= 25;
						}
					}
				}
			}
			
			
			break;
	}
}

void BPMiniGame_ShortCircuitSudoku::LevelUp() {
	++NumTries;
	++CurrentLevel;
	LastStateChange = -1;
	if (CurrentLevel > 15) CurrentLevel = 15;
	
	GenerateGrid();
	
	BPList<int> hidden;
	
	// remove some squares
	for (int i = 0; i < 15 + CurrentLevel; ++i) {
		int hideme = TheGame->RandomRange(0, Sudoku.Count - 1);
		hidden.Add(hideme);
		Sudoku[hideme]->Showing = false;
	}
	
	hidden.Shuffle();
	
	GuessSquare = Sudoku[hidden[0]];
	
	GameState = WAITING;
}

void BPMiniGame_ShortCircuitSudoku::GenerateGrid() {
	Sudoku.Clear();
	SudokuSquare* Squares[81];
	
	for (int i = 0; i < 81; ++i) {
		Squares[i] = new SudokuSquare();
	}
	
	BPList<int>* Available[81];
	int squarecounter = 0;
	
	for (int x = 0; x <= 81 - 1; x++) {
		Available[x] = new BPList<int>();
		for (int i = 1; i <= 9; i++) {
			Available[x]->Add(i);
		}
	}
	

	while (!(squarecounter == 81)) {
	Beginning:
		int i = TheGame->RandomRange(0, Available[squarecounter]->Count - 1);
		if (!(Available[squarecounter]->Count == 0)) { // we may need to start again with this square?
			int z = (*Available[squarecounter])[i];
			SudokuSquare* item = Item(squarecounter, z);
			if (Conflicts(Squares, item) == false) {
				// this number is good!
				SAFE_DELETE(Squares[squarecounter]);
				Squares[squarecounter] = item;
				Available[squarecounter]->RemoveAt(i);
				squarecounter += 1;
			} else {
				// number is no good - try again
				SAFE_DELETE(item);
				Available[squarecounter]->RemoveAt(i);
				goto Beginning; // goto! eek! Jump back to the start
			}
		} else {
			// reset the current square
			for (int y = 1; y <= 9; y++) {
				Available[squarecounter]->Add(y);
			}
			
			// jump back to the start
			SAFE_DELETE(Squares[squarecounter - 1]);
			Squares[squarecounter - 1] = new SudokuSquare();
			squarecounter -= 1;
			goto Beginning;
		}
	}
	
	// all done - allocate all the squares in the grid
	for (int j = 0; j < 81; j++) {
		ostringstream str;
		str << Squares[j]->Value;
		TheGame->AllocString(&Squares[j]->sfcStrValue, str.str().c_str(), NORMAL, 24, 24, CENTRED);
		Sudoku.Add(Squares[j]);
	}

	for (int i = 0; i < 81; ++i) {
		SAFE_DELETE(Available[i]);
	}
}

bool BPMiniGame_ShortCircuitSudoku::Conflicts(SudokuSquare** CurrentValues, SudokuSquare* test) {	
	for (int i = 0; i < 81; ++i) {
		SudokuSquare* s = CurrentValues[i];
		if (s->Across != 0 & s->Across == test->Across) {
			if (s->Value == test->Value) {
				return true;
			}
		}
	}

	for (int i = 0; i < 81; ++i) {
		SudokuSquare* s = CurrentValues[i];

		if (s->Down != 0 & s->Down == test->Down) {
			if (s->Value == test->Value) {
				return true;
			}
		}
	}

	for (int i = 0; i < 81; ++i) {
		SudokuSquare* s = CurrentValues[i];

		if (s->Region != 0 & s->Region == test->Region) {
			if (s->Value == test->Value) {
				return true;
			}
		}
	}
	return false;
}


SudokuSquare* BPMiniGame_ShortCircuitSudoku::Item(int n, int v) {
	SudokuSquare* retval = new SudokuSquare();
	++n;
	
	retval->Across = GetAcrossFromNumber(n);
	retval->Down = GetDownFromNumber(n);
	retval->Region = GetRegionFromNumber(n);
	
	retval->Value = v;
	retval->Index = n - 1;
	
	retval->XPos = 6 + ((retval->Across - 1) * 34);
	retval->YPos = 102 + ((retval->Down - 1) * 34);
	
	if (retval->Across > 6) {
		retval->XPos += 4;
	} else if (retval->Across > 3) {
		retval->XPos += 2;
	}
	
	if (retval->Down > 6) {
		retval->YPos += 4;
	} else if (retval->Down > 3) {
		retval->YPos += 2;
	}
	
	return retval;
}

int BPMiniGame_ShortCircuitSudoku::GetAcrossFromNumber(int n) {
	int k;
	k = n % 9;
	
	if (k == 0) {
		return 9;
	} else {
		return k;
	}
}

int BPMiniGame_ShortCircuitSudoku::GetDownFromNumber(int n) {
	if (GetAcrossFromNumber(n) == 9) {
		return n / 9;
	} else {
		return n / 9 + 1;
	}
}

int BPMiniGame_ShortCircuitSudoku::GetRegionFromNumber(int n) {
	int a = GetAcrossFromNumber(n);
	int d = GetDownFromNumber(n);
	
	if (1 <= a & a < 4 & 1 <= d & d < 4) {
		return 1;
	} else if (4 <= a & a < 7 & 1 <= d & d < 4) {
		return 2;
	} else if (7 <= a & a < 10 & 1 <= d & d < 4) {
		return 3;
	} else if (1 <= a & a < 4 & 4 <= d & d < 7) {
		return 4;
	} else if (4 <= a & a < 7 & 4 <= d & d < 7) {
		return 5;
	} else if (7 <= a & a < 10 & 4 <= d & d < 7) {
		return 6;
	} else if (1 <= a & a < 4 & 7 <= d & d < 10) {
		return 7;
	} else if (4 <= a & a < 7 & 7 <= d & d < 10) {
		return 8;
	} else if (7 <= a & a < 10 & 7 <= d & d < 10) {
		return 9;
	}
}

void BPMiniGame_ShortCircuitSudoku::SubmitAnswer(int Answer) {
	if (Answer == GuessSquare->Value) {
		Score += 60;
		TheGame->PlaySound("correct");
		GameState = CORRECT;
	} else {
		Score -= 100;
		TheGame->PlaySound("wrong");
		GameState = WRONG;
		--CurrentLevel; // LevelUp() auto-raises CurrentLevel, so we pre-cut it here
		if (CurrentLevel < 0) CurrentLevel = 0;
	}
	
	LastStateChange = TheGame->TickCount;
}

void BPMiniGame_ShortCircuitSudoku::SetMarathon() {
	MarathonMode = true;
}
