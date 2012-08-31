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

#include "setfinder.h"
#include "Minigame.h"		

BPMiniGame_SetFinder::BPMiniGame_SetFinder(BPGame* game) : BPMiniGame(game) {
	sfcBackground = TheGame->LoadBitmap("setfinder", 320, 416);
	sfcSelectedHi = TheGame->LoadBitmap("selected48", 48, 48);
	
	sfcCircleBlueEmpty = TheGame->LoadBitmap("set_circle_blue_empty", 45, 45);
	sfcCircleBlueFilled = TheGame->LoadBitmap("set_circle_blue_filled", 45, 45);
	sfcCircleBlueStripy = TheGame->LoadBitmap("set_circle_blue_stripy", 45, 45);
	sfcCircleGreenEmpty = TheGame->LoadBitmap("set_circle_green_empty", 45, 45);
	sfcCircleGreenFilled = TheGame->LoadBitmap("set_circle_green_filled", 45, 45);
	sfcCircleGreenStripy = TheGame->LoadBitmap("set_circle_green_stripy", 45, 45);
	sfcCircleRedEmpty = TheGame->LoadBitmap("set_circle_red_empty", 45, 45);
	sfcCircleRedFilled = TheGame->LoadBitmap("set_circle_red_filled", 45, 45);
	sfcCircleRedStripy = TheGame->LoadBitmap("set_circle_red_stripy", 45, 45);
	
	sfcSquareBlueEmpty = TheGame->LoadBitmap("set_square_blue_empty", 45, 45);
	sfcSquareBlueFilled = TheGame->LoadBitmap("set_square_blue_filled", 45, 45);
	sfcSquareBlueStripy = TheGame->LoadBitmap("set_square_blue_stripy", 45, 45);
	sfcSquareGreenEmpty = TheGame->LoadBitmap("set_square_green_empty", 45, 45);
	sfcSquareGreenFilled = TheGame->LoadBitmap("set_square_green_filled", 45, 45);
	sfcSquareGreenStripy = TheGame->LoadBitmap("set_square_green_stripy", 45, 45);
	sfcSquareRedEmpty = TheGame->LoadBitmap("set_square_red_empty", 45, 45);
	sfcSquareRedFilled = TheGame->LoadBitmap("set_square_red_filled", 45, 45);
	sfcSquareRedStripy = TheGame->LoadBitmap("set_square_red_stripy", 45, 45);
	
	sfcTriangleBlueEmpty = TheGame->LoadBitmap("set_triangle_blue_empty", 45, 45);
	sfcTriangleBlueFilled = TheGame->LoadBitmap("set_triangle_blue_filled", 45, 45);
	sfcTriangleBlueStripy = TheGame->LoadBitmap("set_triangle_blue_stripy", 45, 45);
	sfcTriangleGreenEmpty = TheGame->LoadBitmap("set_triangle_green_empty", 45, 45);
	sfcTriangleGreenFilled = TheGame->LoadBitmap("set_triangle_green_filled", 45, 45);
	sfcTriangleGreenStripy = TheGame->LoadBitmap("set_triangle_green_stripy", 45, 45);
	sfcTriangleRedEmpty = TheGame->LoadBitmap("set_triangle_red_empty", 45, 45);
	sfcTriangleRedFilled = TheGame->LoadBitmap("set_triangle_red_filled", 45, 45);
	sfcTriangleRedStripy = TheGame->LoadBitmap("set_triangle_red_stripy", 45, 45);
	
	MatchCount = SuccessTime = TimeStarted = 0;
	
	sfcClock = NULL;
	
	GameTitle = "Set Finder";
	GameHelp = "You must find five sets of three items with different shapes, colours and patterns. For example, an empty green square, a filled blue circle and a stripy red triangle makes one set. Good luck!";
	GameHelp2 = "The key to this game is that each of the three items in your set must have completely different attributes. For example, having two circles and a square isn't allowed, and neither is having two red items and a blue item.";
	
	MiniGameType = PUZZLE;

	BPList<BPPoint> Positions;
	
	for (int i = 0; i < 6; ++i) {
		for (int j = 0; j < 6; ++j) {
			Positions.Add(BPPoint(12 + (i * 50), 63 + (j * 50)));
		}
	}
	
	Positions.Shuffle();
	
	BPList<FillTypes> TheFillTypes;
	TheFillTypes.Add(EMPTY);
	TheFillTypes.Add(STRIPED);
	TheFillTypes.Add(FILLED);
	
	BPList<ShapeTypes> TheShapeTypes;
	TheShapeTypes.Add(CIRCLE);
	TheShapeTypes.Add(SQUARE);
	TheShapeTypes.Add(TRIANGLE);
	
	int loopcount = 0;
	
	for (int i = 0; i < TheFillTypes.Count; ++i) {
		FillTypes filltype = TheFillTypes[i];

		for (int j = 0; j < TheShapeTypes.Count; ++j) {
			ShapeTypes shapetype = TheShapeTypes[j];
			
			BPMiniGame_SetFinder_Item* item;
			
			item = new BPMiniGame_SetFinder_Item();
			item->Col = RED;
			item->FillType = filltype;
			item->ShapeType = shapetype;
			item->X = Positions[loopcount].X;
			item->Y = Positions[loopcount].Y;
			CurrentItems.Add(item);
			
			++loopcount;
			
			item = new BPMiniGame_SetFinder_Item();
			item->Col = GREEN;
			item->FillType = filltype;
			item->ShapeType = shapetype;
			item->X = Positions[loopcount].X;
			item->Y = Positions[loopcount].Y;
			CurrentItems.Add(item);
			
			++loopcount;
			
			item = new BPMiniGame_SetFinder_Item();
			item->Col = BLUE;
			item->FillType = filltype;
			item->ShapeType = shapetype;
			item->X = Positions[loopcount].X;
			item->Y = Positions[loopcount].Y;
			CurrentItems.Add(item);
			
			++loopcount;
		}
	}
}

