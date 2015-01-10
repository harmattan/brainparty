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

#include "jeweljam.h"
#include "Minigame.h"
	
BPMiniGame_JewelJam::BPMiniGame_JewelJam(BPGame* game) : BPMiniGame(game) {
	sfcBackground = TheGame->LoadBitmap("jeweljam", 320, 416);
	
	SelectedRow = NULL;
	Locked = false; // when true disallow movement
	TimeStarted = 0;
	SuccessTime = -1;
	
	sfcClock = sfcScoreStr = NULL;
	
	DisappearTime = 350;
	
	Score = 0;
	SetScore();
		
	GameTitle = "Jewel Jam";
	GameHelp = "Drag the rows of gems left and right, matching three or more of the same type scores points and more gems will fall. The more you match in one move, the more you score. But hurry - you have only two minutes!";
	GameHelp2 = "The key in this game is that you can only slide left and right, so you need to start looking for vertical columns of gems to match. Generally it's best to work near the bottom of the screen, because that way falling gems are more likely to have a chain reaction and earn you more points.";
	
	MiniGameType = PUZZLE;
	
	ColoursLo.Add(TheGame->LoadBitmap("gem1_40", 40, 40));
	ColoursLo.Add(TheGame->LoadBitmap("gem2_40", 40, 40));
	ColoursLo.Add(TheGame->LoadBitmap("gem3_40", 40, 40));
	ColoursLo.Add(TheGame->LoadBitmap("gem4_40", 40, 40));
	ColoursLo.Add(TheGame->LoadBitmap("gem5_40", 40, 40));
	ColoursLo.Add(TheGame->LoadBitmap("gem6_40", 40, 40));
	ColoursLo.Add(TheGame->LoadBitmap("gem7_40", 40, 40));
	ColoursLo.Add(TheGame->LoadBitmap("gem8_40", 40, 40));
	
	ColoursHi.Add(TheGame->LoadBitmap("gem1_hi_40", 40, 40));
	ColoursHi.Add(TheGame->LoadBitmap("gem2_hi_40", 40, 40));
	ColoursHi.Add(TheGame->LoadBitmap("gem3_hi_40", 40, 40));
	ColoursHi.Add(TheGame->LoadBitmap("gem4_hi_40", 40, 40));
	ColoursHi.Add(TheGame->LoadBitmap("gem5_hi_40", 40, 40));
	ColoursHi.Add(TheGame->LoadBitmap("gem6_hi_40", 40, 40));
	ColoursHi.Add(TheGame->LoadBitmap("gem7_hi_40", 40, 40));
	ColoursHi.Add(TheGame->LoadBitmap("gem8_hi_40", 40, 40));
	
	for (int j = 0; j < 9; ++j) {
		vector<BPMiniGame_JewelJam_Box*>* row = new vector<BPMiniGame_JewelJam_Box*>();
		
		for (int i = 0; i < 8; ++i) {
			BPMiniGame_JewelJam_Box* box = new BPMiniGame_JewelJam_Box();
			box->X = i * BoxSize;
			box->Y = -((9 - j) * BoxSize * i);
			box->DestY = 77 + (j * BoxSize);
			box->Colour = TheGame->RandomRange(0, ColoursLo.Count - 1);
			row->push_back(box);
		}
		
		Boxes.push_back(row);
	}
}

BPMiniGame_JewelJam::~BPMiniGame_JewelJam() {
	SAFE_DELETE(sfcBackground);
	
	BPMiniGame_JewelJam_Box* box;
	
	for (int i = 0; i < Boxes.size(); ++i) {
		vector<BPMiniGame_JewelJam_Box*>* row = Boxes[i];
		
		for (int i = row->size() - 1; i >= 0; --i) {
			box = (*row)[i];
			SAFE_DELETE(box);
		}
		
		row->clear();
		SAFE_DELETE(row);
	}
	
	Boxes.clear();
	
	ColoursLo.Clear();
	ColoursHi.Clear();
	
	if (SelectedRow != NULL) {
		// shouldn't have to do this, because it's just pointers to things that have already been deleted
		//SelectedRow->clear();
		SAFE_DELETE(SelectedRow);
	}
	
	SAFE_DELETE(sfcScoreStr);
	SAFE_DELETE(sfcClock);
}

