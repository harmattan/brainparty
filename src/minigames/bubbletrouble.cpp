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

#include "bubbletrouble.h"
#include "Minigame.h"

				
BPMiniGame_BubbleTrouble::BPMiniGame_BubbleTrouble(BPGame* game) : BPMiniGame(game) {
	sfcBackground = TheGame->LoadBitmap("bubbletrouble", 320, 480);
	sfcRedCross = TheGame->LoadBitmap("red_cross", 64, 64);

	DirectionPos = 0;
	MaxNumber = 0;
	TimeStarted = 0;
	LastCreatedTime = 0;
	SuccessTime = -1;
	
	NumWrong = 0;
	
	GameTitle = "Bubble Trouble";
	GameHelp = "You'll be shown bubbles with numbers on - you need to tap the lowest-numbered bubbles first. Get it wrong or take too long, and I'll create another bubble, so be careful!";
	GameHelp2 = "The key here is to take your time: the lowest bubble may be obscured by another bubble for a second or two, so take your time and make sure you have the right bubble before tapping it!";
	
	MiniGameType = LIVELY;
	
	BubbleTypes.Add(TheGame->LoadBitmap("marble_blue", 64, 64));
	BubbleTypes.Add(TheGame->LoadBitmap("marble_green", 64, 64));
	BubbleTypes.Add(TheGame->LoadBitmap("marble_purple", 64, 64));
	BubbleTypes.Add(TheGame->LoadBitmap("marble_red", 64, 64));
	BubbleTypes.Add(TheGame->LoadBitmap("marble_yellow", 64, 64));
	
	for (int i = 0; i < 360; ++i) {
		if (i < 10) continue;
		if (i >= 350) continue;
		if (i > 80 and i < 100) continue;
		if (i > 170 and i < 190) continue;
		if (i > 260 and i < 280) continue;
		Directions.Add(i);
	}
	
	Directions.Shuffle();
	
	MaxNumber = 0;
}

BPMiniGame_BubbleTrouble::~BPMiniGame_BubbleTrouble() {
	SAFE_DELETE(sfcBackground);
	SAFE_DELETE(sfcRedCross);
	
	BubbleTypes.Clear();
	
	Bubbles.Clear();
	
	Directions.Clear();
}

void BPMiniGame_BubbleTrouble::CreateBubble(bool initial) {
	int TimePassed = (TheGame->TickCount - TimeStarted) / 1000.0f;
	
	// don't create any more bubbles after 90 seconds, otherwise it's just too hard
	if (TimePassed > 90) return;
	
	if (Bubbles.Count > 12) {
		// we already have lots of bubbles - don't create any more!
		return;
	}
	
	MaxNumber += TheGame->RandomRange(1, 4);
	BPMiniGame_BubbleTrouble_Bubble* bubble = new BPMiniGame_BubbleTrouble_Bubble();

	bubble->X = TheGame->RandomRange(0, MiniGameWidth - BubbleTypes[0]->Width);
	bubble->Y = TheGame->RandomRange(0, MiniGameHeight - BubbleTypes[0]->Width);
	
	if (initial) {
		bubble->CreationTime = -1000;
	} else {
		bubble->CreationTime = TheGame->TickCount;
	}
	
	bubble->Speed = TheGame->RandomRange(25, 50);
	bubble->Type = TheGame->RandomRange(0, BubbleTypes.Count - 1);
	bubble->Number = MaxNumber;
	ChooseBubbleDirection(bubble);
	
	ostringstream number;
	number << bubble->Number;
	TheGame->AllocString(&bubble->sfcNumberStr, number.str().c_str(), LARGE, 64, 64, CENTRED, true);
	
	Bubbles.Add(bubble);
	
	LastCreatedTime = TheGame->TickCount;
}

void BPMiniGame_BubbleTrouble::ChooseBubbleDirection(BPMiniGame_BubbleTrouble_Bubble* bubble) {
	++DirectionPos;
	if (DirectionPos == Directions.Count) DirectionPos = 0;
	bubble->Direction = Directions[DirectionPos];
	
	SetBubbleDirection(bubble);
}

void BPMiniGame_BubbleTrouble::SetBubbleDirection(BPMiniGame_BubbleTrouble_Bubble* bubble) {
	bubble->XMove = cos(bubble->Direction * M_PI / 180) * bubble->Speed;
	bubble->YMove = sin(bubble->Direction * M_PI / 180) * bubble->Speed;	
}

void BPMiniGame_BubbleTrouble::Start() {
	LastCreatedTime = TheGame->TickCount;
	TimeStarted = TheGame->TickCount;

	for (int i = 0; i < 15; ++i) {
		CreateBubble(true);
	}
}

int BPMiniGame_BubbleTrouble::GetWeight() {
	int TimePassed = (TheGame->TickCount - TimeStarted) / 1000.0f;
	return MinMax(550 - (NumWrong * 50) - floor(TimePassed * 4));
}