BPMiniGame_SetFinder::~BPMiniGame_SetFinder() {
	SAFE_DELETE(sfcBackground);
	SAFE_DELETE(sfcSelectedHi);
	
	SAFE_DELETE(sfcCircleBlueEmpty);
	SAFE_DELETE(sfcCircleBlueFilled);
	SAFE_DELETE(sfcCircleBlueStripy);
	SAFE_DELETE(sfcCircleGreenEmpty);
	SAFE_DELETE(sfcCircleGreenFilled);
	SAFE_DELETE(sfcCircleGreenStripy);
	SAFE_DELETE(sfcCircleRedEmpty);
	SAFE_DELETE(sfcCircleRedFilled);
	SAFE_DELETE(sfcCircleRedStripy);
	
	SAFE_DELETE(sfcSquareBlueEmpty);
	SAFE_DELETE(sfcSquareBlueFilled);
	SAFE_DELETE(sfcSquareBlueStripy);
	SAFE_DELETE(sfcSquareGreenEmpty);
	SAFE_DELETE(sfcSquareGreenFilled);
	SAFE_DELETE(sfcSquareGreenStripy);
	SAFE_DELETE(sfcSquareRedEmpty);
	SAFE_DELETE(sfcSquareRedFilled);
	SAFE_DELETE(sfcSquareRedStripy);
	
	SAFE_DELETE(sfcTriangleBlueEmpty);
	SAFE_DELETE(sfcTriangleBlueFilled);
	SAFE_DELETE(sfcTriangleBlueStripy);
	SAFE_DELETE(sfcTriangleGreenEmpty);
	SAFE_DELETE(sfcTriangleGreenFilled);
	SAFE_DELETE(sfcTriangleGreenStripy);
	SAFE_DELETE(sfcTriangleRedEmpty);
	SAFE_DELETE(sfcTriangleRedFilled);
	SAFE_DELETE(sfcTriangleRedStripy);
	
	SAFE_DELETE(sfcClock);
	
	CurrentItems.Clear();
}

void BPMiniGame_SetFinder::Start() {
	TimeStarted = TheGame->TickCount;
}

int BPMiniGame_SetFinder::GetWeight() {
	float TimePassed = (TheGame->TickCount - TimeStarted) / 1000.0f;
	return MinMax(800 - round(TimePassed * 10));
}

