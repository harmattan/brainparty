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

#include "bombhunt.h"
#include "Minigame.h"
#include "BPList.h"

BPMiniGame_BombHunt::BPMiniGame_BombHunt(BPGame* game) : BPMiniGame(game) {	
	SuccessTime = -1;
	GuessesLeft = 12;
	LastNumCorrect = 0;
	LastGuessTime = 0;
	TimeStarted = 0;
	
	sfcLastNumCorrect = sfcGuessesLeft = NULL;
	
	Selected1 = Selected2 = Selected3 = NULL;
	
	GameTitle = "Bomb Hunt";
	GameHelp = "There's a bomb, and you have to diffuse it! You have 12 tries to figure out which three gems make up the bomb, and I'll tell you how many of your gem guesses were correct. All three gems in the bomb are different!";
	GameHelp2 = "To make a guess, just tap any three bomb components. If you get it wrong, you'll be told how many of those gems were correct. For example, if you tap red, green and blue gems and Brain Party says that you got 1 correct, it means that two of those gems were wrong.";
		
	MiniGameType = PUZZLE;
	
	sfcBackground = TheGame->LoadBitmap("bombhunt", 320, 480);
	
	Components.Add(TheGame->LoadBitmap("gem1", 64, 64));
	Components.Add(TheGame->LoadBitmap("gem2", 64, 64));
	Components.Add(TheGame->LoadBitmap("gem3", 64, 64));
	Components.Add(TheGame->LoadBitmap("gem4", 64, 64));
	Components.Add(TheGame->LoadBitmap("gem5", 64, 64));
	Components.Add(TheGame->LoadBitmap("gem6", 64, 64));
	Components.Add(TheGame->LoadBitmap("gem7", 64, 64));
	Components.Add(TheGame->LoadBitmap("gem8", 64, 64));
	
	Components.Shuffle();
	
	BombItems.Add(Components[0]);
	BombItems.Add(Components[1]);
	BombItems.Add(Components[2]);
		
	Components.Shuffle();
}

BPMiniGame_BombHunt::~BPMiniGame_BombHunt() {	
	SAFE_DELETE(sfcBackground);
	
	SAFE_DELETE(sfcLastNumCorrect);
	SAFE_DELETE(sfcGuessesLeft);
	
	BombItems.Clear(false);
	Components.Clear();
}

void BPMiniGame_BombHunt::Start() {
	TimeStarted = TheGame->TickCount;
}

int BPMiniGame_BombHunt::GetWeight() {
	int time = floor((TheGame->TickCount - TimeStarted) / 1000);
	return MinMax(600 - round(time * 20));
}

void BPMiniGame_BombHunt::Render() {	
	TheGame->DrawImage(sfcBackground, 0, 0);
	
	for (int i = 0; i < 8; ++i) {
		if (i < 4) {
			TheGame->DrawImage(Components[i], 23 + (i * 70), 77);
		} else {
			TheGame->DrawImage(Components[i], 23 + ((i - 4) * 70), 147);
		}
	}
	
	if (Selected1 != NULL) TheGame->DrawImage(Selected1, 48, 257);
	if (Selected2 != NULL) TheGame->DrawImage(Selected2, 128, 257);
	if (Selected3 != NULL) TheGame->DrawImage(Selected3, 208, 257);
	
	if (SuccessTime == -1) {
		if (LastGuessTime != 0) {
			TheGame->DrawString(sfcLastNumCorrect, WHITE, 0, 350);
		} else {
			TheGame->DrawString(sfcGuessesLeft, WHITE, 0, 350);
		}
	}
}

void BPMiniGame_BombHunt::Tick() {
	if (SuccessTime != -1) {
		if (SuccessTime + 250 < TheGame->TickCount) {
			Success();
		}
	} else {
		// this player has used all their chances!
		if (GuessesLeft == 0) {
			// short delay so the Guesses Left text can be updated
			if (LastGuessTime + 250 < TheGame->TickCount) {
				Failure();
			}
		}
	}
	
	if (LastGuessTime != 0) {
		if (LastGuessTime + 500 < TheGame->TickCount) {
			ClearForNextTry();
		}
	}
}

