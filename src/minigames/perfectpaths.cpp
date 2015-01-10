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

#include "perfectpaths.h"
#include "Minigame.h"
		
BPMiniGame_PerfectPaths::BPMiniGame_PerfectPaths(BPGame* game) : BPMiniGame(game) {
	sfcBackground = TheGame->LoadBitmap("perfectpaths", 320, 416);
	
	sfcPerfect = TheGame->LoadBitmap("perfect", 320, 92);
	sfcOK = TheGame->LoadBitmap("ok", 320, 92);
	
	CurrentScore = BestScore = TotalDiff = TimeStarted = CurrentLevel = 0;
	sfcScore = NULL;
	SetScore();
	
	GameTitle = "Perfect Paths";
	GameHelp = "Tap squares to make a path from the top white square to the bottom one, using the route with the lowest numbers along the way, then press Go when you're ready to advance. You can't move diagonally!";
	GameHelp2 = "Each square has a number on signifying how \"hard\" that square is to move over, and that number is used to calculate the total move difficulty. At the bottom of the screen you'll see the best possible score that you can get, along with your current score.";
	
	MiniGameType = LIVELY;
	
	GameState = WAITING;
	
	LoNumbers.Add(NULL);
	LoNumbers.Add(TheGame->LoadBitmap("pp_1_lo", 44, 44));
	LoNumbers.Add(NULL);
	LoNumbers.Add(TheGame->LoadBitmap("pp_3_lo", 44, 44));
	LoNumbers.Add(NULL);
	LoNumbers.Add(TheGame->LoadBitmap("pp_5_lo", 44, 44));
	LoNumbers.Add(NULL);
	LoNumbers.Add(TheGame->LoadBitmap("pp_7_lo", 44, 44));
	LoNumbers.Add(NULL);
	LoNumbers.Add(TheGame->LoadBitmap("pp_9_lo", 44, 44));
	
	HiNumbers.Add(NULL);
	HiNumbers.Add(TheGame->LoadBitmap("pp_1_hi", 44, 44));
	HiNumbers.Add(NULL);
	HiNumbers.Add(TheGame->LoadBitmap("pp_3_hi", 44, 44));
	HiNumbers.Add(NULL);
	HiNumbers.Add(TheGame->LoadBitmap("pp_5_hi", 44, 44));
	HiNumbers.Add(NULL);
	HiNumbers.Add(TheGame->LoadBitmap("pp_7_hi", 44, 44));
	HiNumbers.Add(NULL);
	HiNumbers.Add(TheGame->LoadBitmap("pp_9_hi", 44, 44));
	
	LevelUp();
}

BPMiniGame_PerfectPaths::~BPMiniGame_PerfectPaths() {
	SAFE_DELETE(sfcBackground);
	StartPositions.Clear();
	EndPositions.Clear();
	LoNumbers.Clear();
	HiNumbers.Clear();
//	SAFE_DELETE(StartPos);
//	SAFE_DELETE(EndPos);
	
	SAFE_DELETE(sfcPerfect);
	SAFE_DELETE(sfcOK);
	
	Squares.Clear();
	
	SAFE_DELETE(sfcScore);
}

void BPMiniGame_PerfectPaths::Start() {
	TimeStarted = TheGame->TickCount;
}

int BPMiniGame_PerfectPaths::GetWeight() {
	float TimePassed = (TheGame->TickCount - TimeStarted) / 1000.0f;
	return MinMax(520 - (TotalDiff * 5) - round(TimePassed));
}

void BPMiniGame_PerfectPaths::Render() {
	TheGame->DrawImage(sfcBackground, 0, 0);
	
	for (int i = 0; i < Squares.Count; ++i) {
		BPMiniGame_PerfectPaths_Square* square = Squares[i];
		
		if (square == EndPos || square == StartPos) {
			TheGame->FillRectangle((*TheGame->White), square->XPos, square->YPos, 44, 44);
		} else {
			if (Moves.Contains(square)) {
				if (square != StartPos) TheGame->DrawImage(HiNumbers[square->Difficulty], square->XPos, square->YPos);
			} else {
				TheGame->DrawImage(LoNumbers[square->Difficulty], square->XPos, square->YPos);
			}
		}
	}
	
	TheGame->DrawString(sfcScore, WHITE, 6, 377);
	
	if (GameState == CORRECT) {
		switch (LastDiff) {
			case 0:
				RenderPerfect();
				break;
				
			case 1:
			case 2:
				RenderCorrect();
				break;
								
			case 3:
			case 4:
			case 5:
				RenderOK();
				break;
				
			default:
				RenderWrong();
				break;
		}
	}
}

void BPMiniGame_PerfectPaths::Tick() {
	if (GameState == CORRECT && LastStateChange + 700 < TheGame->TickCount) {
		LevelUp();
	}
	
	if (CurrentLevel >= 6) {
		Success();
	}
}