void BPMiniGame_JewelJam::OnMouseMove() {
	if (Locked) return;
	if (SelectedRow == NULL) return;
	
	Locked = true;
	
	BPMiniGame_JewelJam_Box* box;
	
	if (LastPos != TheGame->EmptyPoint && LastPos != TouchEvent) {
		if (TouchEvent.X < LastPos.X) {
			// move left
			
			for (int i = SelectedRow->size() - 1; i >= 0; --i) {
				box = (*SelectedRow)[i];
				
				box->X += TouchEvent.X - LastPos.X;
				
				if (box->X <= -BoxSize) {
					box->X = MiniGameWidth + box->X;
					SelectedRow->erase(SelectedRow->begin() + i);
					SelectedRow->push_back(box);
				}
			}
			
		} else {
			// move right
			
			for (int i = 0; i < SelectedRow->size(); ++i) {
				box = (*SelectedRow)[i];
				
				box->X += TouchEvent.X - LastPos.X;
				
				if (box->X >= MiniGameWidth) {
					box->X -= MiniGameWidth;
					SelectedRow->erase(SelectedRow->begin() + i);
					SelectedRow->insert(SelectedRow->begin() + 0, box);
				}
			}
		}
	}
	
	LastPos = TouchEvent;
	
	Locked = false;
}

void BPMiniGame_JewelJam::OnMouseDown() {
	if (Locked) return;
	Locked = true;
	
	LastPos = TouchEvent;
	
	for (int i = 0; i < Boxes.size(); ++i) {
		if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 0, 57 + (i * BoxSize), 320, BoxSize)) {
			// move this row
			SelectedRow = Boxes[i];
			break;
		}
	}
	
	Locked = false;
}

void BPMiniGame_JewelJam::OnMouseUp() {
	if (Locked) return;
	if (SelectedRow == NULL) return;
	
	BPMiniGame_JewelJam_Box* box;
	
	Locked = true;
	
	// we've let go of a row - snap it into place!
	int xoffset = (*SelectedRow)[0]->X;
	
	int remainder = xoffset % BoxSize;
	if (remainder != 0) {
		int adjust = 0;
		
		if (remainder > HalfBoxSize || (remainder < 0 && remainder > -HalfBoxSize)) {
			// snap to the right
			if (remainder > 0) {
				adjust = BoxSize - remainder;
			} else {
				adjust = abs(remainder);
			}
			
			for (int i = 0; i < SelectedRow->size(); ++i) {
				box = (*SelectedRow)[i];
				
				box->X += adjust;
				
				if (box->X >= MiniGameWidth) {
					box->X -= MiniGameWidth;
					SelectedRow->erase(SelectedRow->begin() + i);
					SelectedRow->insert(SelectedRow->begin() + 0, box);
				}
			}
		} else {
			// snap to the left
			if (remainder < 0) {
				adjust = abs(remainder) - BoxSize;
			} else {
				adjust = -remainder;
			}
			
			for (int i = SelectedRow->size() - 1; i >= 0; --i) {
				box = (*SelectedRow)[i];
				
				box->X += adjust;
				
				if (box->X <= -BoxSize) {
					box->X = MiniGameWidth + box->X;
					SelectedRow->erase(SelectedRow->begin() + i);
					SelectedRow->push_back(box);
				}
			}
			
		}
	}
	
	SelectedRow = NULL;
	
	CheckForMatches();
	
	// subtract one point for each move made
	if (Score > 0) {
		--Score;
		SetScore();
	}
}

void BPMiniGame_JewelJam::Start() {
	TimeStarted = TheGame->TickCount;
}

int BPMiniGame_JewelJam::GetWeight() {
	return MinMax(round(Score / 6));
}

