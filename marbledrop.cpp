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

#include "marbledrop.h"
#include "Minigame.h"
		
BPMiniGame_MarbleDrop::BPMiniGame_MarbleDrop(BPGame* game) : BPMiniGame(game) {
	sfcBackground = TheGame->LoadBitmap("marbledrop", 320, 416);
	
	FinishTime = 0;
	IsSuccess = false;
	
	TimeStarted = 0;
	
	GameTitle = "Marble Drop";
	GameHelp = "I dropped some marbles in the top box. Below that is an empty frame; by tapping the columns in that frame to place marbles, can you drop your marbles in the same order as me?";
	GameHelp2 = "The marbles that will be dropped are shown on the right of the screen. The key here is to look ahead: if there are two places a yellow marble could go, look at the marbles you'll have next to see whether one of them needs to go above a yellow marble in a certain place.";
	
	MiniGameType = PUZZLE;
	
	MarbleTypes.Add(TheGame->LoadBitmap("marble_red40", 40, 40));
	MarbleTypes.Add(TheGame->LoadBitmap("marble_green40", 40, 40));
	MarbleTypes.Add(TheGame->LoadBitmap("marble_blue40", 40, 40));
	MarbleTypes.Add(TheGame->LoadBitmap("marble_purple40", 40, 40));
	MarbleTypes.Add(TheGame->LoadBitmap("marble_yellow40", 40, 40));
	
	GenerateBoard();
}

BPMiniGame_MarbleDrop::~BPMiniGame_MarbleDrop() {
	SAFE_DELETE(sfcBackground);
	MarbleTypes.Clear();
	MarbleOrder.Clear();
	OriginalFirstCol.Clear();
	OriginalSecondCol.Clear();
	OriginalThirdCol.Clear();
	OriginalFourthCol.Clear();
	PlayerFirstCol.Clear();
	PlayerSecondCol.Clear();
	PlayerThirdCol.Clear();
	PlayerFourthCol.Clear();
}

void BPMiniGame_MarbleDrop::OnMouseMove() {
	
}

void BPMiniGame_MarbleDrop::OnMouseDown() {
	
}

void BPMiniGame_MarbleDrop::OnMouseUp() {
	if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 11, 212, 51, 197)) {
		if (PlayerFirstCol.Count == 4) return;
		MakeMove(PlayerFirstCol);
	} else if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 71, 212, 51, 197)) {
		if (PlayerSecondCol.Count == 4) return;
		MakeMove(PlayerSecondCol);
	} else if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 131, 212, 51, 197)) {
		if (PlayerThirdCol.Count == 4) return;
		MakeMove(PlayerThirdCol);
	} else if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 191, 212, 51, 197)) {
		if (PlayerFourthCol.Count == 4) return;
		MakeMove(PlayerFourthCol);
	} else if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 9, 7, 241, 197)) {
		MessageBox::Show("This is the original box, where I placed my marbles. Tap anywhere in the bottom box to place marbles in the same places I put mine here.", GameTitle);
	} else if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 259, 4, 58, 409)) {
		MessageBox::Show("These are the marbles you need to place in the bottom box - place them by tapping any of the columns in the bottom box. The marble at the top of this list will be used next.", GameTitle);
	}
	
	if (PlayerFirstCol.Count == 4 && PlayerSecondCol.Count == 4 && PlayerThirdCol.Count == 4 && PlayerFourthCol.Count == 4) {
		// player has placed all their pieces; check their answer
		IsSuccess = CheckAnswer();
		FinishTime = TheGame->TickCount;
	} else {
		// player hasn't placed all their pieces; check their answer anyway
		if (!CheckAnswer()) {
			MessageBox::Show("Oops - that's not right! You need to place the marbles in the exact same position as shown in the top. Try starting again...", "Try again...");
			GenerateBoard();
		}
	}
}


bool BPMiniGame_MarbleDrop::CheckAnswer() {
	for (int i = 0; i < OriginalFirstCol.Count; ++i) {
		if (PlayerFirstCol.Count > i && OriginalFirstCol[i] != PlayerFirstCol[i]) return false;
	}
	
	for (int i = 0; i < OriginalSecondCol.Count; ++i) {
		if (PlayerSecondCol.Count > i && OriginalSecondCol[i] != PlayerSecondCol[i]) return false;
	}
	
	for (int i = 0; i < OriginalThirdCol.Count; ++i) {
		if (PlayerThirdCol.Count > i && OriginalThirdCol[i] != PlayerThirdCol[i]) return false;
	}
	
	for (int i = 0; i < OriginalFourthCol.Count; ++i) {
		if (PlayerFourthCol.Count > i && OriginalFourthCol[i] != PlayerFourthCol[i]) return false;
	}
	
	// if we've made it this far, all the columns match!
	return true;
}