void BPMiniGame_PerfectPaths::OnMouseUp() {
	if (GameState != WAITING) return;
	
	if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 187, 368, 137, 47)) {
		CheckResult();
	} else if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 0, 0, 320, 364)) {
		// making a move
		for (int i = 0; i < Squares.Count; ++i) {
			BPMiniGame_PerfectPaths_Square* square = Squares[i];
			
			if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, square->XPos, square->YPos, 44, 44)) {
				if (square == StartPos || square == EndPos) continue;
				
				if (Moves.Contains(square)) {
					if (Moves[Moves.Count - 1] == square) {
						// they can only remove a square if it was the last one to be added
						Moves.Remove(square);
						CalculateOurScore();
					} else {
						MessageBox::Show("You can only remove the last position in your path.", "Oops!");
					}
				} else {
					BPMiniGame_PerfectPaths_Square* last = Moves[Moves.Count - 1];
					
					if (CanMove(square, last)) {
						Moves.Add(square);
						CalculateOurScore();
					} else {
						if (Moves.Count == 1) {
							MessageBox::Show("You can only move to a square that's horizontally or vertically next to your previous move, starting from the top white square.", "Oops!");
						} else {
							MessageBox::Show("You can only move to a square that's horizontally or vertically next to your previous move.", "Oops!");
						}
					}
				}
				
				break;
			}
		}
	} else if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 0, 273, 72, 18)) {
		MessageBox::Show("This shows the current number of moves it will take to get from the top to the bottom. The closer this is to the Best score, the more points you get.", GameTitle);
	} else if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 80, 273, 72, 18)) {
		MessageBox::Show("This shows the best possible number of moves between the top white square and the bottom white square. If you want to get the highest score, you must match this every time.", GameTitle);
	}
}

void BPMiniGame_PerfectPaths::OnMouseMove() {
	
}

void BPMiniGame_PerfectPaths::OnMouseDown() {

}

void BPMiniGame_PerfectPaths::CalculateOurScore() {
	CurrentScore = 0;
	
	for (int i = 0; i < Moves.Count; ++i) {
		BPMiniGame_PerfectPaths_Square* square = Moves[i];
		if (square == StartPos) continue;
		CurrentScore += square->Difficulty;
	}
	
	SetScore();
}

void BPMiniGame_PerfectPaths::CheckResult() {
	BPMiniGame_PerfectPaths_Square* last = Moves[Moves.Count - 1];
	
	if (CanMove(EndPos, last)) {
		LastDiff = abs(BestScore - CurrentScore);
		
		TotalDiff += LastDiff;
		
		GameState = CORRECT;
		LastStateChange = TheGame->TickCount;
		
		switch (LastDiff) {
			case 0:
			case 1:
			case 2:
			case 3:
				TheGame->PlaySound("correct");
				break;
				
			default:
				TheGame->PlaySound("down");
				break;
		}
	} else {
		MessageBox::Show("That doesn't work - your path needs to connect the white square at the top with the white square at the bottom!", "Try again");
	}
}

void BPMiniGame_PerfectPaths::LevelUp() {
	++CurrentLevel;
	CurrentScore = 0;
	
	if (CurrentLevel >= 6) {
		return;
	}
	
	Squares.Clear();
	Moves.Clear();
	MoveSquares.Clear();
	
	int max_difficulty = 7;
	
	switch (CurrentLevel) {
		case 1:
			max_difficulty = 7;
			break;
			
		case 2:
			max_difficulty = 8;
			break;
			
		case 3:
			max_difficulty = 9;
			break;
			
		case 4:
			max_difficulty = 11;
			break;
			
		case 5:
			max_difficulty = 12;
			break;
	}
	
	for (int i = 0; i < 7; ++i) {
		for (int j = 0; j < 8; ++j) {
			BPMiniGame_PerfectPaths_Square* square = new BPMiniGame_PerfectPaths_Square();
			square->Difficulty = TheGame->RandomRange(0, max_difficulty);
			
			switch (square->Difficulty) {
				case 0:
				case 1:
				case 2:
					square->Difficulty = 1;
					square->DifficultyStr = "1";
					square->Col = TheGame->Green;
					break;
					
				case 3:
				case 4:
				case 5:
				case 6:
					square->Difficulty = 3;
					square->DifficultyStr = "3";
					square->Col = TheGame->Orange;
					break;
					
				case 7:
				case 8:
				case 9:
					square->Difficulty = 5;
					square->DifficultyStr = "5";
					square->Col = TheGame->Red;
					break;
					
				case 10:
				case 11:
					square->Difficulty = 7;
					square->DifficultyStr = "7";
					square->Col = TheGame->DarkRed;
					break;
					
				case 12:
					square->Difficulty = 9;
					square->DifficultyStr = "9";
					square->Col = TheGame->DarkGrey;
					break;
			}
						
			square->X = i;
			square->Y = j;
			square->XPos = 6 + (i * 44);
			square->YPos = 6 + (j * 44);
			square->Pos = Squares.Count;
			
			Squares.Add(square);
		}
	}
	
	StartPositions.Clear();
	EndPositions.Clear();
	
	if (TheGame->RandomRange(0, 1) == 0) {
		StartPositions.Add(8);
		StartPositions.Add(16);
		StartPositions.Add(24);
		EndPositions.Add(39);
		EndPositions.Add(47);
		EndPositions.Add(55);
	} else {
		StartPositions.Add(32);
		StartPositions.Add(40);
		StartPositions.Add(48);
		EndPositions.Add(7);
		EndPositions.Add(15);
		EndPositions.Add(23);
	}
	
	StartPositions.Shuffle();
	EndPositions.Shuffle();
	
	StartPos = Squares[StartPositions[0]];
	EndPos = Squares[EndPositions[0]];
	
	Moves.Add(StartPos);
	
	CalculateBestMove();
	
	SetScore();
	
	GameState = WAITING;
	LastStateChange = TheGame->TickCount;
}

