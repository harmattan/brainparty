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

#include "bpsays.h"
#include "Minigame.h"
			
BPMiniGame_BPSays::BPMiniGame_BPSays(BPGame* game) : BPMiniGame(game) {
	sfcBackground = TheGame->LoadBitmap("bpsays", 320, 480);
	sfcBackgroundChoosing = TheGame->LoadBitmap("bpsays_choosing", 320, 480);
	sfcCircle = TheGame->LoadBitmap("white_circle", 64, 64);
	
	Sequence = new BPList<int>();
	Numbers = new BPPList<Texture*>();
	
	Speed = NumCorrect = NumErrors = SequenceCounter = SelectedNumber = NumTries = TimeStarted = 0;
	Countdown = -1;
	Repeating = false;
	
	GameTitle = "Brain Party Says";
	GameHelp = "Watch the colours I'll show you, then copy the sequence yourself. I'll make things harder by showing you words that are intentionally wrong - ignore those words and focus on the colours you see!";
	GameHelp2 = "The key in this game is to focus on the colours, not the words. That is, Brain Party might show a colour like Red, but then show the text \"Blue\" to try to put you off. You need to ignore the text and look at the colour: the correct answer is Red, not Blue.";
	
	MiniGameType = PUZZLE;
	
	Numbers->Add(TheGame->LoadBitmap("1", 215, 215));
	Numbers->Add(TheGame->LoadBitmap("2", 215, 215));
	Numbers->Add(TheGame->LoadBitmap("3", 215, 215));
	
	ColourList.Add(Colour(0.0f, 0.0f, 1.0f, 1.0f));
	ColourList.Add(Colour(0.0f, 1.0f, 0.0f, 1.0f));
	ColourList.Add(Colour(1.0f, 0.75f, 0.0f, 1.0f));
	ColourList.Add(Colour(1.0f, 0.0f, 0.0f, 1.0f));
		
	SpriteFont* tex = NULL; TheGame->AllocString(&tex, "Blue", ALMOSTEPIC, 320, 297, CENTRED, true);
	ColourWords.Add(tex);
	
	SpriteFont* tex2 = NULL; TheGame->AllocString(&tex2, "Green", ALMOSTEPIC, 320, 297, CENTRED, true);
	ColourWords.Add(tex2);
	
	SpriteFont* tex3 = NULL; TheGame->AllocString(&tex3, "Yellow", ALMOSTEPIC, 320, 297, CENTRED, true);
	ColourWords.Add(tex3);
	
	SpriteFont* tex4 = NULL; TheGame->AllocString(&tex4, "Red", ALMOSTEPIC, 320, 297, CENTRED, true);
	ColourWords.Add(tex4);
	
	GameState = 2;
	Speed = 700;
}

BPMiniGame_BPSays::~BPMiniGame_BPSays() {
	SAFE_DELETE(sfcBackground);
	SAFE_DELETE(sfcBackgroundChoosing);
	SAFE_DELETE(sfcCircle);
	
	ColourWords.Clear();
	
	Sequence->Clear();
	SAFE_DELETE(Sequence);
	
	Numbers->Clear();
	SAFE_DELETE(Numbers);
}

void BPMiniGame_BPSays::Start() {
	Countdown = TheGame->TickCount;	
	TimeStarted = TheGame->TickCount;
	TheGame->PlaySound("beep_hi");
}

int BPMiniGame_BPSays::GetWeight() {
	float TimePassed = (TheGame->TickCount - TimeStarted) / 1000.0f;
	return MinMax(570 - (NumErrors * 50) - (TimePassed / 0.9));
}

