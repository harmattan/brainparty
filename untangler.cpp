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

#include "untangler.h"
#include "Minigame.h"
		
BPMiniGame_Untangler::BPMiniGame_Untangler(BPGame* game) : BPMiniGame(game) {
	SelectedPoint = NULL;
	sfcCircleHandle = TheGame->LoadBitmap("circle_handle", 16, 16);
	sfcBackground = TheGame->LoadBitmap("stars_glow", 320, 416);
	
	LastMoveTime = CurrentLevel = TimeStarted = 0;
	SuccessTime = -1;
	
	GenerateStarfield(Stars);
	
	GameTitle = "Untangler";
	GameHelp = "Drag the line ends around so the lines don't cross. You need to ensure that none of the lines cross to advance!";
	GameHelp2 = "The white circles at the ends of each line can be moved. If a line is green, it means it doesn't cross any other line. If it is red, it's crossing at least one other line. You need to move the line ends until no lines cross any others.";
	
	MiniGameType = PUZZLE;
	
	LevelUp();
}

BPMiniGame_Untangler::~BPMiniGame_Untangler() {
	TanglePoints.Clear();
	Stars.Clear();
	
	SAFE_DELETE(SelectedPoint);
	
	SAFE_DELETE(sfcCircleHandle);
	SAFE_DELETE(sfcBackground);
}

void BPMiniGame_Untangler::Start() {
	TimeStarted = TheGame->TickCount;
}

int BPMiniGame_Untangler::GetWeight() {
	float TimePassed = (TheGame->TickCount - TimeStarted) / 1000.0f;
	return MinMax(530 - floor(TimePassed * 1.1));
}

void BPMiniGame_Untangler::Render() {
	TheGame->DrawImage(sfcBackground, 0, 0);
	
	DrawStarfield(Stars);
	
	bool p1_collides;
	bool p2_collides;
	
	bool any_collisions = false;
	
	for (int i = 0; i < TanglePoints.Count; ++i) {
		BPMiniGame_Untangler_Point* point = TanglePoints[i];
		
		p1_collides = false;
		p2_collides = false;
		
		// does this line collide with any other?
		for (int j = 0; j < TanglePoints.Count; ++j) {
			BPMiniGame_Untangler_Point* other_point = TanglePoints[j];
			//if (point == other_point) continue;
			
			// if our first point collides with the other first point
			if (LinesCross(point->Pos.X, point->Pos.Y, point->Connect1->Pos.X, point->Connect1->Pos.Y, other_point->Pos.X, other_point->Pos.Y, other_point->Connect1->Pos.X, other_point->Connect1->Pos.Y) == 1) {
				p1_collides = true;
			} else {
				// if our first point collides with the other second point
				if (LinesCross(point->Pos.X, point->Pos.Y, point->Connect1->Pos.X, point->Connect1->Pos.Y, other_point->Pos.X, other_point->Pos.Y, other_point->Connect2->Pos.X, other_point->Connect2->Pos.Y) == 1) {
					p1_collides = true;
				}
			}
			
			// if our second point collides with the other first point
			if (LinesCross(point->Pos.X, point->Pos.Y, point->Connect2->Pos.X, point->Connect2->Pos.Y, other_point->Pos.X, other_point->Pos.Y, other_point->Connect1->Pos.X, other_point->Connect1->Pos.Y) == 1) {
				p2_collides = true;
			} else if (LinesCross(point->Pos.X, point->Pos.Y, point->Connect2->Pos.X, point->Connect2->Pos.Y, other_point->Pos.X, other_point->Pos.Y, other_point->Connect2->Pos.X, other_point->Connect2->Pos.Y) == 1) {
				// if our second point collides with the other second point
				p2_collides = true;
			}
		}
		
		if (p1_collides) {
			any_collisions = true;
			TheGame->DrawLine(point->Pos.X, point->Pos.Y, point->Connect1->Pos.X, point->Connect1->Pos.Y, TheGame->Red, 3.0f);
		} else {
			TheGame->DrawLine(point->Pos.X, point->Pos.Y, point->Connect1->Pos.X, point->Connect1->Pos.Y, TheGame->Green, 3.0f);
		}
		
		if (p2_collides) {
			any_collisions = true;
			TheGame->DrawLine(point->Pos.X, point->Pos.Y, point->Connect2->Pos.X, point->Connect2->Pos.Y, TheGame->Red, 3.0f);
		} else {
			TheGame->DrawLine(point->Pos.X, point->Pos.Y, point->Connect2->Pos.X, point->Connect2->Pos.Y, TheGame->Green, 3.0f);
		}
	}

	// draw blocks separately to the lines so they always appear on top
	for (int i = 0; i < TanglePoints.Count; ++i) {
		BPMiniGame_Untangler_Point* point = TanglePoints[i];
		TheGame->DrawImage(sfcCircleHandle, point->Pos.X - 8, point->Pos.Y - 10);
	}
	
	if (SelectedPoint == NULL && !any_collisions && LastMoveTime + 500 < TheGame->TickCount) {
		// success!
		TheGame->PlaySound("correct");
		LevelUp();
	}
}

void BPMiniGame_Untangler::Tick() {
	if (SuccessTime != -1 && SuccessTime + 250 < TheGame->TickCount) {
		Success();
		return;
	}
	
	UpdateStarfield(Stars);
}