void BPMiniGame_PerfectPaths::CalculateBestMove() {
	MoveSquares.Add(StartPos);
	StartPos->MoveCalc = 0;
	
	while (MoveSquares.Count != 0) {
		BPMiniGame_PerfectPaths_Square* square = MoveSquares[0];
		MoveSquares.RemoveAt(0);
		FloodFill(square);
	}
	
	int probable_best = 999;
	
	// now we need to figure out what the best score was: what were the lowest numbers around the end position?
	if (EndPos->X > 0) probable_best = Squares[EndPos->Pos - 8]->MoveCalc;
	if (EndPos->X < 6 && Squares[EndPos->Pos + 8]->MoveCalc < probable_best) probable_best = Squares[EndPos->Pos + 8]->MoveCalc;
	if (EndPos->Y > 0 && Squares[EndPos->Pos - 1]->MoveCalc < probable_best) probable_best = Squares[EndPos->Pos - 1]->MoveCalc;
	if (EndPos->Y < 6 && Squares[EndPos->Pos + 1]->MoveCalc < probable_best) probable_best = Squares[EndPos->Pos + 1]->MoveCalc;
	
	BestScore = probable_best;
}

void BPMiniGame_PerfectPaths::FloodFill(BPMiniGame_PerfectPaths_Square* square) {
	int Plus1 = square->Pos + 1;
	int Minus1 = square->Pos - 1;
	int Plus10 = square->Pos + 8;
	int Minus10 = square->Pos - 8;
	
	if (square->X > 0) {
		// check square to the left
		if (square->MoveCalc + Squares[Minus10]->Difficulty < Squares[Minus10]->MoveCalc) {
			Squares[Minus10]->MoveCalc = square->MoveCalc + Squares[Minus10]->Difficulty;
			MoveSquares.Add(Squares[Minus10]);
		}
	}
	
	if (square->X < 6) {
		// check square to the right
		if (square->MoveCalc + Squares[Plus10]->Difficulty < Squares[Plus10]->MoveCalc) {
			Squares[Plus10]->MoveCalc = square->MoveCalc + Squares[Plus10]->Difficulty;
			MoveSquares.Add(Squares[Plus10]);
		}
	}
	
	if (square->Y > 0) {
		// check square above
		if (square->MoveCalc + Squares[Minus1]->Difficulty < Squares[Minus1]->MoveCalc) {
			Squares[Minus1]->MoveCalc = square->MoveCalc + Squares[Minus1]->Difficulty;
			MoveSquares.Add(Squares[Minus1]);
		}
	}
	
	if (square->Y < 7) {
		// check square below
		if (square->MoveCalc + Squares[Plus1]->Difficulty < Squares[Plus1]->MoveCalc) {
			Squares[Plus1]->MoveCalc = square->MoveCalc + Squares[Plus1]->Difficulty;
			MoveSquares.Add(Squares[Plus1]);
		}
	}
}

bool BPMiniGame_PerfectPaths::CanMove(BPMiniGame_PerfectPaths_Square* square1, BPMiniGame_PerfectPaths_Square* square2) {
	// return true if this square is adjacent to the last move square
	
	return (abs(square1->X - square2->X) + abs(square1->Y - square2->Y)) == 1;
}

void BPMiniGame_PerfectPaths::SetScore() {
	ostringstream score;
	
	score << "Move: " << CurrentScore << "      Best: " << BestScore;
	
	TheGame->AllocString(&sfcScore, score.str().c_str(), NORMAL, 239, 47, LEFT);
}

void BPMiniGame_PerfectPaths::RenderPerfect() {
	TheGame->DrawImage(sfcPerfect, 0, 172);
}

void BPMiniGame_PerfectPaths::RenderOK() {
	TheGame->DrawImage(sfcOK, 0, 172);
}
