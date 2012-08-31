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

#include "diceoff.h"
#include "Minigame.h"

BPMiniGame_DiceOff::BPMiniGame_DiceOff(BPGame* game) : BPMiniGame(game) {
	TheGame = game;
	
	State = WAITING;
	OurTurn = true;
	FirstAITurn = true;
	
	PlaySound = false;
	
	NeutralColor = Colour(0.7f, 0.7f, 0.7f, 1.0f);
	PlayerColor = Colour(0.0f, 0.0f, 1.0f, 1.0f);
	AIColor = Colour(1.0f, 0.0f, 0.0f, 1.0f);
	
	DicePics.Add(NULL);
	DicePics.Add(TheGame->LoadBitmap("die_1", 50, 50));
	DicePics.Add(TheGame->LoadBitmap("die_2", 50, 50));
	DicePics.Add(TheGame->LoadBitmap("die_3", 50, 50));
	DicePics.Add(TheGame->LoadBitmap("die_4", 50, 50));
	
	for (int i = 0; i < NumRows; ++i) {
		for (int j = 0; j < NumCols; ++j) {
			BPMiniGame_DiceOff_Die* die = new BPMiniGame_DiceOff_Die();
			die->Pos = BPPoint(10 + (j * DiceSize), 8 + (i * DiceSize));
			die->Value = 1;
			die->Index = Dice.Count;
			die->X = j;
			die->Y = i;
			die->Col = NeutralColor;
			die->NumNeighbours = CountNeighbours(j, i);
			Dice.Add(die);
		}
	}
	
	UpdateScore();
	
	GameTitle = "Dice Off";
	GameHelp = "Press a dice to add one to its score. When it goes higher than the number of neighbours it has, it will overflow, taking over each of its neighbours and adding one to them. Can you take over all your opponent's dice?";
	GameHelp2 = "Dice in the corners have just two neighbours, so as soon as they reach three they will overflow and add one to each neighbour, taking control of them at the same time.";
	
	MiniGameType = ACTION;
}

BPMiniGame_DiceOff::~BPMiniGame_DiceOff() {
	DicePics.Clear();
	Dice.Clear();
}

void BPMiniGame_DiceOff::Start() {
	
}

int BPMiniGame_DiceOff::GetWeight() {
	return 500;
}

void BPMiniGame_DiceOff::Tick() {
	switch (State) {
		case SUCCESS:
			if (LastStateChange + 1000 < TheGame->TickCount) {
				Success();
			}
			
			break;
			
		case FAILURE:
			if (LastStateChange + 1000 < TheGame->TickCount) {
				Failure();
			}
			
			break;
			
		case WAITING:
			// do nothing!
			break;
			
		case THINKING:
			// give a short delay, then make the AI act
			if (LastStateChange + AIThinkSpeed < TheGame->TickCount) {
				AITurn();
			}
			
			break;
			
		case CHANGING:
			// wait before changing more pieces
			if (LastStateChange + ChainSpeed < TheGame->TickCount) {
				PlaySound = false;
				
				if (CheckChangeList.Count == 0 || ChangeStartTime + 4000 < TheGame->TickCount) { // force kill changes that take too long
					// no more moves to make
					if (OurTurn) {
						OurTurn = false;
						SetGameState(THINKING);
					} else {
						OurTurn = true;
						SetGameState(WAITING);
					}
					
					return;
				}
				
				BPList<BPMiniGame_DiceOff_Die*> ToChange;
				
				for (int i = 0; i < CheckChangeList.Count; ++i) {
					ToChange.Add(CheckChangeList[i]);
				}
				
				CheckChangeList.Clear();
				
				for (int i = 0; i < ToChange.Count; ++i) {
					BPMiniGame_DiceOff_Die* die = ToChange[i];
					
					if (OurTurn) {
						BumpDie(die, OWNER_PLAYER);
					} else {
						BumpDie(die, OWNER_AI);
					}
				}
				
				LastStateChange = TheGame->TickCount;
				
				if (PlaySound) {
					// something interesting changed; we should play a sound!
					TheGame->PlaySound("whack");
				}
				
				UpdateScore();
			}
	}
}

int BPMiniGame_DiceOff::CountNeighbours(int x, int y) {
	int result = 0;
	if (x > 0) ++result; // one to the left
	if (x < NumCols - 1) ++result; // one to the right
	if (y > 0) ++result; // one above
	if (y < NumRows - 1) ++result; // one below
	
	return result;
}

void BPMiniGame_DiceOff::Render() {
	TheGame->Clear(TheGame->Black);
	
	for (int i = 0; i < Dice.Count; ++i) {
		BPMiniGame_DiceOff_Die* die = Dice[i];
		
		if (die->LastStateChange + ChangeSpeed > TheGame->TickCount) {
			// this needs to be animated!
			float diff = (TheGame->TickCount - die->LastStateChange) / (float)ChangeSpeed;
			if (diff > 1.0f) diff = 1.0f;
			
			Colour drawcol = TheGame->ColourSmoothStep((*TheGame->White), die->Col, diff);
			
			DicePics[die->Value]->Draw(die->Pos.X + 25, die->Pos.Y + 25, 0.0f, 1.0f, drawcol);
		} else {
			// draw plain-old die
			DicePics[die->Value]->Draw(die->Pos.X + 25, die->Pos.Y + 25, 0.0f, 1.0f, die->Col);
		}
	}
}

