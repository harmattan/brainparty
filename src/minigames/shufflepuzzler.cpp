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

#include "shufflepuzzler.h"
#include "Minigame.h"
		
BPMiniGame_ShufflePuzzler::BPMiniGame_ShufflePuzzler(BPGame* game) : BPMiniGame(game) {
	ShowResult = Loaded = false;
	
	TimeStarted = TheGame->TickCount;

	SuccessTime = -1;
	
	GameTitle = "Shuffle Puzzler";
	GameHelp = "Re-arrange the pieces to form the original picture by tapping on a piece to move. To see how the finished picture should look, tap the 'Shuffle Puzzler' title.";
	GameHelp2 = "We recommend solving the top three first, because that's quite easy to do. Once they are solved, don't move them again: treat it as a 3x2 problem.";
		
	MiniGameType = PUZZLE;
	
	sfcBackground = TheGame->LoadBitmap("shufflepuzzler", 320, 416);
	sfcPicture = TheGame->LoadBitmap("frog", 300, 300);
	
	ImageBits.Add(TheGame->LoadBitmap("shufflepuzzler1", 100, 100));
	ImageBits.Add(TheGame->LoadBitmap("shufflepuzzler2", 100, 100));
	ImageBits.Add(TheGame->LoadBitmap("shufflepuzzler3", 100, 100));
	ImageBits.Add(TheGame->LoadBitmap("shufflepuzzler4", 100, 100));
	ImageBits.Add(TheGame->LoadBitmap("shufflepuzzler5", 100, 100));
	ImageBits.Add(TheGame->LoadBitmap("shufflepuzzler6", 100, 100));
	ImageBits.Add(TheGame->LoadBitmap("shufflepuzzler7", 100, 100));
	ImageBits.Add(TheGame->LoadBitmap("shufflepuzzler8", 100, 100));
	ImageBits.Add(TheGame->LoadBitmap("shufflepuzzler9", 100, 100));

	sfcClock = NULL;
	
	int count = 0;
	
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			BPMiniGame_ShufflePuzzler_Item* item = new BPMiniGame_ShufflePuzzler_Item();
			item->ProperPosition = count;
			item->ActualPosition = count;
			item->Pic = ImageBits[count];
			Items.Add(item);
			++count;
		}
	}
}

BPMiniGame_ShufflePuzzler::~BPMiniGame_ShufflePuzzler() {
	SAFE_DELETE(sfcBackground);
	SAFE_DELETE(sfcPicture);
	
	ImageBits.Clear();
	Items.Clear();
	
	SAFE_DELETE(sfcClock);
}

void BPMiniGame_ShufflePuzzler::OnMouseUp() {
	
}

void BPMiniGame_ShufflePuzzler::OnMouseMove() {
	
}

void BPMiniGame_ShufflePuzzler::OnMouseDown() {
	if (TouchEvent.Y < 90) {
		ShowResult = !ShowResult;
		return;
	}
	
	// still here? User has clicked outside of the game logo - if they are showing the results, hide them now
	if (ShowResult) {
		ShowResult = false;
		return;
	}
	
	int x = (TouchEvent.X - 10) / 100;
	int y = (TouchEvent.Y - 106) / 100;
	
	int pos = (y * 3) + x;
	
	SwapPiece(pos);
	
	bool success = true;
	
	for (int i = 0; i < Items.Count; ++i) {
		BPMiniGame_ShufflePuzzler_Item* item = Items[i];

		if (item->ActualPosition != item->ProperPosition) {
			success = false;
			break;
		}
	}
	
	if (success && SuccessTime == -1) {
		SuccessTime = TheGame->TickCount;
	}
}

void BPMiniGame_ShufflePuzzler::SwapPiece(int pos) {
	// SwapPiece() is called many times at the beginning; only call it when it's a real player move
	
	switch (pos) {
		case 0: TrySwap(0, 1, 3, -1, -1); break;
		case 1: TrySwap(1, 0, 2, 4, -1); break;
		case 2: TrySwap(2, 1, 5, -1, -1); break;
		case 3: TrySwap(3, 0, 4, 6, -1); break;
		case 4: TrySwap(4, 1, 3, 5, 7); break;
		case 5: TrySwap(5, 2, 4, 8, -1); break;
		case 6: TrySwap(6, 3, 7, -1, -1); break;
		case 7: TrySwap(7, 6, 4, 8, -1); break;
		case 8: TrySwap(8, 5, 7, -1, -1); break;
	}
}

