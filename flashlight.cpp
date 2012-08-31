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

#include "flashlight.h"
#include "Minigame.h"
		
BPMiniGame_Flashlight::BPMiniGame_Flashlight(BPGame* game) : BPMiniGame(game) {
	sfcBackground = TheGame->LoadBitmap("flashlight", 320, 416);
	sfcLightOn = TheGame->LoadBitmap("light_on", 80, 80);
	sfcLightOff = TheGame->LoadBitmap("light_off", 80, 80);
	
	CurrentLevel = NumTries = NumWrong = ChangeSpeed = LastStateChange = 0;
	SuccessTime = CurrentlyFlashing = LastChange = -1;
	GameState = PAUSING;
	
	TheGame->PlaySound("beep_hi", true);
	
	GameTitle = "Flashlight";
	GameHelp = "Watch carefully as the lights flash. When they are finished, can you tell which light hasn't flashed at all? I'll make it harder by adding more lights and making them flash faster, so do your best!";
	GameHelp2 = "Try to relax. If you clear your mind and unfocus your eyes just a little, your brain can do a much better job of keeping track of which lights have flashed.";
	
	MiniGameType = PUZZLE;
	
	Positions.Add(new BPPoint(20, 28));
	Positions.Add(new BPPoint(120, 28));
	Positions.Add(new BPPoint(220, 28));
	Positions.Add(new BPPoint(70, 98));
	Positions.Add(new BPPoint(170, 98));
	Positions.Add(new BPPoint(20, 168));
	Positions.Add(new BPPoint(120, 168));
	Positions.Add(new BPPoint(220, 168));
	Positions.Add(new BPPoint(70, 238));
	Positions.Add(new BPPoint(170, 238));
	Positions.Add(new BPPoint(20, 308));
	Positions.Add(new BPPoint(120, 308));
	Positions.Add(new BPPoint(220, 308));
}

BPMiniGame_Flashlight::~BPMiniGame_Flashlight() {
	SAFE_DELETE(sfcBackground);
	SAFE_DELETE(sfcLightOn);
	SAFE_DELETE(sfcLightOff);
	
	Positions.Clear();
	Lights.Clear();
}

void BPMiniGame_Flashlight::ShowLayout() {
	Positions.Shuffle();
	
	int numlights = 4;
	
	switch (CurrentLevel) {
		case 0:
			ChangeSpeed = 400;
			numlights = 4;
			break;
			
		case 1:
			ChangeSpeed = 375;
			numlights = 5;
			break;
			
		case 2:
			ChangeSpeed = 350;
			numlights = 6;
			break;
			
		case 3:
			ChangeSpeed = 325;
			numlights = 7;
			break;
			
		case 4:
			ChangeSpeed = 300;
			numlights = 8;
			break;
			
		case 5:
			ChangeSpeed = 275;
			numlights = 9;
			break;
			
		case 6:
			ChangeSpeed = 250;
			numlights = 10;
			break;
			
		case 7:
			ChangeSpeed = 225;
			numlights = 11;
			break;
			
		case 8:
			ChangeSpeed = 200;
			numlights = 12;
			break;
	}
	
	Lights.Clear();
	
	for (int i = 0; i < numlights; ++i) {
		BPMiniGame_Flashlight_Light* light = new BPMiniGame_Flashlight_Light();
		light->X = Positions[i]->X;
		light->Y = Positions[i]->Y;
		
		Lights.Add(light);
	}
	
	// start currentlyflashing at 1 because the first light doesn't flash!
	CurrentlyFlashing = 1;
	LastChange = TheGame->TickCount + 500; // give a pause here so there's a period where all the lights are out
	GameState = PAUSING;
}

void BPMiniGame_Flashlight::Start() {
	LastChange = TheGame->TickCount + 500;
}

int BPMiniGame_Flashlight::GetWeight() {
	return MinMax(500 - (NumWrong * 75));
}

void BPMiniGame_Flashlight::Render() {
	TheGame->DrawImage(sfcBackground, 0, 0);
	
	for (int i = 0; i < Lights.Count; ++i) {
		BPMiniGame_Flashlight_Light* light = Lights[i];
		
		if (CurrentlyFlashing == i && GameState != PAUSING && LastChange < TheGame->TickCount) {
			float diff = TheGame->TickCount - LastChange;
			float cdiff = 1 - (diff / ChangeSpeed);
			
			Colour col = Colour(1.0f, 1.0f, 1.0f, cdiff);
			TheGame->DrawImage(sfcLightOff, light->X + sfcLightOn->HalfWidth, light->Y + sfcLightOn->HalfHeight, 0.0f, 1.0f + (cdiff / 4), (*TheGame->White));
			TheGame->DrawImage(sfcLightOn, light->X + sfcLightOn->HalfWidth, light->Y + sfcLightOn->HalfHeight, 0.0f, 1.0f + (cdiff / 4), col);
		} else {
			TheGame->DrawImage(sfcLightOff, light->X + sfcLightOn->HalfWidth, light->Y + sfcLightOn->HalfHeight, 0.0f, 1.0f, (*TheGame->White));
		}
	}
	
	switch (GameState) {
		case CORRECT:
			RenderCorrect();
			break;
			
		case WRONG:
			RenderWrong();
			break;
	}
}

void BPMiniGame_Flashlight::Tick() {
	if (SuccessTime != -1 && SuccessTime + 500 < TheGame->TickCount) {
		Success();
		return;
	}
	
	if (GameState == SHOWING || GameState == PAUSING) {
		if (LastChange + ChangeSpeed < TheGame->TickCount) {
			if (GameState == PAUSING) {
				GameState = SHOWING;
				LastChange = TheGame->TickCount;
				TheGame->PlaySound("beep_hi");
			} else {
				++CurrentlyFlashing;
				LastChange = TheGame->TickCount;
			
				if (CurrentlyFlashing == Lights.Count) {
					GameState = WAITING;
				} else {
					TheGame->PlaySound("beep_hi");
				}
			}
		}
	}
	
	if (LastStateChange != -1 && LastStateChange + 750 < TheGame->TickCount) {
		// time to ask another question
		LastStateChange = -1;
		ShowLayout();
	}
}

void BPMiniGame_Flashlight::OnMouseDown() {
	
}

void BPMiniGame_Flashlight::OnMouseMove() {
	
}

void BPMiniGame_Flashlight::OnMouseUp() {
	if (GameState == CORRECT || GameState == WRONG) return;
	
	for (int i = 0; i < Lights.Count; ++i) {
		BPMiniGame_Flashlight_Light* light = Lights[i];

		if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, light->X, light->Y, 80, 80)) {
			++NumTries;
			
			if (Lights.IndexOf(light) == 0) {
				TheGame->PlaySound("correct");
				GameState = CORRECT;
				++CurrentLevel;
			} else {
				TheGame->PlaySound("wrong3");
				GameState = WRONG;
				++NumWrong;
			}
			
			LastStateChange = TheGame->TickCount;
			
			if (NumTries >= 9 && SuccessTime == -1) SuccessTime = TheGame->TickCount;
			
			break;
		}
	}
}