void BPMiniGame_Untangler::OnMouseUp() {
	if (SelectedPoint != NULL) {
		// don't let people drag the points off the screen!
		if (SelectedPoint->Pos.X < Size) {
			SelectedPoint->Pos.X = Size;
		} else if (SelectedPoint->Pos.X > MiniGameWidth - Size) {
			SelectedPoint->Pos.X = MiniGameWidth - Size;
		}
		
		if (SelectedPoint->Pos.Y < Size) {
			SelectedPoint->Pos.Y = Size;
		} else if (SelectedPoint->Pos.Y > 416 - Size) {
			SelectedPoint->Pos.Y = 416 - Size;
		}
	}
	
	SelectedPoint = NULL;
	LastMoveTime = TheGame->TickCount;
}

void BPMiniGame_Untangler::OnMouseDown() {
	BPMiniGame_Untangler_Point* closest = NULL;
	float distance = 999.0f;
	
	for (int i = 0; i < TanglePoints.Count; ++i) {
		BPMiniGame_Untangler_Point* point = TanglePoints[i];
		
		float thisdist = BPPoint::DistanceSquared(BPPoint(TouchEvent.X, TouchEvent.Y), BPPoint(point->Pos.X, point->Pos.Y));
		
		if (thisdist < distance) {
			closest = point;
			distance = thisdist;
		}
		
//		if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, point->Pos.X - (HalfSize + 8), point->Pos.Y - (HalfSize + 8), (Size + 16), (Size + 16))) {
//			SelectedPoint = point;
//			break;
//		}
	}
	
	if (distance < 2304.0f) { // 48 * 48
		SelectedPoint = closest;
	}
}

void BPMiniGame_Untangler::OnMouseMove() {
	if (SelectedPoint != NULL) {
		SelectedPoint->Pos = TouchEvent;
	}
}

float BPMiniGame_Untangler::abs2(float x) {
	if (x < 0) { return -x; }
	return x;
}

int BPMiniGame_Untangler::LinesCross(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3) {
	float d = (x1 - x0) * (y3 - y2) - (y1 - y0) * (x3 - x2);
	if (abs2(d) < 0.001) { return -1; }
	float AB = ((y0 - y2) * (x3 - x2) - (x0 - x2) * (y3 - y2)) / d;
	if (AB > 0.0 && AB < 1.0) {
		float CD = ((y0 - y2) * (x1 - x0) - (x0 - x2) * (y1 - y0)) / d;
		if (CD > 0.0 && CD < 1.0) {
			//linx = x0 + AB * (x1 - x0);
			//liny = y0 + AB * (y1 - y0);
			return 1;
		}
	}
	return 0;
}

void BPMiniGame_Untangler::LevelUp() {
	if (!MarathonMode && CurrentLevel >= 8 && SuccessTime == -1) {
		SuccessTime = TheGame->TickCount;
		return;
	}
	
	if (SuccessTime != -1) {
		// we've finished - bail out!
		return;
	}
	
	++CurrentLevel;
	
	do {	
		TanglePoints.Clear();
		
		// first, add all the lines
		for (int i = 0; i < CurrentLevel + 5; ++i) {
			BPMiniGame_Untangler_Point* line = new BPMiniGame_Untangler_Point();
			line->Pos.X = TheGame->RandomRange(5, 235);
			line->Pos.Y = TheGame->RandomRange(5, 290);
			TanglePoints.Add(line);
		}
		
		for (int i = 0; i < TanglePoints.Count; ++i) {
			if (i == 0) {
				// this is the first item; set its Connect1 value to be the last item
				TanglePoints[i]->Connect1 = TanglePoints[TanglePoints.Count - 1];
			} else {
				TanglePoints[i]->Connect1 = TanglePoints[i - 1];
			}
			
			if (i == TanglePoints.Count - 1) {
				// this is the last item; set its Connect2 value to be the first item
				TanglePoints[i]->Connect2 = TanglePoints[0];
			} else {
				TanglePoints[i]->Connect2 = TanglePoints[i + 1];
			}
		}
	} while (!AnyCollisions());
}

bool BPMiniGame_Untangler::AnyCollisions() {
	for (int i = 0; i < TanglePoints.Count; ++i) {
		BPMiniGame_Untangler_Point* point = TanglePoints[i];
		
		// does this line collide with any other?
		for (int j = 0; j < TanglePoints.Count; ++j) {
			BPMiniGame_Untangler_Point* other_point = TanglePoints[j];
			//if (point == other_point) continue;
			
			// if our first point collides with the other first point
			if (LinesCross(point->Pos.X, point->Pos.Y, point->Connect1->Pos.X, point->Connect1->Pos.Y, other_point->Pos.X, other_point->Pos.Y, other_point->Connect1->Pos.X, other_point->Connect1->Pos.Y) == 1) {
				return true;
			} else {
				// if our first point collides with the other second point
				if (LinesCross(point->Pos.X, point->Pos.Y, point->Connect1->Pos.X, point->Connect1->Pos.Y, other_point->Pos.X, other_point->Pos.Y, other_point->Connect2->Pos.X, other_point->Connect2->Pos.Y) == 1) {
					return true;
				}
			}
			
			// if our second point collides with the other first point
			if (LinesCross(point->Pos.X, point->Pos.Y, point->Connect2->Pos.X, point->Connect2->Pos.Y, other_point->Pos.X, other_point->Pos.Y, other_point->Connect1->Pos.X, other_point->Connect1->Pos.Y) == 1) {
				return true;
			} else if (LinesCross(point->Pos.X, point->Pos.Y, point->Connect2->Pos.X, point->Connect2->Pos.Y, other_point->Pos.X, other_point->Pos.Y, other_point->Connect2->Pos.X, other_point->Connect2->Pos.Y) == 1) {
				// if our second point collides with the other second point
				return true;
			}
		}
	}
	
	// if we're stil here, there are no collisions
	return false;
}

void BPMiniGame_Untangler::SetMarathon() {
	MarathonMode = true;
}