void BPMiniGame_BubbleTrouble::Render() {
	TheGame->DrawImage(sfcBackground, 0, 0);
	
	BPMiniGame_BubbleTrouble_Bubble* bubble;
	
	for (int i = 0; i < Bubbles.Count; ++i) {
		bubble = Bubbles[i];
		
		if (bubble->CreationTime + 350.0f > TheGame->TickCount) {
			// fade this bubble in
			float diff = (bubble->CreationTime + 350.0f) - TheGame->TickCount;
			diff /= 250.0f;
			diff = 1 - diff;
			
			glColor4f(1.0f, 1.0f, 1.0f, diff * 0.8f);
			TheGame->DrawImage(BubbleTypes[bubble->Type], bubble->X, bubble->Y);

			glColor4f(0.0f, 0.0f, 0.0f, diff);
			TheGame->DrawString(bubble->sfcNumberStr, NOCOLOUR, bubble->X, bubble->Y + 15);
			
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		} else {
			glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
			TheGame->DrawImage(BubbleTypes[bubble->Type], bubble->X, bubble->Y);
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		
			TheGame->DrawString(bubble->sfcNumberStr, BLACK, bubble->X, bubble->Y + 15);
		}
		
		if (bubble->LastWrong + 250 > TheGame->TickCount) {
			TheGame->DrawImage(sfcRedCross, bubble->X, bubble->Y);
		}
	}
}

void BPMiniGame_BubbleTrouble::Tick() {
	if (SuccessTime == -1) {
		if (LastCreatedTime + 3500 < TheGame->TickCount) {
			CreateBubble(false);
		}
	} else {
		if (SuccessTime + 500 < TheGame->TickCount) {
			Success();
		}
	}
	
	BPMiniGame_BubbleTrouble_Bubble* bubble;
		
	for (int i = 0; i < Bubbles.Count; ++i) {
		bubble = Bubbles[i];
		
		bubble->X += bubble->XMove * TheGame->ElapsedSeconds;
		bubble->Y += bubble->YMove * TheGame->ElapsedSeconds;
		
		bool fixdirection = false;
		
		if (bubble->X > MiniGameWidth - BubbleTypes[0]->Width) {
			bubble->X = MiniGameWidth - BubbleTypes[0]->Width;
			bubble->Direction = 180 - bubble->Direction;
			fixdirection = true;
		} else if (bubble->X < 0) {
			bubble->X = 0;
			bubble->Direction = 180 - bubble->Direction;
			fixdirection = true;
		}
		
		if (bubble->Y > 416 - BubbleTypes[0]->Height) {
			bubble->Y = 416 - BubbleTypes[0]->Height;
			bubble->Direction = 360 - bubble->Direction;
			fixdirection = true;
		} else if (bubble->Y < 0) {
			bubble->Y = 0;
			bubble->Direction = 360 - bubble->Direction;
			fixdirection = true;
		}
		
		if (fixdirection) {
			if (bubble->Direction >= 360) bubble->Direction = bubble->Direction - 360;
			if (bubble->Direction < 0) bubble->Direction = 360 - abs(bubble->Direction);
			SetBubbleDirection(bubble);
		}
	}
}

void BPMiniGame_BubbleTrouble::OnMouseUp() {
	for (int i = 0; i < Bubbles.Count; ++i) {
		BPMiniGame_BubbleTrouble_Bubble* bubble = Bubbles[i];
		
		if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, round(bubble->X), Round(bubble->Y), BubbleTypes[0]->Width, BubbleTypes[0]->Height)) {
			ClickBubble(bubble);
			return;
		}
	}
}

void BPMiniGame_BubbleTrouble::OnMouseDown() {
	
}

void BPMiniGame_BubbleTrouble::OnMouseMove() {
	
}

void BPMiniGame_BubbleTrouble::ClickBubble(BPMiniGame_BubbleTrouble_Bubble* clicked) {
	BPMiniGame_BubbleTrouble_Bubble* bubble;
	
	for (int i = 0; i < Bubbles.Count; ++i) {
		bubble = Bubbles[i];
		
		if (bubble->Number < clicked->Number) {
			TheGame->PlaySound("wrong3");
			clicked->LastWrong = TheGame->TickCount;
			++NumWrong;
			
			if (Bubbles.Count > 12) {
				// we already have lots of bubbles - don't create any more!
				return;
			}
			
			CreateBubble(false);
			ChooseBubbleDirection(bubble);
			
			return;
		}
	}
	
	TheGame->PlaySound("bubble");
	
	// if we're still here, this bubble was the correct one!
	Bubbles.Remove(clicked);
	
	if (Bubbles.Count == 0) SuccessTime = TheGame->TickCount;
}