void BPMiniGame_BombHunt::ClearForNextTry() {
	Selected1 = NULL;
	Selected2 = NULL;
	Selected3 = NULL;
	
	LastGuessTime = 0;
}

void BPMiniGame_BombHunt::OnMouseUp() {
	if (LastGuessTime != 0) ClearForNextTry(); // speed up the guessing time when you tap the screen
	if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 23, 77, 64, 64)) SelectItem(Components[0]);
	if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 93, 77, 64, 64)) SelectItem(Components[1]);
	if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 163, 77, 64, 64)) SelectItem(Components[2]);
	if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 233, 77, 64, 64)) SelectItem(Components[3]);
	if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 23, 147, 64, 64)) SelectItem(Components[4]);
	if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 93, 147, 64, 64)) SelectItem(Components[5]);
	if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 163, 147, 64, 64)) SelectItem(Components[6]);
	if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 233, 147, 64, 64)) SelectItem(Components[7]);
	
	if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 0, 231, 240, 20)) {
		if (GuessesLeft > 1) {
			//MessageBox.Show("You only have " + GuessesLeft + " more tries to figure out which three items make up the bomb - good luck!", GameTitle);
		} else {
			//MessageBox.Show("You only have one more try to figure out which three items make up the bomb - you'd better guess correctly!", GameTitle);
		}
	}
	
	if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 28, 243, 263, 90)) {
		MessageBox::Show("This is your guessing area. As you tap on the gems above, they'll be placed here so you can see which ones you have chosen.", GameTitle);
	}
}

void BPMiniGame_BombHunt::OnMouseDown() {

}

void BPMiniGame_BombHunt::OnMouseMove() {
	
}

void BPMiniGame_BombHunt::SelectItem(Texture* item) {
	if (Selected1 == NULL) {
		TheGame->PlaySound("gem_select");
		Selected1 = item;
		return;
	} else if (Selected2 == NULL && Selected1 != item) {
		TheGame->PlaySound("gem_select");
		Selected2 = item;
		return;
	} else if (Selected3 == NULL && Selected1 != item && Selected2 != item) {
		Selected3 = item;
		CheckAnswer();
		return;
	}
	
	if (Selected1 == item || Selected2 == item || Selected3 == item) {
		MessageBox::Show("You've chosen that item already! Remember, the bomb is made up of three different components.", "Oops!");
	}
}

void BPMiniGame_BombHunt::CheckAnswer() {
	--GuessesLeft;
	
	LastNumCorrect = 0;
	
	if (BombItems.Contains(Selected1)) ++LastNumCorrect;
	if (BombItems.Contains(Selected2)) ++LastNumCorrect;
	if (BombItems.Contains(Selected3)) ++LastNumCorrect;
	
	if (LastNumCorrect == 3) {
		TheGame->PlaySound("gem_select");
		SuccessTime = TheGame->TickCount;
	} else {
		TheGame->PlaySound("wrong2");
		LastGuessTime = TheGame->TickCount;
	}
	
	ostringstream numcorrect;
	numcorrect << "You got " << LastNumCorrect << " correct.";
	TheGame->AllocString(&sfcLastNumCorrect, numcorrect.str().c_str(), NORMAL, 320, 50, CENTRED);
	
	ostringstream guesses;
	
	if (GuessesLeft <= 1) {
		TheGame->AllocString(&sfcGuessesLeft, "1 guess remaining.", NORMAL, 320, 50, CENTRED);
	} else {
		guesses << GuessesLeft << " guesses remaining.";
		TheGame->AllocString(&sfcGuessesLeft, guesses.str().c_str(), NORMAL, 320, 50, CENTRED);	
	}

}
