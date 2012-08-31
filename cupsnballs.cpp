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

#include "cupsnballs.h"
#include "Minigame.h"

BPMiniGame_CupsNBalls::BPMiniGame_CupsNBalls(BPGame* game) : BPMiniGame(game) {
	GameTitle = "Cups 'n' Balls";
	GameHelp = "Pull back the string on your catapult and fire balls into one of the green cups - but don't get them into the red cup or you'll lose points!";
	GameHelp2 = "This is one of the few games where your score isn't affected by how long you take, so pull back the string on the catapult and wait for the cups to move to where you want them to be. Then when you're ready, fire off a shot - or, better yet, fire off lots of shots!";
	
	MiniGameType = ACTION;
	
	sfcBackground = TheGame->LoadBitmap("clouds", 320, 480);
	sfcBall = TheGame->LoadBitmap("ball", 32, 32);
	sfcPuff = TheGame->LoadBitmap("puff", 64, 64);
	sfcSlingshot = TheGame->LoadBitmap("slingshot", 80, 90);
	
	sfcGreenCup = TheGame->LoadBitmap("green_cup", 86, 44);
	sfcRedCup = TheGame->LoadBitmap("red_cup", 86, 44);
	
	BallHalfWidth = sfcBall->Width / 2;
	BallHalfHeight = sfcBall->Height / 2;
	
	CupHalfWidth = sfcGreenCup->Width / 2;
	CupHalfHeight = sfcGreenCup->Height / 2;
	
	SlingshotHalfWidth = sfcSlingshot->Width / 2;
	
	SlingshotX = 123;
	SlingshotY = 296;
	
	// how low down the cup front picture is, in pixels; this gives us better collision detection
	CupFrontOffset = 19;
	Ammo = 20;
	Score = 0;
	
	sfcScore = NULL;
	sfcAmmo = NULL;
	SetScore();
	SetAmmo();
	
	CreateCup(0, 0);
	CreateCup(90, 0);
	CreateCup(180, 0);
	CreateCup(270, 1);
}

BPMiniGame_CupsNBalls::~BPMiniGame_CupsNBalls() {
	SAFE_DELETE(sfcBackground);
	SAFE_DELETE(sfcBall);
	SAFE_DELETE(sfcPuff);
	SAFE_DELETE(sfcSlingshot);
	SAFE_DELETE(sfcGreenCup);
	SAFE_DELETE(sfcRedCup);
	
	SAFE_DELETE(sfcScore);
	SAFE_DELETE(sfcAmmo);
	
	Balls.Clear();
	Cups.Clear();
}

void BPMiniGame_CupsNBalls::Start() {
	TimeStarted = TheGame->TickCount;
}

int BPMiniGame_CupsNBalls::GetWeight() {
	return MinMax(round(Score * 31));
}
		
void BPMiniGame_CupsNBalls::Render() {
	sfcBackground->Draw(0, 0);
	
	for (int i = 0; i < Cups.Count; ++i) {
		BPMiniGame_CupsNBalls_Cup* cup = Cups[i];
		if (cup->Type == 0) {
			sfcGreenCup->Draw(Round(cup->X), Round(cup->Y));
		} else {
			sfcRedCup->Draw(Round(cup->X), Round(cup->Y));
		}
	}
	
	for (int i = 0; i < Balls.Count; ++i) {
		BPMiniGame_CupsNBalls_Ball* ball = Balls[i];
		
		if (ball->HitTime == 0) {
			if (ball->YSpeed <= 0) {
				sfcBall->Draw(Round(ball->X), Round(ball->Y));
			}
		} else {
			sfcPuff->Draw(Round(ball->X - 10), Round(ball->Y - 5));
		}
	}
	
	for (int i = 0; i < Balls.Count; ++i) {
		BPMiniGame_CupsNBalls_Ball* ball = Balls[i];
		
		if (ball->YSpeed >= 0) {
			sfcBall->Draw(Round(ball->X), Round(ball->Y));
		}
	}
	
	sfcSlingshot->Draw(SlingshotX, SlingshotY);
	
	if (IsStringPulled && MouseMoved) {
		TheGame->DrawLine(SlingshotX + 5, SlingshotY, LastMousePos.X, LastMousePos.Y, TheGame->White, 3.0f);
		TheGame->DrawLine(SlingshotX - 5 + sfcSlingshot->Width, SlingshotY, LastMousePos.X, LastMousePos.Y, TheGame->White, 3.0f);
		
		sfcBall->Draw(LastMousePos.X - BallHalfWidth, LastMousePos.Y - BallHalfHeight);
	} else {
		TheGame->DrawLine(SlingshotX + 5, SlingshotY, SlingshotX - 5 + sfcSlingshot->Width, SlingshotY, TheGame->White, 3.0f);
	}

	TheGame->DrawString(sfcAmmo, (*TheGame->Black), 10, 380);
	TheGame->DrawString(sfcScore, (*TheGame->Black), 182, 380);
}
		