void BPMiniGame_JewelJam::Render() {
	TheGame->DrawImage(sfcBackground, 0, 0);
	
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
		
		TheGame->DrawString(sfcClock, WHITE, 150, 4);
	}
	
	TheGame->DrawString(sfcScoreStr, WHITE, 6, 4);
	
	BPMiniGame_JewelJam_Box* box;
	
	for (int i = 0; i < Boxes.size(); ++i) {
		vector<BPMiniGame_JewelJam_Box*>* row = Boxes[i];

		for (int i = row->size() - 1; i >= 0; --i) {
			box = (*row)[i];
			
			// HACK: There's an unknown bug in this minigame that occasionally puts boxes in the wrong X pos.
			// To reproduce, just keep matching stuff while dragging around on the screen manically.
			// This force-resets the X position for each box to the correct value. What a waste of CPU time!
			if (SelectedRow != row) box->X = i * BoxSize;
			
			if (box->MatchTime == -1) {
				TheGame->DrawImage(ColoursLo[box->Colour], box->X + HalfBoxSize, box->Y, 0.0f, 1.0f, (*TheGame->White));
				
				if (box->X < 0) {
					TheGame->DrawImage(ColoursLo[box->Colour], MiniGameWidth + box->X + HalfBoxSize, box->Y, 0.0f, 1.0f, (*TheGame->White));
				} else if (box->X > MiniGameWidth - BoxSize) {
					TheGame->DrawImage(ColoursLo[box->Colour],  HalfBoxSize + box->X - MiniGameWidth, box->Y, 0.0f, 1.0f, (*TheGame->White));
				}
			} else {
				float diff = TheGame->TickCount - box->MatchTime;
				
				if (diff <= DisappearTime) {
					float step = diff / DisappearTime; // get a value between 0 and 1
					Colour col = Colour(1.0f, 1.0f, 1.0f, 1 - step);
					
					TheGame->DrawImage(ColoursHi[box->Colour], box->X + HalfBoxSize, box->Y, 0.0f, TheGame->SmoothStep(1.0f, 3.0f, step), col);
					
					if (box->X < 0) {
						TheGame->DrawImage(ColoursHi[box->Colour], MiniGameWidth + box->X + HalfBoxSize, box->Y, 0.0f, TheGame->SmoothStep(1.0f, 3.0f, step), (*TheGame->White));
					} else if (box->X > MiniGameWidth - BoxSize) {
						TheGame->DrawImage(ColoursHi[box->Colour],  HalfBoxSize + box->X - MiniGameWidth, box->Y, 0.0f, TheGame->SmoothStep(1.0f, 3.0f, step), (*TheGame->White));
					}
				}
			}
		}
	}
}

void BPMiniGame_JewelJam::Tick() {
	vector<BPMiniGame_JewelJam_Box*>* row;
	BPMiniGame_JewelJam_Box* box;
	BPMiniGame_JewelJam_Box* copybox;
	
	int MatchTimeout = TheGame->TickCount - 150;
	
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
			}
			
			if (box->MatchTime != -1 && box->MatchTime < MatchTimeout) {
				SAFE_DELETE(box);
				
				row->erase(row->begin() + j);
				// move all boxes down above me
				
				for (int k = i - 1; k >= 0; --k) {
					copybox = (*Boxes[k])[j];
					Boxes[k]->erase(Boxes[k]->begin() + j);
					Boxes[k + 1]->insert(Boxes[k + 1]->begin() + j, copybox);
					copybox->DestY = 77 + ((k + 1) * BoxSize);
				}
				
				BPMiniGame_JewelJam_Box* newbox = new BPMiniGame_JewelJam_Box();
				newbox->X = j * BoxSize;
				newbox->Y = -((9 - j) * BoxSize);
				newbox->DestY = 77;
				newbox->Colour = TheGame->RandomRange(0, ColoursLo.Count - 1);
				Boxes[0]->insert(Boxes[0]->begin() + j, newbox);
			}
		}
	}
	
	if (AllStill) {
		if (Locked) {
			CheckForMatches();
		}
	} else {
		Locked = true;
	}
}