void BPMiniGame_SetFinder::Render() {
	static float SelectedRotation = 0.0f;
	static float SelectedScale = 0.0f;
	SelectedRotation += TheGame->ElapsedSeconds * 50.0f;
	SelectedScale += TheGame->ElapsedSeconds;
	if (SelectedScale > 1.0f) SelectedScale = 0.0f;
	
	TheGame->DrawImage(sfcBackground, 0, 0);
	
	Colour drawcol = (*TheGame->White);
	
	for (int i = 0; i < CurrentItems.Count; ++i) {
		BPMiniGame_SetFinder_Item* item = CurrentItems[i];
		
		if (item->MatchTime == -1) {
			drawcol = (*TheGame->White);
		} else {
			float diff = (TheGame->TickCount - item->MatchTime) / 150.0f;
			if (diff > 1) diff = 1;
			drawcol = Colour(1.0f, 1.0f, 1.0f, TheGame->Lerp(0.9f, 0, diff));
		}
		
		switch (item->ShapeType) {
			case CIRCLE:
				switch (item->Col) {
					case BLUE:
						switch (item->FillType) {
							case EMPTY:
								TheGame->DrawImage(sfcCircleBlueEmpty, item->X, item->Y, drawcol);
								break;
								
							case FILLED:
								TheGame->DrawImage(sfcCircleBlueFilled, item->X, item->Y, drawcol);
								break;
								
							case STRIPED:
								TheGame->DrawImage(sfcCircleBlueStripy, item->X, item->Y, drawcol);
								break;
						}
						break;
						
					case GREEN:
						switch (item->FillType) {
							case EMPTY:
								TheGame->DrawImage(sfcCircleGreenEmpty, item->X, item->Y, drawcol);
								break;
								
							case FILLED:
								TheGame->DrawImage(sfcCircleGreenFilled, item->X, item->Y, drawcol);
								break;
								
							case STRIPED:
								TheGame->DrawImage(sfcCircleGreenStripy, item->X, item->Y, drawcol);
								break;
						}
						break;
						
					case RED:
						switch (item->FillType) {
							case EMPTY:
								TheGame->DrawImage(sfcCircleRedEmpty, item->X, item->Y, drawcol);
								break;
								
							case FILLED:
								TheGame->DrawImage(sfcCircleRedFilled, item->X, item->Y, drawcol);
								break;
								
							case STRIPED:
								TheGame->DrawImage(sfcCircleRedStripy, item->X, item->Y, drawcol);
								break;
						}
						
						break;
				}
				
				break;
			case SQUARE:
				switch (item->Col) {
					case BLUE:
						switch (item->FillType) {
							case EMPTY:
								TheGame->DrawImage(sfcSquareBlueEmpty, item->X, item->Y, drawcol);
								break;
								
							case FILLED:
								TheGame->DrawImage(sfcSquareBlueFilled, item->X, item->Y, drawcol);
								break;
								
							case STRIPED:
								TheGame->DrawImage(sfcSquareBlueStripy, item->X, item->Y, drawcol);
								break;
						}
						break;
						
					case GREEN:
						switch (item->FillType) {
							case EMPTY:
								TheGame->DrawImage(sfcSquareGreenEmpty, item->X, item->Y, drawcol);
								break;
								
							case FILLED:
								TheGame->DrawImage(sfcSquareGreenFilled, item->X, item->Y, drawcol);
								break;
								
							case STRIPED:
								TheGame->DrawImage(sfcSquareGreenStripy, item->X, item->Y, drawcol);
								break;
						}
						break;
						
					case RED:
						switch (item->FillType) {
							case EMPTY:
								TheGame->DrawImage(sfcSquareRedEmpty, item->X, item->Y, drawcol);
								break;
								
							case FILLED:
								TheGame->DrawImage(sfcSquareRedFilled, item->X, item->Y, drawcol);
								break;
								
							case STRIPED:
								TheGame->DrawImage(sfcSquareRedStripy, item->X, item->Y, drawcol);
								break;
						}
						
						break;
				}
				
				break;
				
			case TRIANGLE:
				switch (item->Col) {
					case BLUE:
						switch (item->FillType) {
							case EMPTY:
								TheGame->DrawImage(sfcTriangleBlueEmpty, item->X, item->Y, drawcol);
								break;
								
							case FILLED:
								TheGame->DrawImage(sfcTriangleBlueFilled, item->X, item->Y, drawcol);
								break;
								
							case STRIPED:
								TheGame->DrawImage(sfcTriangleBlueStripy, item->X, item->Y, drawcol);
								break;
						}
						break;
						
					case GREEN:
						switch (item->FillType) {
							case EMPTY:
								TheGame->DrawImage(sfcTriangleGreenEmpty, item->X, item->Y, drawcol);
								break;
								
							case FILLED:
								TheGame->DrawImage(sfcTriangleGreenFilled, item->X, item->Y, drawcol);
								break;
								
							case STRIPED:
								TheGame->DrawImage(sfcTriangleGreenStripy, item->X, item->Y, drawcol);
								break;
						}
						break;
						
					case RED:
						switch (item->FillType) {
							case EMPTY:
								TheGame->DrawImage(sfcTriangleRedEmpty, item->X, item->Y, drawcol);
								break;
								
							case FILLED:
								TheGame->DrawImage(sfcTriangleRedFilled, item->X, item->Y, drawcol);
								break;
								
							case STRIPED:
								TheGame->DrawImage(sfcTriangleRedStripy, item->X, item->Y, drawcol);
								break;
						}
						
						break;
				}
				
				break;
		}
	}
	
	float scale = TheGame->SmoothStep2(1.0f, 1.2f, SelectedScale);
	Colour col = Colour(1.0f, TheGame->SmoothStep2(0.0f, 1.0f, SelectedScale), 1.0f, 1.0f);
	
	for (int i = 0; i < CurrentItems.Count; ++i) {
		BPMiniGame_SetFinder_Item* item = CurrentItems[i];
		
		if (item->Selected) {
			TheGame->DrawImage(sfcSelectedHi, item->X + sfcTriangleRedStripy->HalfWidth, item->Y + sfcTriangleRedStripy->HalfWidth, SelectedRotation, scale, col);
		}
	}
	

	int TimePassed = TheGame->TickCount - TimeStarted;
		
	if (sfcClock == NULL || RedrawClock()) {
		TheGame->AllocString(&sfcClock, TheGame->TicksToTime(TimePassed)->c_str(), XLARGE, 320, 49, CENTRED);
	}
	
	TheGame->DrawString(sfcClock, WHITE, 0, 364);
}

