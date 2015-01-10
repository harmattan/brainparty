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

#include "Minigame.h"
#include "balloonblaster.h"

BPMiniGame_BalloonBlaster::BPMiniGame_BalloonBlaster(BPGame* game) : BPMiniGame(game) {
	sfcBackground = TheGame->LoadBitmap("clouds", 320, 480);
	
	Balloons = new vector<vector<BPMiniGame_BalloonBlaster_Balloon*>* >();
	BalloonTypes = new BPPList<Texture*>();
	sfcBalloonBlack = TheGame->LoadBitmap("balloon_black", 32, 45);
	sfcPuff = TheGame->LoadBitmap("puff", 61, 61);
	
	TimeStarted = 0;
	TimePassed = 0;
	SmartBombTime = -1;
	SuccessTime = -1;
	
	DisappearTime = 200;
	
	Score = 0;
	sfcScoreStr = sfcClock = NULL;
	SetScore();
	
	GameTitle = "Balloon Blaster";
	GameHelp = "Pop balloons by tapping them - get three or more of the same colour to score. But watch out: pop one and you lose straight away, pop two and you lose lots of points. What's more, you have only two minutes!";
	GameHelp2 = "The key to scoring high is to get as many similar-coloured balloons together as possible before popping them - even if that means popping one or two smaller groups first. Remember: if you're going to score lots of points then losing a few thousand first is a good investment!";
	
	MiniGameType = LIVELY;
		
	BalloonTypes->Add(TheGame->LoadBitmap("balloon_blue", 32, 45));
	BalloonTypes->Add(TheGame->LoadBitmap("balloon_red", 32, 45));
	BalloonTypes->Add(TheGame->LoadBitmap("balloon_purple", 32, 45));
	BalloonTypes->Add(TheGame->LoadBitmap("balloon_green", 32, 45));
	
	for (int j = 0; j < 7; ++j) {
		vector<BPMiniGame_BalloonBlaster_Balloon*>* row = new vector<BPMiniGame_BalloonBlaster_Balloon*>();
		
		for (int i = 0; i < 9; ++i) {
			BPMiniGame_BalloonBlaster_Balloon* balloon = new BPMiniGame_BalloonBlaster_Balloon();
			balloon->X = 4 + (i * 35);
			balloon->Y = MiniGameHeight + (j * 50);
			balloon->DestY = 10 + (j * 50);
			
			balloon->Colour = TheGame->RandomRange(0, BalloonTypes->Count - 1);
			row->push_back(balloon);
		}
		
		Balloons->push_back(row);
	}
}

BPMiniGame_BalloonBlaster::~BPMiniGame_BalloonBlaster() {
	SAFE_DELETE(sfcBackground);
	SAFE_DELETE(sfcBalloonBlack);
	SAFE_DELETE(sfcPuff);
		
	vector<BPMiniGame_BalloonBlaster_Balloon*>* row;
	BPMiniGame_BalloonBlaster_Balloon* balloon;
	
	for (int i = 0; i < Balloons->size(); ++i) {
		row = (*Balloons)[i];
		
		for (int j = 0; j < row->size(); ++j) {
			balloon = (*row)[j];
			SAFE_DELETE(balloon);
		}
		
		row->clear();
		SAFE_DELETE(row);
	};
	
	Balloons->clear();
	SAFE_DELETE(Balloons);
	
	SAFE_DELETE(sfcScoreStr);
	SAFE_DELETE(sfcClock);
	
	BalloonTypes->Clear();
	SAFE_DELETE(BalloonTypes);
}

void BPMiniGame_BalloonBlaster::OnMouseUp() {
	
}

void BPMiniGame_BalloonBlaster::OnMouseMove() {
	
}
	
void BPMiniGame_BalloonBlaster::OnMouseDown() {
	vector<BPMiniGame_BalloonBlaster_Balloon*>* row;
	BPMiniGame_BalloonBlaster_Balloon* balloon;
		
	for (int i = 0; i < Balloons->size(); ++i) {
		row = (*Balloons)[i];
			
		for (int j = 0; j < row->size(); ++j) {
			balloon = (*row)[j];
				
			if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, balloon->X, balloon->Y, 32, 45)) {
				if (balloon->Colour == -1) {
					TheGame->PlaySound("explosion");
					int ScoreAdd = SmartBomb(balloon, i, j);
					ScoreAdd = floor(pow(2.0f, min(16, ScoreAdd)));
					ModifyScore(ScoreAdd);
				} else {
					int ScoreAdd = MatchBalloon(balloon, i, j);
						
					switch (ScoreAdd) {
						case 0:
							break;
						case 1:
							Failure();
							break;
						case 2:
							TheGame->PlaySound("balloon_pop");
							ModifyScore(-100);
							break;
						default:
							TheGame->PlaySound("balloon_pop");
							ModifyScore(floor(pow(2.0f, min(16, ScoreAdd))));
							break;
					}
				}
				
				return;
			}
		}
	}
}
	