void BPMiniGame_JewelJam::CheckForMatches() {
	Locked = true;
	
	int ThisScore = 0;
	
	// if any three boxes are in a line, match them and any others touching them of the same colour
	bool got_match = false;
	
	vector<BPMiniGame_JewelJam_Box*>* row;
	BPMiniGame_JewelJam_Box* box;
	
	for (int i = Boxes.size() - 1; i >= 0; --i) {
		row = Boxes[i];
		
		for (int j = row->size() - 1; j >= 0; --j) {
			// does this box match?
			box = (*row)[j];
			
			// check two boxes to the left
			if (j > 1) {
				if ((*row)[j - 1]->Colour == box->Colour && (*row)[j - 2]->Colour == box->Colour) {
					got_match = true;
					ThisScore += MatchBox(box, i, j);
					continue;
				}
			}
			
			if (j > 0 && j < 7) {
				// try matching one to the left and one to the right
				if ((*row)[j - 1]->Colour == box->Colour && (*row)[j + 1]->Colour == box->Colour) {
					got_match = true;
					ThisScore += MatchBox(box, i, j);
					continue;
				}
			}
			
			// check two boxes to the right
			if (j < 6) {
				if ((*row)[j + 1]->Colour == box->Colour && (*row)[j + 2]->Colour == box->Colour) {
					got_match = true;
					ThisScore += MatchBox(box, i, j);
					continue;
				}
			}
			
			
			// check two boxes above
			if (i > 1) {
				if ((*Boxes[i - 1])[j]->Colour == box->Colour && (*Boxes[i - 2])[j]->Colour == box->Colour) {
					got_match = true;
					ThisScore += MatchBox(box, i, j);
					continue;
				}
			}
			
			// try matching one box above and one box below
			if (i > 0 && i < 7) {
				if ((*Boxes[i - 1])[j]->Colour == box->Colour && (*Boxes[i + 1])[j]->Colour == box->Colour) {
					got_match = true;
					ThisScore += MatchBox(box, i, j);
					continue;
				}
			}
			
			// check two boxes below
			if (i < 6) {
				if ((*Boxes[i + 1])[j]->Colour == box->Colour && (*Boxes[i + 2])[j]->Colour == box->Colour) {
					got_match = true;
					ThisScore += MatchBox(box, i, j);
					continue;
				}
			}
			
		}
	}
	
	if (ThisScore != 0) {
		Score += min(512, (int)round(pow(2.0f, ThisScore)));
		SetScore();
	}
	
	Locked = got_match;
	
	if (got_match) {
		TheGame->PlaySound("gem_select");
	}
}

int BPMiniGame_JewelJam::MatchBox(BPMiniGame_JewelJam_Box* box, int row, int col) {
	if (box->MatchTime == -1) {
		box->MatchTime = TheGame->TickCount;
		
		int thisscore = 1;
		
		// now match all other identical boxes around it
		if (row > 0) {
			if ((*Boxes[row - 1])[col]->Colour == box->Colour && (*Boxes[row - 1])[col]->MatchTime == -1) {
				thisscore += MatchBox((*Boxes[row - 1])[col], row - 1, col);
			}
		}
		
		if (row < 7) {
			if ((*Boxes[row + 1])[col]->Colour == box->Colour && (*Boxes[row + 1])[col]->MatchTime == -1) {
				thisscore += MatchBox((*Boxes[row + 1])[col], row + 1, col);
			}
		}
		
		if (col > 0) {
			if ((*Boxes[row])[col - 1]->Colour == box->Colour && (*Boxes[row])[col - 1]->MatchTime == -1) {
				thisscore += MatchBox((*Boxes[row])[col - 1], row, col - 1);
			}
		}
		
		if (col < 7) {
			if ((*Boxes[row])[col + 1]->Colour == box->Colour && (*Boxes[row])[col + 1]->MatchTime == -1) {
				thisscore += MatchBox((*Boxes[row])[col + 1], row, col + 1);
			}
		}
		
		return thisscore;
	} else {
		return 0;
	}
}

void BPMiniGame_JewelJam::SetMarathon() {
	MarathonMode = true;
	GameHelp = "Drag rows of jewels left and right, matching three or more to score points; the more you match, the more you score.";
}

void BPMiniGame_JewelJam::SetScore() {
	if (Score > 0) {
		ostringstream score;
		string scorestr = TheGame->SeparateThousands(Score);
		score << "Score: " << scorestr;
		TheGame->AllocString(&sfcScoreStr, score.str().c_str(), LARGE, 270, 50, LEFT);
	} else {
		TheGame->AllocString(&sfcScoreStr, "Score: 0", LARGE, 270, 50, LEFT);
	}
}
