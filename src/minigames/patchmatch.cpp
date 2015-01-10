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

#include "patchmatch.h"
#include "Minigame.h"

BPMiniGame_PatchMatch::BPMiniGame_PatchMatch(BPGame* game) : BPMiniGame(game) {
	sfcBackground = TheGame->LoadBitmap("patchmatch", 512, 512);
	
	Locked = false; // when true disallow movement
	TimeStarted = 0;
	SuccessTime = -1;
	
	sfcClock = sfcScoreStr = NULL;
	SelectedBox = NULL;
	
	DisappearTime = 400;
	
	Score = 0;
	SetScore();
	
	GameTitle = "Patch Match";
	GameHelp = "Tap on any shape, then drag it onto a square with the same picture to score points. It sounds easy enough, but can you stay focused for two minutes?";
	GameHelp2 = "Not every box will have a match on the screen at any given time, so you need to look hard and find matches wherever you can.";
	
	MiniGameType = PUZZLE;
	
	for (int i = 1; i <= 59; ++i) {
		ostringstream filename;
		filename << "patch" << i;
		PatchPictures.Add(TheGame->LoadBitmap(filename.str().c_str(), 48, 48));
	}
	
	for (int j = 0; j < 7; ++j) {
		vector<BPMiniGame_PatchMatch_Box*>* row = new vector<BPMiniGame_PatchMatch_Box*>();
		
		for (int i = 0; i < 6; ++i) {
			BPMiniGame_PatchMatch_Box* box = new BPMiniGame_PatchMatch_Box();
			box->X = 36 + (i * 50);
			box->Y = 36 + (j * 50);
			box->DestY = 36 + (j * 50);
			box->DrawX = box->X;
			box->DrawY = box->Y;
			box->Colour = TheGame->RandomRange(0, PatchPictures.Count - 1);
			row->push_back(box);
		}
		
		Boxes.push_back(row);
	}
}

BPMiniGame_PatchMatch::~BPMiniGame_PatchMatch() {
	SAFE_DELETE(sfcBackground);
	
	BPMiniGame_PatchMatch_Box* box;
	
	for (int i = 0; i < Boxes.size(); ++i) {
		vector<BPMiniGame_PatchMatch_Box*>* row = Boxes[i];
		
		for (int i = row->size() - 1; i >= 0; --i) {
			box = (*row)[i];
			SAFE_DELETE(box);
		}
		
		row->clear();
		SAFE_DELETE(row);
	}
	
	Boxes.clear();
	
	PatchPictures.Clear();
		
	SAFE_DELETE(sfcScoreStr);
	SAFE_DELETE(sfcClock);
}

void BPMiniGame_PatchMatch::OnMouseMove() {
	if (Locked) return;
	if (SelectedBox == NULL) return;
	
	Locked = true;
	SelectedBox->DrawX = TouchEvent.X;
	SelectedBox->DrawY = TouchEvent.Y;
	
	LastPos = TouchEvent;
	
	Locked = false;
}

void BPMiniGame_PatchMatch::OnMouseDown() {
	if (Locked) return;
	Locked = true;
	
	LastPos = TouchEvent;
	
	for (int i = 0; i < Boxes.size(); ++i) {
		vector<BPMiniGame_PatchMatch_Box*>* row = Boxes[i];
		
		for (int j = 0; j < row->size(); ++j) {
			BPMiniGame_PatchMatch_Box* box = (*row)[j];
			
			if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, box->X - HalfBoxSize, box->Y - HalfBoxSize, BoxSize, BoxSize)) {
				SelectedBox = (*row)[j];
				TheGame->PlaySound("card_flip");
				return;
			}
		}
	}
	
	Locked = false;
}

void BPMiniGame_PatchMatch::OnMouseUp() {
	if (SelectedBox == NULL) return;
	if (Locked) return;
	
	for (int i = 0; i < Boxes.size(); ++i) {
		vector<BPMiniGame_PatchMatch_Box*>* row = Boxes[i];
		
		for (int j = 0; j < row->size(); ++j) {
			BPMiniGame_PatchMatch_Box* box = (*row)[j];
			
			if (box == SelectedBox) continue;
			
			if (box->Colour != SelectedBox->Colour) continue;
			
			if (TheGame->RectOverRect(SelectedBox->DrawX, SelectedBox->DrawY, BoxSize, BoxSize, box->X, box->Y, BoxSize, BoxSize)) {
				box->MatchTime = TheGame->TickCount;
				SelectedBox->MatchTime = TheGame->TickCount;
				
				Score += 1;
				SetScore();
				
				TheGame->PlaySound("gem_select");
				
				SelectedBox = NULL;
				return;
			}
		}
	}
	
	SelectedBox->DrawX = SelectedBox->X;
	SelectedBox->DrawY = SelectedBox->Y;
	SelectedBox = NULL;
}

void BPMiniGame_PatchMatch::Start() {
	TimeStarted = TheGame->TickCount;
}

int BPMiniGame_PatchMatch::GetWeight() {
	return MinMax(round(Score * 17));
}