void BPMiniGame_CupsNBalls::Tick() {
	if (Ammo == 0 && Balls.Count == 0) {
		Success();
	}
	
	for (int i = Cups.Count - 1; i >= 0; --i) {
		BPMiniGame_CupsNBalls_Cup* cup = Cups[i];
		
		cup->SinVal += 20.0 * TheGame->ElapsedSeconds;
		
		cup->X = MiniGameHalfWidth + (sin(cup->SinVal * M_PI / 180) * 110) - CupHalfWidth;
		cup->Y = MiniGameHalfHeight - 100 + (cos(cup->SinVal * M_PI / 180) * 50) - CupHalfHeight;
		
		for (int j = Balls.Count - 1; j >= 0; --j) {
			BPMiniGame_CupsNBalls_Ball* ball = Balls[j];
			if (ball->HitTime == 0) {
				// only balls (rather than ex-balls) can collide
				if (ball->YSpeed <= 0) {
					// only falling balls can collide
					
					if (TheGame->RectOverRect(Round(ball->X), Round(ball->Y), sfcBall->Width, sfcBall->Height, Round(cup->X) + 20, Round(cup->Y) + CupFrontOffset - 10, sfcGreenCup->Width - 40, 1)) {
						ball->HitTime = TheGame->TickCount;
						
						if (cup->Type == 0) {
							TheGame->PlaySound("balloon_pop");
							Score += 1;
						} else {
							TheGame->PlaySound("wrong");
							Score -= 1;
						}
						
						SetScore();
						
						continue;
					}
				}
			} else {
				if (ball->HitTime + 200 < TheGame->TickCount) {
					Balls.RemoveAt(j);
				}
			}
		}
	}
	
	for (int i = Balls.Count - 1; i >= 0; --i) {
		BPMiniGame_CupsNBalls_Ball* ball = Balls[i];
		
		if (ball->HitTime == 0) {
			// only move if still "alive"
			ball->X -= ball->XSpeed * TheGame->ElapsedSeconds;
			ball->Y -= ball->YSpeed * TheGame->ElapsedSeconds;
			ball->YSpeed -= 1600.0f * TheGame->ElapsedSeconds;
			
			if (ball->Y > 490) Balls.RemoveAt(i);
		}
	}
}
		
void BPMiniGame_CupsNBalls::OnMouseDown() {
	MouseMoved = false;
	LastMousePos = TouchEvent;
	
	if (Ammo > 0) {
		if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, SlingshotX - 10, SlingshotY - 10, sfcSlingshot->Width + 20, sfcSlingshot->Height + 20)) {
			IsStringPulled = true;
		}
	}
}
		
void BPMiniGame_CupsNBalls::OnMouseMove() {
	if (LastMousePos != TouchEvent) {
		MouseMoved = true;
	}
	
	LastMousePos = TouchEvent;
}
		
void BPMiniGame_CupsNBalls::OnMouseUp() {
	if (IsStringPulled && MouseMoved) {
		IsStringPulled = false;
		
		// don't allow people to pull the string up then fire down into the cups
		if (TouchEvent.Y < SlingshotY) return;
		
		FireBullet();
	} else {
		IsStringPulled = false;
	}
}
		
void BPMiniGame_CupsNBalls::FireBullet() {
	TheGame->PlaySound("swoosh_long");
	
	// in Marathon Mode, ammo is infinite
	if (!MarathonMode) {
		--Ammo;
		SetAmmo();
	}
	
	BPMiniGame_CupsNBalls_Ball* ball = new BPMiniGame_CupsNBalls_Ball();
	ball->X = TouchEvent.X - BallHalfWidth;
	ball->Y = TouchEvent.Y - BallHalfHeight;
	
	double xdiff = TouchEvent.X - (SlingshotX + SlingshotHalfWidth);
	double ydiff = TouchEvent.Y - SlingshotY;
	double totaldiff = (fabs(xdiff) + fabs(ydiff)) / 4;
	double angle = atan(ydiff / xdiff) / (M_PI / 180);
	
	if (xdiff < 0) angle += 180;
	if (xdiff >= 0 && ydiff < 0) angle += 360;
	
	ball->XSpeed = (float)(cos(angle * M_PI / 180) * totaldiff) * 40.0f;
	ball->YSpeed = (float)(sin(angle * M_PI / 180) * totaldiff) * 35.0f;
	
	Balls.Add(ball);
}
		
void BPMiniGame_CupsNBalls::CreateCup(int sinval, int type) {
	BPMiniGame_CupsNBalls_Cup* cup = new BPMiniGame_CupsNBalls_Cup();
	
	cup->Type = type;
	cup->SinVal = sinval;
	
	Cups.Add(cup);
}
		
void BPMiniGame_CupsNBalls::SetMarathon() {
	MarathonMode = true;
	SAFE_DELETE(GameHelp);
	GameHelp = "Pull back the string on your catapult and fire balls into one of the green cups - but don't get them into the red cup or you'll lose points!";
	Ammo = 999;
}

void BPMiniGame_CupsNBalls::SetScore() {			
	if (Score > 0) {
		ostringstream ScoreStr;
		ScoreStr << "Score: " << TheGame->SeparateThousands(Score);
		TheGame->AllocString(&sfcScore, ScoreStr.str().c_str(), NORMAL, 128, 50, RIGHT);
	} else {
		TheGame->AllocString(&sfcScore, "Score: 0", NORMAL, 128, 50, RIGHT);
		Score = 0; // don't let it go below zero
	}
	

}

void BPMiniGame_CupsNBalls::SetAmmo() {		
	if (Ammo > 0) {
		ostringstream AmmoStr;
		AmmoStr << "Ammo: " << Ammo;
		TheGame->AllocString(&sfcAmmo, AmmoStr.str().c_str(), NORMAL, 128, 50, LEFT);
	} else {
		TheGame->AllocString(&sfcAmmo, "Ammo: 0", NORMAL, 128, 50, LEFT);
	}
	

}