void BPMiniGame_BPSays::Render() {
	if (Countdown == -1) return; // don't do anything until the game begins!
	
	switch (GameState) {
		case 0:
		case 1:
		case 2:
			// counting down
			TheGame->DrawImage(sfcBackground, 0, 0);
			
		{
			float diff = (Countdown + Speed) - TheGame->TickCount;
			diff /= Speed;
			
			diff = TheGame->Clamp(diff, 0.0f, 1.0f);
			
			Colour fade = Colour(1.0f, 1.0f, 1.0f, diff);
			
			TheGame->DrawImage((*Numbers)[GameState], MiniGameHalfWidth - 107, MiniGameHalfHeight - 107, fade);
		}
			
			if (Countdown + Speed < TheGame->TickCount) {
				--GameState;
				Countdown = TheGame->TickCount;
				
				if (GameState == -1) {
					GameState = 4;
					
					// if we're repeating, don't add to the sequence
					if (Repeating) {
						Repeating = false;
					} else {
						GenerateSequence();
					}
				} else {
					TheGame->PlaySound("beep_hi");					
				}
			}
			break;
			
		case 4:
			// showing a sequence
			TheGame->FillRectangle(ColourList[(*Sequence)[SequenceCounter]], 0, 0, 320, 480);
			
			if (SequenceCounter % 2 == 0) {
				TheGame->DrawString(ColourWords[(SequenceCounter + Sequence->Count) % ColourWords.Count], WHITE, 0, 0);
				TheGame->DrawString(ColourWords[(SequenceCounter + Sequence->Count) % ColourWords.Count], WHITE, 0, 200);
			} else {
				TheGame->DrawString(ColourWords[(SequenceCounter + Sequence->Count) % ColourWords.Count], WHITE, 0, 100);
				TheGame->DrawString(ColourWords[(SequenceCounter + Sequence->Count) % ColourWords.Count], WHITE, 0, 300);
			}
			
			if (Countdown + Speed < TheGame->TickCount) {
				++SequenceCounter;
				Countdown = TheGame->TickCount;
			}
			
			if (SequenceCounter == Sequence->Count) {
				GameState = 5;
				SequenceCounter = 0;
				SelectedNumber = -1;
			}
			
			break;
			
		case 5:
			// players copying the sequence
			
			TheGame->DrawImage(sfcBackgroundChoosing, 0, 0);
			{
				Colour col = Colour(1.0f, 1.0f, 1.0f, 0.6f);
			
				switch (SelectedNumber) {
					case 0:
						TheGame->DrawImage(sfcCircle, 128, 292, col);
						break;
					case 1:
						TheGame->DrawImage(sfcCircle, 35, 200, col);
						break;
					case 2:
						TheGame->DrawImage(sfcCircle, 221, 200, col);
						break;
					case 3:
						TheGame->DrawImage(sfcCircle, 128, 106, col);
						break;
				}
			}
			
			break;
			
		case 6:
			// player made a mistake; show the oops message then repeat the sequence
			
			Repeating = true;
			
			TheGame->DrawImage(sfcBackground, 0, 0);
			RenderWrong();
			
			if (Countdown + Speed < TheGame->TickCount) {
				SequenceCounter = 0;
				GameState = 2;
				Countdown = TheGame->TickCount;
				TheGame->PlaySound("beep_hi");
			}
			
			break;
			
		case 7:
			// player was correct; show the correct message then continue
			TheGame->DrawImage(sfcBackgroundChoosing, 0, 0);
			
			{
				Colour col = Colour(1.0f, 1.0f, 1.0f, 0.6f);
				
				switch (SelectedNumber) {
					case 0:
						TheGame->DrawImage(sfcCircle, 128, 292, col);
						break;
					case 1:
						TheGame->DrawImage(sfcCircle, 35, 200, col);
						break;
					case 2:
						TheGame->DrawImage(sfcCircle, 221, 200, col);
						break;
					case 3:
						TheGame->DrawImage(sfcCircle, 128, 106, col);
						break;
				}
			}
			
			RenderCorrect();
			
			if (Countdown + Speed < TheGame->TickCount) {
				GameState = 2;
				TheGame->PlaySound("beep_hi");
				SelectedNumber = -1;
				Countdown = TheGame->TickCount;
			}
			
			break;
	}
}

void BPMiniGame_BPSays::GenerateSequence() {
	SequenceCounter = 0;
	
	if (NumCorrect == 0) {
		Sequence->Add(TheGame->RandomRange(0, ColourList.Count - 1));
	} else {
		// don't repeat the same colour twice in a row
		// otherwise it's hard to spot!
		Sequence->Add(TheGame->RandomRangeExcept(0, ColourList.Count - 1, (*Sequence)[Sequence->Count - 1]));
	}
}

void BPMiniGame_BPSays::Tick() {
	if (NumTries >= 8 && !MarathonMode) {
		Success();
	}
}

void BPMiniGame_BPSays::OnMouseDown() {
	
}

void BPMiniGame_BPSays::OnMouseMove() {
	
}

void BPMiniGame_BPSays::OnMouseUp() {
	if (GameState != 5) {
		// player is copying the sequence
		return;
	}
	
	BPPoint blue = BPPoint(159, 374);
	BPPoint green = BPPoint(16, 232);
	BPPoint yellow = BPPoint(302, 232);
	BPPoint red = BPPoint(159, 88);
	
	SelectedNumber = 0;
	float distance = BPPoint::Distance(TouchEvent, blue);
	
	if (BPPoint::Distance(TouchEvent, green) < distance) {
		distance = BPPoint::Distance(TouchEvent, green);
		SelectedNumber = 1;
	}
	
	if (BPPoint::Distance(TouchEvent, yellow) < distance) {
		distance = BPPoint::Distance(TouchEvent, yellow);
		SelectedNumber = 2;
	}
	
	if (BPPoint::Distance(TouchEvent, red) < distance) {
		distance = BPPoint::Distance(TouchEvent, red);
		SelectedNumber = 3;
	}
	
	if ((*Sequence)[SequenceCounter] == SelectedNumber) {
		++SequenceCounter;
		TheGame->PlaySound("gem_select");
	} else {
		Wrong();
	}
	
	if (SequenceCounter == Sequence->Count) {
		LevelUp();
	}
}

void BPMiniGame_BPSays::LevelUp() {
	++NumTries;
	GameState = 7;
	++NumCorrect;
	Speed -= 25;
	Countdown = TheGame->TickCount;
}

void BPMiniGame_BPSays::Wrong() {
	++NumTries;
	TheGame->PlaySound("wrong");
	SelectedNumber = -1;
	GameState = 6;
	Countdown = TheGame->TickCount;
	++NumErrors;
}

void BPMiniGame_BPSays::SetMarathon() {
	MarathonMode = true;
}