void BPMiniGame_MarbleDrop::MakeMove(BPList<int>& playerlist) {
	TheGame->PlaySound("down");
	
	int nextmarble = MarbleOrder[0];
	playerlist.Add(nextmarble);
	
	MarbleOrder.RemoveAt(0);
}

void BPMiniGame_MarbleDrop::Start() {
	TimeStarted = TheGame->TickCount;
}

int BPMiniGame_MarbleDrop::GetWeight() {
	if (IsSuccess) {
		float TimePassed = (TheGame->TickCount - TimeStarted) / 1000.0f;
		return MinMax(660 - round(TimePassed * 8));
	} else {
		return 0;
	}
}

void BPMiniGame_MarbleDrop::Render() {
	TheGame->DrawImage(sfcBackground, 0, 0);
	
	for (int i = 0; i < OriginalFirstCol.Count; ++i) TheGame->DrawImage(MarbleTypes[OriginalFirstCol[i]], 17, 154 - (i * 45));
	for (int i = 0; i < OriginalSecondCol.Count; ++i) TheGame->DrawImage(MarbleTypes[OriginalSecondCol[i]], 77, 154 - (i * 45));
	for (int i = 0; i < OriginalThirdCol.Count; ++i) TheGame->DrawImage(MarbleTypes[OriginalThirdCol[i]], 137, 154 - (i * 45));
	for (int i = 0; i < OriginalFourthCol.Count; ++i) TheGame->DrawImage(MarbleTypes[OriginalFourthCol[i]], 197, 154 - (i * 45));
	
	for (int i = 0; i < PlayerFirstCol.Count; ++i) TheGame->DrawImage(MarbleTypes[PlayerFirstCol[i]], 17, 359 - (i * 45));
	for (int i = 0; i < PlayerSecondCol.Count; ++i) TheGame->DrawImage(MarbleTypes[PlayerSecondCol[i]], 77, 359 - (i * 45));
	for (int i = 0; i < PlayerThirdCol.Count; ++i) TheGame->DrawImage(MarbleTypes[PlayerThirdCol[i]], 137, 359 - (i * 45));
	for (int i = 0; i < PlayerFourthCol.Count; ++i) TheGame->DrawImage(MarbleTypes[PlayerFourthCol[i]], 197, 359 - (i * 45));
	
	if (MarbleOrder.Count > 0) TheGame->DrawImage(MarbleTypes[MarbleOrder[0]], 265, 150);
	if (MarbleOrder.Count > 1) TheGame->DrawImage(MarbleTypes[MarbleOrder[1]], 265, 194);
	if (MarbleOrder.Count > 2) TheGame->DrawImage(MarbleTypes[MarbleOrder[2]], 265, 238);
	if (MarbleOrder.Count > 3) TheGame->DrawImage(MarbleTypes[MarbleOrder[3]], 265, 282);
	if (MarbleOrder.Count > 4) TheGame->DrawImage(MarbleTypes[MarbleOrder[4]], 265, 326);
	if (MarbleOrder.Count > 5) TheGame->DrawImage(MarbleTypes[MarbleOrder[5]], 265, 370);
}

void BPMiniGame_MarbleDrop::Tick() {
	if (FinishTime != 0) {
		if (FinishTime + 250 < TheGame->TickCount) {
			if (IsSuccess) {
				Success();
			} else {
				Failure();
			}
		}
	}
}

void BPMiniGame_MarbleDrop::GenerateBoard() {
	MarbleOrder.Clear();
	OriginalFirstCol.Clear();
	OriginalSecondCol.Clear();
	OriginalThirdCol.Clear();
	OriginalFourthCol.Clear();
	PlayerFirstCol.Clear();
	PlayerSecondCol.Clear();
	PlayerThirdCol.Clear();
	PlayerFourthCol.Clear();
	
	
	// this randomises the list of possible positions; avoiding the possibility of placing more than four pieces in a given column
	BPList<int> PositionOrder;
	
	for (int i = 1; i < 5; ++i) {
		PositionOrder.Add(i);
		PositionOrder.Add(i);
		PositionOrder.Add(i);
		PositionOrder.Add(i);
	}
	
	PositionOrder.Shuffle();
	
	for (int i = 0; i < 16; ++i) {
		// add a random marble type
		int marbletype = TheGame->RandomRange(0, MarbleTypes.Count - 1);
		
		MarbleOrder.Add(marbletype);
		
		switch (PositionOrder[i]) {
			case 1:
				OriginalFirstCol.Add(marbletype);
				break;
				
			case 2:
				OriginalSecondCol.Add(marbletype);
				break;
				
			case 3:
				OriginalThirdCol.Add(marbletype);
				break;
				
			case 4:
				OriginalFourthCol.Add(marbletype);
				break;
		}
	}
}