int BPMiniGame_BalloonBlaster::SmartBomb(BPMiniGame_BalloonBlaster_Balloon* balloon, int row, int col) {
	// this matches all adjacent balloons
	int ThisScore = 0;
		
	if (balloon->MatchTime == -1) {
		balloon->MatchTime = TheGame->TickCount;
			
		if (row > 0) {
			if (col > 0) ThisScore += MatchBalloon((*(*Balloons)[row - 1])[col - 1], row - 1, col - 1);
			ThisScore += MatchBalloon((*(*Balloons)[row - 1])[col], row - 1, col);
			if (col < 8) ThisScore += MatchBalloon((*(*Balloons)[row - 1])[col + 1], row - 1, col + 1);
		}
			
		if (col > 0) ThisScore += MatchBalloon((*(*Balloons)[row])[col - 1], row, col - 1);
		if (col < 8) ThisScore += MatchBalloon((*(*Balloons)[row])[col + 1], row, col + 1);
			
		if (row < 5) {
			if (col > 0) ThisScore += MatchBalloon((*(*Balloons)[row + 1])[col - 1], row + 1, col - 1);
			ThisScore += MatchBalloon((*(*Balloons)[row + 1])[col], row + 1, col);
			if (col < 8) ThisScore += MatchBalloon((*(*Balloons)[row + 1])[col + 1], row + 1, col + 1);
		}
	}
		
	// used to give a quick white flash on the screen
	SmartBombTime = TheGame->TickCount + 100;
		
	return ThisScore;
}
	
void BPMiniGame_BalloonBlaster::ModifyScore(int adjust) {
	Score += adjust;
		
	if (Score < 0) Score = 0;
	
	SetScore();
}
	
void BPMiniGame_BalloonBlaster::Start() {
	TimeStarted = TheGame->TickCount;
}
	
int BPMiniGame_BalloonBlaster::GetWeight() {
	return MinMax(Score / 490);
}
	
void BPMiniGame_BalloonBlaster::Render() {
	if (SmartBombTime > TheGame->TickCount) {
		// give a quick white flash for the smart bomb
		TheGame->Clear(TheGame->White);
		return;
	}
		
	TheGame->DrawImage(sfcBackground, 0, 0);
		
	BPMiniGame_BalloonBlaster_Balloon* balloon;
		
	for (int b = 0; b < Balloons->size(); ++b) {
		vector<BPMiniGame_BalloonBlaster_Balloon*>* row = (*Balloons)[b];
		
		for (int i = row->size() - 1; i >= 0; --i) {
			balloon = (*row)[i];
								
			if (balloon->MatchTime != -1) {
				float diff = TheGame->TickCount - balloon->MatchTime;
				
				if (diff <= DisappearTime) {
					float step = diff / DisappearTime; // get a value between 0 and 1
					Colour col = Colour(1.0f, 1.0f, 1.0f, 1 - step);

					if (balloon->Colour == -1) {
						TheGame->DrawImage(sfcBalloonBlack, balloon->X + sfcBalloonBlack->HalfWidth, balloon->Y + sfcBalloonBlack->HalfHeight, 0.0f, 1.0f + step, col);
					} else {
						TheGame->DrawImage((*BalloonTypes)[balloon->Colour], balloon->X + sfcBalloonBlack->HalfWidth, balloon->Y + sfcBalloonBlack->HalfHeight, 0.0f, 1.0f + step, col);
					}			
					
					TheGame->DrawImage(sfcPuff, balloon->X + sfcBalloonBlack->HalfWidth, balloon->Y + sfcBalloonBlack->HalfHeight, 0.0f, 1.0f, col);
				}
			} else {
				if (balloon->Colour == -1) {
					TheGame->DrawImage(sfcBalloonBlack, balloon->X +  + sfcBalloonBlack->HalfWidth, balloon->Y + sfcBalloonBlack->HalfHeight, 0.0f, 1.0f, (*TheGame->White));
				} else {
					TheGame->DrawImage((*BalloonTypes)[balloon->Colour], balloon->X +  sfcBalloonBlack->HalfWidth, balloon->Y + sfcBalloonBlack->HalfHeight, 0.0f, 1.0f, (*TheGame->White));
				}				
			}
		}
	}
	
	if (!MarathonMode) {
		TimePassed = TheGame->TickCount - TimeStarted;
		TimePassed = 120000 - TimePassed;
			
		if (TimePassed <= 0) {
			if (SuccessTime == -1) {
				SuccessTime = TheGame->TickCount;
				Success();
			}
		}
		
		if (sfcClock == NULL || RedrawClock()) {
			TheGame->AllocString(&sfcClock, TheGame->TicksToTime(TimePassed)->c_str(), LARGE, 80, 47, LEFT);
		}
		
		TheGame->DrawString(sfcClock, BLACK, 235, 366);
	}

	TheGame->DrawString(sfcScoreStr, BLACK, 15, 367);
}
	
