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

#include "moonjump.h"
#include "Minigame.h"

BPMiniGame_MoonJump::BPMiniGame_MoonJump(BPGame* game) : BPMiniGame(game) {
	sfcBackground1 = TheGame->LoadBitmap("moonjump1", 320, 512);
	sfcBackground2 = TheGame->LoadBitmap("moonjump2", 320, 512);
	sfcBackground3 = TheGame->LoadBitmap("moonjump3", 320, 512);
	sfcBackground4 = TheGame->LoadBitmap("moonjump4", 320, 512);
	sfcBackground5 = TheGame->LoadBitmap("moonjump5", 320, 512);
	sfcBackground6 = TheGame->LoadBitmap("moonjump6", 320, 512);
	
	sfcSheepLeft = TheGame->LoadBitmap("sheep_left", 38, 34);
	sfcSheepRight = TheGame->LoadBitmap("sheep_right", 38, 34);
	
	LastCreateTime = 0;
	Y = -96;
	SuccessTime = -1;
	
	SheepCreateDelay = 6000;
	
	GameTitle = "Moon Jump";
	GameHelp = "Tap on your sheep to have them jump in the air, but make sure you don't lose any! How high can you make them fly?";
	GameHelp2 = "Be careful: every few seconds a new sheep will join your group, and you need to make sure they all stay alive!";
	
	MiniGameType = LIVELY;
}

BPMiniGame_MoonJump::~BPMiniGame_MoonJump() {
	SAFE_DELETE(sfcBackground1);
	SAFE_DELETE(sfcBackground2);
	SAFE_DELETE(sfcBackground3);
	SAFE_DELETE(sfcBackground4);
	SAFE_DELETE(sfcBackground5);
	SAFE_DELETE(sfcBackground6);
	
	SAFE_DELETE(sfcSheepLeft);
	SAFE_DELETE(sfcSheepRight);
	
	Sheep.Clear();
}

void BPMiniGame_MoonJump::Start() {	
	LastCreateTime = TheGame->TickCount + 3000;
	CreateSheep();
}

int BPMiniGame_MoonJump::GetWeight() {	
	return MinMax(Y / 4.7f); // ensures that when they reach the top they get 500g; everything else is pro rata
}

void BPMiniGame_MoonJump::Render() {
	TheGame->DrawImage(sfcBackground6, 0, -2560 + round(Y));
	TheGame->DrawImage(sfcBackground5, 0, -2048 + round(Y));
	TheGame->DrawImage(sfcBackground4, 0, -1536 + round(Y));
	TheGame->DrawImage(sfcBackground3, 0, -1024 + round(Y));
	TheGame->DrawImage(sfcBackground2, 0, -512 + round(Y));
	TheGame->DrawImage(sfcBackground1, 0, round(Y));
		
	for (int i = 0; i < Sheep.Count; ++i) {
		BPMiniGame_MoonJump_Sheep* sheep = Sheep[i];
		
		if (sheep->MovingRight) {
			TheGame->DrawImage(sfcSheepRight, round(sheep->X), round(sheep->Y));
		} else {
			TheGame->DrawImage(sfcSheepLeft, round(sheep->X), round(sheep->Y));
		}
	}
}

void BPMiniGame_MoonJump::Tick() {
	if (SuccessTime != -1 && SuccessTime + 1000 < TheGame->TickCount) {
		Success();
		return;
	}
	
	if (Y < 2372) {
		// only create balloons when we're in the atmosphere!
		if (LastCreateTime + SheepCreateDelay < TheGame->TickCount) {
			CreateSheep();
		}
	}
	
	for (int i = 0; i < Sheep.Count; ++i) {
		BPMiniGame_MoonJump_Sheep* sheep = Sheep[i];
		
		sheep->X += sheep->XSpeed * TheGame->ElapsedSeconds;
		sheep->Y += sheep->YSpeed * TheGame->ElapsedSeconds;
		
		sheep->YSpeed += 85.0f * TheGame->ElapsedSeconds;
		if (sheep->YSpeed > 200) sheep->YSpeed = 200;
		
		sheep->XSpeed *= 0.999f;
		
		if (sheep->OnScreen) {
			if (sheep->Y < 0) {
				sheep->Y = 0;
				sheep->YSpeed = 0;
			}
		} else {
			if (sheep->Y >= 0) {
				sheep->OnScreen = true;
			}
		}
		
		if (sheep->X < 0) {
			sheep->X = 0;
			sheep->MovingRight = true;
			sheep->XSpeed = abs((int)round(sheep->XSpeed));
		}
		
		if (sheep->X + sfcSheepRight->Width > MiniGameWidth) {
			sheep->X = MiniGameWidth - sfcSheepRight->Width;
			sheep->MovingRight = false;
			sheep->XSpeed = -sheep->XSpeed;
		}
		
		if (sheep->Y > MiniGameHeight) {
			Success();
		}
	}
	
	
	Y += 35.0f * TheGame->ElapsedSeconds;
	
	if (Y >= 2560.0f) {
		if (SuccessTime == -1) SuccessTime = TheGame->TickCount;
		Y = 2560.0f;
	}
}

void BPMiniGame_MoonJump::CreateSheep() {	
	BPMiniGame_MoonJump_Sheep* sheep = new BPMiniGame_MoonJump_Sheep();
	sheep->X = TheGame->RandomRange(50, 270);
	sheep->Y = -sfcSheepLeft->Height;
	sheep->XSpeed = TheGame->RandomRange(0, 10);
	sheep->YSpeed = 0.0f;
	Sheep.Add(sheep);
	
	LastCreateTime = TheGame->TickCount;
	SheepCreateDelay += 600;
}

void BPMiniGame_MoonJump::OnMouseDown() {
	
}

void BPMiniGame_MoonJump::OnMouseMove() {
	
}

void BPMiniGame_MoonJump::OnMouseUp() {
	for (int i = 0; i < Sheep.Count; ++i) {
		BPMiniGame_MoonJump_Sheep* sheep = Sheep[i];
		
		if (sheep->YSpeed < 0) continue; // don't let him jump again if he's still jumping
		
		if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, round(sheep->X) - 15, round(sheep->Y) - 15, sfcSheepLeft->Width + 30, sfcSheepRight->Height + 30)) {
			switch (TheGame->RandomRange(0, 10)) {
				case 0:
				case 1:
				case 2:
				case 3:
				case 4:
				case 5:
					TheGame->PlaySound("baa");
					break;
					
				case 6:
				case 7:
				case 8:
				case 9:
					TheGame->PlaySound("baa2");
					break;
					
				case 10:
					TheGame->PlaySound("baa3");
					break;
			}
			
			int halfpoint = round(sheep->X) + 19;
			int diff = TouchEvent.X - halfpoint;
			
			if (diff > 0) {
				// we clicked to the right of the middle
				sheep->XSpeed = -diff * 3;
				sheep->MovingRight = false;
			} else {
				// we clicked to the left
				sheep->XSpeed = -diff * 3;
				sheep->MovingRight = true;
			}
			
			sheep->YSpeed = -120.0f;
		}
	}
}