void BPMiniGame_ShufflePuzzler::TrySwap(int clicked, int pos1, int pos2, int pos3, int pos4) {
	BPMiniGame_ShufflePuzzler_Item* item_clicked = FindImageWithPos(clicked);
	BPMiniGame_ShufflePuzzler_Item* item1 = FindImageWithPos(pos1);
	BPMiniGame_ShufflePuzzler_Item* item2 = FindImageWithPos(pos2);
	BPMiniGame_ShufflePuzzler_Item* item3 = FindImageWithPos(pos3);
	BPMiniGame_ShufflePuzzler_Item* item4 = FindImageWithPos(pos4);
	
	if (item1 != NULL && item1->ProperPosition == 8) {
		if (Loaded) TheGame->PlaySound("slide");
		int tmp = item1->ActualPosition;
		item1->ActualPosition = item_clicked->ActualPosition;
		item_clicked->ActualPosition = tmp;
		return;
	}
	
	if (item2 != NULL && item2->ProperPosition == 8) {
		if (Loaded) TheGame->PlaySound("slide");
		int tmp = item2->ActualPosition;
		item2->ActualPosition = item_clicked->ActualPosition;
		item_clicked->ActualPosition = tmp;
		return;
	}
	
	if (item3 != NULL && item3->ProperPosition == 8) {
		if (Loaded) TheGame->PlaySound("slide");
		int tmp = item3->ActualPosition;
		item3->ActualPosition = item_clicked->ActualPosition;
		item_clicked->ActualPosition = tmp;
		return;
	}
	
	if (item4 != NULL && item4->ProperPosition == 8) {
		if (Loaded) TheGame->PlaySound("slide");
		int tmp = item4->ActualPosition;
		item4->ActualPosition = item_clicked->ActualPosition;
		item_clicked->ActualPosition = tmp;
		return;
	}
}


BPMiniGame_ShufflePuzzler_Item* BPMiniGame_ShufflePuzzler::FindImageWithPos(int pos) {
	for (int i = 0; i < Items.Count; ++i) {
		if (Items[i]->ActualPosition == pos) {
			return Items[i];
		}
	}
	
	return NULL;
}

void BPMiniGame_ShufflePuzzler::Start() {
	Shuffle();
	TimeStarted = TheGame->TickCount;
	Loaded = true;
}

int BPMiniGame_ShufflePuzzler::GetWeight() {
	float TimePassed = (TheGame->TickCount - TimeStarted) / 1000.0f;
	return MinMax(500 - round(TimePassed * 1.25));
}

void BPMiniGame_ShufflePuzzler::Render() {
	TheGame->DrawImage(sfcBackground, 0, 0);
	
	if (ShowResult) {
		TheGame->FillRectangle((*TheGame->Red), 8, 104, 304, 304);
		TheGame->DrawImage(sfcPicture, 10, 106);
	} else {		
		for (int i = 0; i <= 8; ++i) {
			for (int j = 0; j < Items.Count; ++j) {
				BPMiniGame_ShufflePuzzler_Item* item = Items[j];
					
				if (item->ActualPosition == i) {
					int x;
					int y;
					y = DivRem(i, 3, x);
					TheGame->DrawImage(item->Pic, 10 + (x * 100), 106 + (y * 100));
				}
			}
		}
		
		if (SuccessTime == -1) {
			TheGame->DrawLine(110, 106, 110, 406, TheGame->Black, 3);
			TheGame->DrawLine(210, 106, 210, 406, TheGame->Black, 3);
			TheGame->DrawLine(10, 206, 310, 206, TheGame->Black, 3);
			TheGame->DrawLine(10, 306, 310, 306, TheGame->Black, 3);
		}
	}
	
	if (SuccessTime == -1) {
		int TimePassed = TheGame->TickCount - TimeStarted;

		if (sfcClock == NULL || RedrawClock()) {
			TheGame->AllocString(&sfcClock, TheGame->TicksToTime(TimePassed)->c_str(), XLARGE, 152, 49, RIGHT);
		}
		
		TheGame->DrawString(sfcClock, WHITE, 149, 30);
	}
}

void BPMiniGame_ShufflePuzzler::Tick() {
	// this delay is longer than normal to give them a moment to see the finished picture clearly
	if (SuccessTime != -1 && SuccessTime + 750 < TheGame->TickCount) {
		Success();
	}
}

void BPMiniGame_ShufflePuzzler::Shuffle() {
	// shuffle a la brute force
	for (int i = 0; i < 1000; ++i) {
		SwapPiece(TheGame->RandomRange(0, 8));
	}
}