void BPMiniGame_BalloonBlaster::Tick() {
	vector<BPMiniGame_BalloonBlaster_Balloon*>* row;
	BPMiniGame_BalloonBlaster_Balloon* balloon;
	BPMiniGame_BalloonBlaster_Balloon* copyballoon;
			
	int MatchTimeout = TheGame->TickCount - DisappearTime;
			
	for (int i = 0; i < Balloons->size(); ++i) {
		row = (*Balloons)[i];
				
		for (int j = row->size() - 1; j >= 0; --j) {
			balloon = (*row)[j];
			
			if (balloon->Y != balloon->DestY) {
				++balloon->YSpeed;
				balloon->Y -= balloon->YSpeed;
				if (balloon->Y < balloon->DestY) {
					balloon->Y = balloon->DestY;
					balloon->YSpeed = 0;
				}
			}
					
			if (balloon->MatchTime != -1 && balloon->MatchTime < MatchTimeout) {
				SAFE_DELETE((*row)[j]);
				
				row->erase(row->begin() + j);
				
				// move all balloons up below me
				
				for (int k = i + 1; k < Balloons->size(); ++k) {
					copyballoon = (*(*Balloons)[k])[j];
					(*Balloons)[k]->erase((*Balloons)[k]->begin() + j);
										
					(*Balloons)[k - 1]->insert((*Balloons)[k - 1]->begin() + j, copyballoon);
					copyballoon->DestY = 10 + ((k - 1) * 50);
				}
						
				BPMiniGame_BalloonBlaster_Balloon* newballoon = new BPMiniGame_BalloonBlaster_Balloon();
				newballoon->X = 4 + (j * 35);
				newballoon->Y = MiniGameHeight;
				newballoon->DestY = 310;
						
				if (TheGame->RandomRange(0, 100) == 56) {
					newballoon->Colour = -1;
				} else {
					newballoon->Colour = TheGame->RandomRange(0, BalloonTypes->Count - 1);
				}
						
				(*Balloons)[Balloons->size() - 1]->insert((*Balloons)[Balloons->size() - 1]->begin() + j, newballoon);
			}
		}
	}
}
	
int BPMiniGame_BalloonBlaster::MatchBalloon(BPMiniGame_BalloonBlaster_Balloon* balloon, int row, int col) {
	int ThisScore = 0;
		
	if (balloon->MatchTime == -1) {
		balloon->MatchTime = TheGame->TickCount;
			
		ThisScore = 1;
			
		// now match all other identical balloons around it
		if (row > 0) {
			if ((*(*Balloons)[row - 1])[col]->Colour == balloon->Colour && (*(*Balloons)[row - 1])[col]->MatchTime == -1) {
				ThisScore += MatchBalloon((*(*Balloons)[row - 1])[col], row - 1, col);
			}
		}
	
		if (row < 6) {
			if ((*(*Balloons)[row + 1])[col]->Colour == balloon->Colour && (*(*Balloons)[row + 1])[col]->MatchTime == -1) {
				ThisScore += MatchBalloon((*(*Balloons)[row + 1])[col], row + 1, col);
			}
		}
		
		if (col > 0) {
			if ((*(*Balloons)[row])[col - 1]->Colour == balloon->Colour && (*(*Balloons)[row])[col - 1]->MatchTime == -1) {
				ThisScore += MatchBalloon((*(*Balloons)[row])[col - 1], row, col - 1);
			}
		}
			
		if (col < 8) {
			if ((*(*Balloons)[row])[col + 1]->Colour == balloon->Colour && (*(*Balloons)[row])[col + 1]->MatchTime == -1) {
				ThisScore += MatchBalloon((*(*Balloons)[row])[col + 1], row, col + 1);
			}
		}
	}
		
	return ThisScore;
}
	
void BPMiniGame_BalloonBlaster::SetMarathon() {
	MarathonMode = true;
	GameHelp = "Pop balloons by tapping them - get three or more of the same colour to score. But watch out: popping one or two is bad!";
}

void BPMiniGame_BalloonBlaster::SetScore() {
	if (Score > 0) {
		ostringstream str;
		string score_str = TheGame->SeparateThousands(Score);
		str << "Score: " << score_str;
		TheGame->AllocString(&sfcScoreStr, str.str().c_str(), LARGE, 250, 50, LEFT);
	} else {
		TheGame->AllocString(&sfcScoreStr, "Score: 0", LARGE, 250, 50, LEFT);
	}
}
