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

#include "sharpshooter.h"
#include "Minigame.h"
				
BPMiniGame_Sharpshooter::BPMiniGame_Sharpshooter(BPGame* game) : BPMiniGame(game) {
	sfcBackground = TheGame->LoadBitmap("sharpshooter", 320, 416);
	sfcTarget = TheGame->LoadBitmap("target", 64, 64);
	sfcDontShoot = TheGame->LoadBitmap("marble_red", 64, 64);
	
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
	
	LastCreatedTime = NumTargets = NumHit = 0;
	CreateDelay = 500;
	TargetLife = 1700;
	PauseTime = 0;
	
	SuccessTime = -1;
	
	GameTitle = "Sharpshooter";
	GameHelp = "Tap the screen to shoot as many targets as you can, but watch out - shooting bubbles will cost you points!";
	GameHelp2 = "When shooting the targets, don't worry about ammo: you have infinite shots, and firing more doesn't lower your score. For maximum points, don't miss any targets!";
	
	MiniGameType = ACTION;
}

BPMiniGame_Sharpshooter::~BPMiniGame_Sharpshooter() {
	SAFE_DELETE(sfcBackground);
	SAFE_DELETE(sfcTarget);
	SAFE_DELETE(sfcDontShoot);
	
	Targets.Clear();
	Positions.Clear();
}

void BPMiniGame_Sharpshooter::Start() {
	LastCreatedTime = TheGame->TickCount + 2000;
}

int BPMiniGame_Sharpshooter::GetWeight() {
	int nummissed = 64 - NumHit;
	return MinMax(500 - (nummissed * 6));
}

void BPMiniGame_Sharpshooter::Render() {
	TheGame->DrawImage(sfcBackground, 0, 0);
	
	for (int i = 0; i < Targets.Count; ++i) {
		BPMiniGame_Sharpshooter_Target* target = Targets[i];
		
		if (target->HitTime != -1 || target->CreatedTime + 150 > TheGame->TickCount) {
			float diff;
			
			if (target->HitTime == -1) {
				// target is appearing
				diff = (((target->CreatedTime + 150) - TheGame->TickCount) / 150.0f);
			} else {
				// target is disappearing
				diff = (TheGame->TickCount - target->HitTime) / 150.0f;

			}
			
			if (diff > 1) diff = 1;
			Colour col = Colour(1.0f, 1.0f, 1.0f, TheGame->Lerp(1.0f, 0, diff));
			
			if (target->HitTime != -1) {
				diff = 1 - diff;
			}
			
			if (target->TargetType == -1) {
				TheGame->DrawImage(sfcTarget, target->X + sfcTarget->HalfWidth, target->Y + sfcTarget->HalfHeight, 0.0f, diff, col);
			} else {
				TheGame->DrawImage(sfcDontShoot, target->X + sfcTarget->HalfWidth, target->Y + sfcTarget->HalfHeight, 0.0f, diff, col);
			}
		} else {
			if (target->TargetType == -1) {
				TheGame->DrawImage(sfcTarget, target->X + sfcTarget->HalfWidth, target->Y + sfcTarget->HalfHeight, 0, 1.0f, (*TheGame->White));
			} else {
				TheGame->DrawImage(sfcDontShoot, target->X + sfcTarget->HalfWidth, target->Y + sfcTarget->HalfHeight, 0, 1.0f, (*TheGame->White));
			}
		}
	}
}

void BPMiniGame_Sharpshooter::Tick() {
	if (SuccessTime != -1 && SuccessTime + 500 < TheGame->TickCount) {
		Success();
	}
	
	if (LastCreatedTime + CreateDelay + PauseTime < TheGame->TickCount) {		
		switch (TheGame->RandomRange(0, 5)) {
			case 0:
			case 1:
			case 2:
			case 3:
				CreateTarget(-1);
				break;
			case 4:
			case 5:
				CreateTarget(1);
				break;
				
			case 6:
				// take a breather
				break;
		}
	}
	
	for (int i = Targets.Count - 1; i >= 0; --i) {
		BPMiniGame_Sharpshooter_Target* target = Targets[i];
		
		if (target->HitTime != -1 && target->HitTime + 150 < TheGame->TickCount) {
			Targets.RemoveAt(i);
			continue;
		}
		
		if (target->CreatedTime != -1 && target->CreatedTime + TargetLife < TheGame->TickCount) {
			if (target->HitTime == -1) {
				target->HitTime = TheGame->TickCount;
			}
		}
	}
}

void BPMiniGame_Sharpshooter::OnMouseDown() {	
	for (int i = Targets.Count - 1; i >= 0; --i) {
		BPMiniGame_Sharpshooter_Target* target = Targets[i];
		
		if (target->HitTime != -1) continue;
		
		float distance = BPPoint::DistanceSquared(TouchEvent, BPPoint(target->X + sfcTarget->HalfWidth, target->Y + sfcTarget->HalfWidth));
		
		if (distance < sfcTarget->HalfWidth * sfcTarget->HalfWidth) {
			if (target->TargetType == -1) {
				target->HitTime = TheGame->TickCount;
				++NumHit;
				
				TheGame->PlaySound("gun2");
			} else {
				target->HitTime = TheGame->TickCount;
				NumHit -= 5;
				
				TheGame->PlaySound("zap");
			}
			
			target->Hit = true;
			
			return;
		}
	}
	
	// if we're still here, it means we hit nothing - play the quieter of the two gun sounds
	TheGame->PlaySound("gun1");	
}

void BPMiniGame_Sharpshooter::OnMouseMove() {
	
}

void BPMiniGame_Sharpshooter::OnMouseUp() {

}

void BPMiniGame_Sharpshooter::CreateTarget(int type) {
	if (NumTargets > 64) {
		// create no more targets
		if (Targets.Count == 0 && SuccessTime == -1) SuccessTime = TheGame->TickCount;
		return;
	}
	
	if (TheGame->RandomRange(0, 4) == 2) {
		PauseTime = 2000;
	} else {
		PauseTime = 0;
	}
		
	
	Positions.Shuffle();
	
	int i;
	bool usethis;
	
	for (i = 0; i < Positions.Count; ++i) {
		// find the first free position
		usethis = true;
		
		for (int j = 0; j < Targets.Count; ++j) {
			if (Targets[j]->X == Positions[i]->X && Targets[j]->Y == Positions[i]->Y) {
				usethis = false;
				break;
			}
		}
		
		if (usethis) break;
	}
	
	if (!usethis) {
		// no free spaces!
		return;
	}
	
	BPMiniGame_Sharpshooter_Target* target = new BPMiniGame_Sharpshooter_Target();
	target->CreatedTime = TheGame->TickCount;
	target->X = Positions[i]->X;
	target->Y = Positions[i]->Y;
	target->TargetType = type;
	
	Targets.Add(target);
	
	LastCreatedTime = TheGame->TickCount;
	CreateDelay -= 2;
	++NumTargets;
}
