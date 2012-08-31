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

#include "cardmatch.h"
#include "Minigame.h"


BPMiniGame_CardMatch::BPMiniGame_CardMatch(BPGame* game) : BPMiniGame(game) {		
	sfcBackground = TheGame->LoadBitmap("cardmatch", 320, 480);
	sfcCardBack = TheGame->LoadBitmap("card_unknown", 64, 64);
	
	CardTypes = new BPPList<Texture*>();
	Cards = new BPPList<BPMiniGame_CardMatch_Card*>();
	
	Locked = false; // true to stop the player making any more moves
	RemovalSpeed = 500;
	
	NumErrors = TimeStarted = 0;
	
	GameTitle = "Card Match";
	GameHelp = "This is a game kids have been playing for years, so you'll find it a cinch - just match pairs of cards on the table as fast as you can.";
	GameHelp2 = "When you see the grid of cards, try tapping the first two, but make sure you remember what is shown on the cards. Each card has at least one other like it, and when you tap them both they disappear. Your job is to keep trying all the cards until you have matched them all.";
	
	MiniGameType = PUZZLE;
	
	CardTypes->Add(TheGame->LoadBitmap("card_1", 64, 64));
	CardTypes->Add(TheGame->LoadBitmap("card_2", 64, 64));
	CardTypes->Add(TheGame->LoadBitmap("card_3", 64, 64));
	CardTypes->Add(TheGame->LoadBitmap("card_4", 64, 64));
	CardTypes->Add(TheGame->LoadBitmap("card_5", 64, 64));
	CardTypes->Add(TheGame->LoadBitmap("card_6", 64, 64));
	CardTypes->Add(TheGame->LoadBitmap("card_7", 64, 64));
	CardTypes->Add(TheGame->LoadBitmap("card_8", 64, 64));
	
	BPList<int>* nums = new BPList<int>();
	int newnum;
	
	for (int i = 0; i < 15; ++i) { // we need to insert fifteen pairs of numbers
		newnum = TheGame->RandomRange(0, 7);
		nums->Add(newnum);
		nums->Add(newnum);
	}
	
	nums->Shuffle();
	
	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 6; ++j) {
			BPMiniGame_CardMatch_Card* card = new BPMiniGame_CardMatch_Card();
			
			newnum = TheGame->RandomRange(0, nums->Count - 1);
			card->Type = (*nums)[newnum];
			nums->RemoveAt(newnum);
			
			card->IsShowing = false;
			card->X = 2 + (i * 63);
			card->Y = 18 + (j * 63);
			
			Cards->Add(card);
		}
	}
}

BPMiniGame_CardMatch::~BPMiniGame_CardMatch() {
	SAFE_DELETE(sfcBackground);
	SAFE_DELETE(sfcCardBack);
	
	CardTypes->Clear();
	SAFE_DELETE(CardTypes);
	
	Cards->Clear();
	SAFE_DELETE(Cards);
}

void BPMiniGame_CardMatch::Start() {
	TimeStarted = TheGame->TickCount;
}

int BPMiniGame_CardMatch::GetWeight() {
	int TimePassed = TheGame->TickCount - TimeStarted;
	return MinMax(650 - (NumErrors * 15) - (TimePassed / 1000.0f));
}

void BPMiniGame_CardMatch::Render() {
	TheGame->DrawImage(sfcBackground, 0, 0);
	
	for (int i = 0; i < Cards->Count; ++i) {
		BPMiniGame_CardMatch_Card* card = (*Cards)[i];
		
		if (card->IsShowing) {		
			if (card->MatchTime == 0) {
				// hasn't been matched yet
				TheGame->DrawImage((*CardTypes)[card->Type], card->X + 32, card->Y + 32, 0.0f, 1.0f, (*TheGame->White));
			} else {
				// has been matched - make it fade away
				int diffticks = TheGame->TickCount - card->MatchTime;
				
				if (diffticks <= RemovalSpeed) {
					float fticks = ((float)diffticks / RemovalSpeed); // gives a number between 0 and 1, for colours
					Colour drawcol = Colour(1.0f, 1.0f, 1.0f, 1 - fticks);

					TheGame->DrawImage((*CardTypes)[card->Type], card->X + 32, card->Y + 32, (1 - fticks) * 360, 1 - fticks, drawcol);
				}
			}
		} else {
				TheGame->DrawImage(sfcCardBack, card->X + 32, card->Y + 32, 0.0f, 1.0f, (*TheGame->White));
		}
	}
}

void BPMiniGame_CardMatch::Tick() {
	CleanUpCards(false);
	
	if (Cards->Count == 0) {
		Success();
	}
}

void BPMiniGame_CardMatch::CleanUpCards(bool force) {
	for (int i = Cards->Count - 1; i >= 0; --i) {
		BPMiniGame_CardMatch_Card* card = (*Cards)[i];
		
		if (card->MatchTime != 0) {
			if (force || card->MatchTime + 500 < TheGame->TickCount) {
				Cards->RemoveAt(i);
				Locked = false;
			}
		}
		
		if (card->ShowStartTime != 0) {
			if (force || card->ShowStartTime + 500 < TheGame->TickCount) {
				card->ShowStartTime = 0;
				card->IsShowing = false;
				Locked = false;
			}
		}
	}
}

void BPMiniGame_CardMatch::OnMouseDown() {
	
}

void BPMiniGame_CardMatch::OnMouseMove() {
	
}

void BPMiniGame_CardMatch::OnMouseUp() {
	if (Locked) {
		CleanUpCards(true);
		return;
	}
	
	for (int i = 0; i < Cards->Count; ++i) {
		BPMiniGame_CardMatch_Card* card = (*Cards)[i];
		
		if (card->X <= TouchEvent.X && card->X + sfcCardBack->Width >= TouchEvent.X) {
			if (card->Y <= TouchEvent.Y && card->Y + sfcCardBack->Height >= TouchEvent.Y) {
				
				// this card has already been matched; skip it
				if (card->MatchTime != 0) continue;
				
				if (card->IsShowing) {
					card->IsShowing = false;
				} else {
					TheGame->PlaySound("card_flip");
					card->IsShowing = true;
				}
			}
		}
	}
	
	CheckSelectedCards();
}


void BPMiniGame_CardMatch::CheckSelectedCards() {
	// the selected cards
	BPMiniGame_CardMatch_Card* typefirst = NULL;
	BPMiniGame_CardMatch_Card* typesecond = NULL;
	
	for (int i = 0; i < Cards->Count; ++i) {
		BPMiniGame_CardMatch_Card* card = (*Cards)[i];
		
		if (card->IsShowing) {
			if (typefirst == NULL) {
				typefirst = card;
			} else {
				typesecond = card;
			}
		}
	}
	
	if (typefirst != NULL && typesecond != NULL) {
		if (typefirst->Type == typesecond->Type) {
			typefirst->MatchTime = TheGame->TickCount;
			typesecond->MatchTime = TheGame->TickCount;
		} else {
			typefirst->ShowStartTime = TheGame->TickCount;
			typesecond->ShowStartTime = TheGame->TickCount;
			
			++NumErrors;
		}
		
		Locked = true;
	}
}