void BPMiniGame_PatchMatch::Render() {
	TheGame->Clear(TheGame->Black);
	
	int time_passed = TheGame->TickCount - TimeStarted;
	TheGame->DrawImage(sfcBackground, 160, 208, time_passed / 500.0f, 1.1f, (*TheGame->White));
	
	Colour col = Colour(0.0f, 0.0f, 0.0f, 0.6f);
	TheGame->FillRectangle(col, 0, 369, 320, 40);
	
	if (!MarathonMode) {
		int TimePassed = TheGame->TickCount - TimeStarted;
		TimePassed = 120000 - TimePassed;
		
		if (TimePassed <= 0) {
			if (SuccessTime == -1) {
				SuccessTime = TheGame->TickCount;
				Success();
			}
		}
		
		if (sfcClock == NULL || RedrawClock()) {
			TheGame->AllocString(&sfcClock, TheGame->TicksToTime(TimePassed)->c_str(), LARGE, 160, 50, RIGHT);
		}
		
		TheGame->DrawString(sfcClock, WHITE, 145, 370);
	}
	
	TheGame->DrawString(sfcScoreStr, WHITE, 14, 370);
	
	BPMiniGame_PatchMatch_Box* box;
	
	for (int i = 0; i < Boxes.size(); ++i) {
		vector<BPMiniGame_PatchMatch_Box*>* row = Boxes[i];
		
		for (int i = row->size() - 1; i >= 0; --i) {
			box = (*row)[i];
			
			if (box->MatchTime == -1) {
				TheGame->DrawImage(PatchPictures[box->Colour], box->DrawX, box->DrawY, 0.0f, 1.0f, (*TheGame->White));
			}
		}
	}
	
	for (int i = 0; i < Boxes.size(); ++i) {
		vector<BPMiniGame_PatchMatch_Box*>* row = Boxes[i];
		
		for (int i = row->size() - 1; i >= 0; --i) {
			box = (*row)[i];
			
			if (box->MatchTime != -1) {
				float diff = TheGame->TickCount - box->MatchTime;
				
				if (diff <= DisappearTime) {
					float step = diff / DisappearTime; // get a value between 0 and 1
					Colour col = Colour(1.0f, 1.0f, 1.0f, 1 - step);
					
					TheGame->DrawImage(PatchPictures[box->Colour], box->DrawX, box->DrawY, TheGame->Lerp(0, 360, step), TheGame->SmoothStep(1.0f, 6.0f, step), col);
				}
			}
		}
	}
	
	if (SelectedBox != NULL) {
		TheGame->DrawImage(PatchPictures[SelectedBox->Colour], SelectedBox->DrawX, SelectedBox->DrawY, 0.0f, 1.0f, (*TheGame->White));
	}
}

void BPMiniGame_PatchMatch::Tick() {
	vector<BPMiniGame_PatchMatch_Box*>* row;
	BPMiniGame_PatchMatch_Box* box;
	BPMiniGame_PatchMatch_Box* copybox;
	
	int MatchTimeout = TheGame->TickCount - 400;
	
	bool AllStill = true;
	
	for (int i = Boxes.size() - 1; i >= 0; --i) {
		row = Boxes[i];
		
		for (int j = row->size() - 1; j >= 0; --j) {
			box = (*row)[j];
			
			if (box->MatchTime != -1) AllStill = false;
			
			if (box->Y != box->DestY) {
				AllStill = false;
				
				box->YSpeed += 2.0f;
				box->Y += box->YSpeed;
				if (box->Y > box->DestY) {
					box->Y = box->DestY;
					box->YSpeed = 0;
				}
				
				box->DrawY = box->Y;
			}
			
			if (box->MatchTime != -1 && box->MatchTime < MatchTimeout) {
				SAFE_DELETE(box);
				
				row->erase(row->begin() + j);
				// move all boxes down above me
				
				for (int k = i - 1; k >= 0; --k) {
					copybox = (*Boxes[k])[j];
					Boxes[k]->erase(Boxes[k]->begin() + j);
					Boxes[k + 1]->insert(Boxes[k + 1]->begin() + j, copybox);
					copybox->DestY = 36 + ((k + 1) * BoxSize);
				}
				
				BPMiniGame_PatchMatch_Box* newbox = new BPMiniGame_PatchMatch_Box();
				newbox->X = 36 + (j * BoxSize);
				newbox->Y = -((9 - j) * BoxSize);
				newbox->DrawX = newbox->X;
				newbox->DrawY = newbox->Y;
				newbox->DestY = 36;
				newbox->Colour = TheGame->RandomRange(0, PatchPictures.Count - 1);
				Boxes[0]->insert(Boxes[0]->begin() + j, newbox);
			}
		}
	}
	
	if (AllStill) {
		Locked = false;
	} else {
		Locked = true;
	}
}

void BPMiniGame_PatchMatch::SetMarathon() {
	MarathonMode = true;
	GameHelp = "Drag rows of jewels left and right, matching three or more to score points; the more you match, the more you score.";
}

void BPMiniGame_PatchMatch::SetScore() {
	if (Score > 0) {
		ostringstream score;
		string scorestr = TheGame->SeparateThousands(Score);
		score << "Matches: " << scorestr;
		TheGame->AllocString(&sfcScoreStr, score.str().c_str(), LARGE, 270, 50, LEFT);
	} else {
		TheGame->AllocString(&sfcScoreStr, "Matches: 0", LARGE, 270, 50, LEFT);
	}
}