void BPMiniGame_DiceOff::OnMouseDown() {
	if (!OurTurn) return;
	if (State != WAITING) return;
	
	for (int i = 0; i < Dice.Count; ++i) {
		BPMiniGame_DiceOff_Die* die = Dice[i];
		
		if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, die->Pos.X, die->Pos.Y, DiceSize, DiceSize)) {
			if (die->Owner != OWNER_AI) {				
				CheckChangeList.Add(die);
				SetGameState(CHANGING);
				ChangeStartTime = TheGame->TickCount;
				break;
			}
		}
	}
}

void BPMiniGame_DiceOff::OnMouseUp() {
	
}

void BPMiniGame_DiceOff::OnMouseMove() {
	
}

void BPMiniGame_DiceOff::BumpDie(BPMiniGame_DiceOff_Die* die, Ownership newowner) {
	++die->Value;
	die->LastStateChange = TheGame->TickCount;
	die->Owner = newowner;
	
	if (newowner == OWNER_PLAYER) {
		die->Col = PlayerColor;
	} else {
		die->Col = AIColor;
	}
	
	PlaySound = true;
	
	if (die->Value > die->NumNeighbours) {
		// we need to split this!
		die->Value = 1;
		
		if (die->X > 0) CheckChangeList.Add(Dice[die->Index - 1]);
		if (die->X < NumCols - 1) CheckChangeList.Add(Dice[die->Index + 1]);
		if (die->Y > 0) CheckChangeList.Add(Dice[die->Index - NumCols]);
		if (die->Y < NumRows - 1) CheckChangeList.Add(Dice[die->Index + NumCols]);
	}
}

void BPMiniGame_DiceOff::AITurn() {
	// this isn't a particularly smart AI, but it's good enough to provide a challenge to all but the best players
	
	OurTurn = false;
	
	BPList<BPMiniGame_DiceOff_Die*> bestdice;
	int bestscore = 0;
	
	for (int i = 0; i < Dice.Count; ++i) {
		AIClosedList.Clear();
		
		BPMiniGame_DiceOff_Die* die = Dice[i];
		if (die->Owner == OWNER_PLAYER) continue; // we can't move their pieces
		AICheck(die);
		
		if (AIClosedList.Count > bestscore) {
			// this is a better score than what we had before - clear all the options and use this one
			bestscore = AIClosedList.Count;
			bestdice.Clear();
			bestdice.Add(die);
		} else if (AIClosedList.Count == bestscore) {
			// this move is as good as our previous move
			bestdice.Add(die);
		}
	}
	
	if (bestdice.Count > 0) {
		BPMiniGame_DiceOff_Die* die = bestdice[TheGame->RandomRange(0, bestdice.Count - 1)];
		
		if (FirstAITurn) {
			// IF the player started in a corner
			// AND if the AI chooses to start next to them
			// THEN the player can win in one turn just by bumping their corner piece one more
			// SO: stop the AI using an edge tile on their first turn
			
			while (die->NumNeighbours < 4) {
				die = bestdice[TheGame->RandomRange(0, bestdice.Count - 1)];				
			}
		}
		
		CheckChangeList.Add(die);
		SetGameState(CHANGING);
		FirstAITurn = false;
		ChangeStartTime = TheGame->TickCount;
	}
}

void BPMiniGame_DiceOff::AICheck(BPMiniGame_DiceOff_Die* die) {
	if (AIClosedList.Contains(die)) return;
	
	AIClosedList.Add(die);
	
	if (die->Value + 1 > die->NumNeighbours) {
		// this die would break if we bumped it!
		if (die->X > 0) AICheck(Dice[die->Index - 1]);
		if (die->X < NumCols - 1) AICheck(Dice[die->Index + 1]);
		if (die->Y > 0) AICheck(Dice[die->Index - NumCols]);
		if (die->Y < NumRows - 1) AICheck(Dice[die->Index + NumCols]);
	}
}

void BPMiniGame_DiceOff::SetGameState(MiniGameStates state) {
	// if we've already won/lost, bail out
	if (State == FAILURE || State == SUCCESS) return;
	
	State = state;
	LastStateChange = TheGame->TickCount;
}

void BPMiniGame_DiceOff::UpdateScore() {
	int playerscore = 0;
	int aiscore = 0;
	
	for (int i = 0; i < Dice.Count; ++i) {
		BPMiniGame_DiceOff_Die* die = Dice[i];
		
		if (die->Owner == OWNER_PLAYER) {
			++playerscore;
		} else if (die->Owner == OWNER_AI) {
			++aiscore;
		}
	}
	
//	TheGame->AllocString(&sfcScore, [NSString stringWithFormat:"%d:%d", playerscore, aiscore], XLARGE, 320, 64, CENTRED, false);	
	
	// if we're both low on points, this is the start of the game
	if (playerscore <= 1 && aiscore <= 1) return;
	
	if (playerscore == 0 && State != FAILURE) {
		SetGameState(FAILURE);
		return;
	} else if (aiscore == 0 && State != SUCCESS) {
		SetGameState(SUCCESS);
		return;
	}
}