void BPMiniGame_SetFinder::Tick() {
	if (SuccessTime != 0 && SuccessTime + 500 < TheGame->TickCount) {
		Success();
	}
	
	CleanMatches();
}

void BPMiniGame_SetFinder::CleanMatches() {
	for (int i = CurrentItems.Count - 1; i >= 0; --i) {
		if (CurrentItems[i]->MatchTime != -1 && CurrentItems[i]->MatchTime + 150 < TheGame->TickCount) {
			CurrentItems.RemoveAt(i);
		}
	}
}

void BPMiniGame_SetFinder::OnMouseUp() {
	
}

void BPMiniGame_SetFinder::OnMouseMove() {
	
}

void BPMiniGame_SetFinder::OnMouseDown() {
	int numselected = 0;
	
	for (int i = 0; i < CurrentItems.Count; ++i) {
		BPMiniGame_SetFinder_Item* item = CurrentItems[i];
		
		if (item->Selected && item->MatchTime == -1) ++numselected;
	}
	
	for (int i = 0; i < CurrentItems.Count; ++i) {
		BPMiniGame_SetFinder_Item* item = CurrentItems[i];

		if (item->MatchTime != -1) continue;
		
		if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, item->X, item->Y, 45, 45)) {
			if (item->Selected) {
				item->Selected = false;
				TheGame->PlaySound("wrong");
				return;
			} else if (numselected < 3) {
				item->Selected = true;
				
				if (numselected < 2) {
					TheGame->PlaySound("card_flip");
				}
			} else {
				// already have three; bail out
				return;
			}
			
			CheckForMatch();
		}
	}
}

void BPMiniGame_SetFinder::CheckForMatch() {
	BPMiniGame_SetFinder_Item* selected1 = NULL;
	BPMiniGame_SetFinder_Item* selected2 = NULL;
	BPMiniGame_SetFinder_Item* selected3 = NULL;
	
	for (int i = 0; i < CurrentItems.Count; ++i) {
		BPMiniGame_SetFinder_Item* item = CurrentItems[i];
		
		if (item->Selected && item->MatchTime == -1) {
			if (selected1 == NULL) {
				selected1 = item;
			} else if (selected2 == NULL) {
				selected2 = item;
			} else if (selected3 == NULL) {
				selected3 = item;
				break;
			}
		}
	}
	
	if (selected3 != NULL) {
		// three items matched - are they all different?
		if (ItemsAreDifferent(selected1, selected2) && ItemsAreDifferent(selected1, selected3) && ItemsAreDifferent(selected2, selected3)) {
			selected1->MatchTime = TheGame->TickCount;
			selected2->MatchTime = TheGame->TickCount;
			selected3->MatchTime = TheGame->TickCount;
			
			selected1->Selected = false;
			selected2->Selected = false;
			selected3->Selected = false;
			
			++MatchCount;
			
			if (MatchCount >= 5 && SuccessTime == 0) {
				SuccessTime = TheGame->TickCount;
			}
			
			TheGame->PlaySound("card_flip2");
		} else {
			TheGame->PlaySound("card_flip");
		}
	}
}

bool BPMiniGame_SetFinder::ItemsAreDifferent(BPMiniGame_SetFinder_Item* item1, BPMiniGame_SetFinder_Item* item2) {
	if ((item1->Col != item2->Col) && (item1->ShapeType != item2->ShapeType) && (item1->FillType != item2->FillType)) return true;
	
	return false;
}
