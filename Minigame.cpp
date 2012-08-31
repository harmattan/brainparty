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
		
BPMiniGame::BPMiniGame(BPGame* game) {
	TheGame = game;
	
	ReturnType = RT_NORMAL;
	
	FinishedTime = FinalWeight = 0;
	
	FinalGrade = NULL;
	
	sfcGameTitle = NULL;
	sfcGameHelp = NULL;
	sfcFinalWeight = NULL;
	sfcFinalGrade = NULL;
	
	MarathonMode = false;
	
	WantToQuit = -1;
}

void BPMiniGame::Init() {
	TheGame->AllocString(&sfcGameTitle, GameTitle, LARGE, 276, 46, LEFT, true);
	TheGame->AllocString(&sfcGameHelp, GameHelp, XSMALL, 276, 170, LEFT);
}

BPMiniGame::~BPMiniGame() {
//	SAFE_DELETE(GameTitle);
//	SAFE_DELETE(sfcGameTitle);
//	SAFE_DELETE(GameHelp);
//	SAFE_DELETE(sfcGameHelp);
//	SAFE_DELETE(GameHelp2);
//	SAFE_DELETE(FinalGrade);
	
//	SAFE_DELETE(sfcFinalWeight);
//	SAFE_DELETE(sfcFinalGrade);
}
		
RankTypes BPMiniGame::GetRank(int Weight) {
	if (Weight < 50) {
		return FAIL;
	} else if (Weight < 150) {
		return BRONZE;
	} else if (Weight < 350) {
		return SILVER;
	} else if (Weight < 450) {
		return GOLD;
	} else {
		return PLATINUM;
	}
}
		
const char* BPMiniGame::GetGrade(int Weight) {
	if (Weight < 50) {
		return "F";
	} else if (Weight < 50) {
		return "D-";
	} else if (Weight < 100) {
		return "D";
	} else if (Weight < 125) {
		return "D+";
	} else if (Weight < 150) {
		return "D++";
	} else if (Weight < 175) {
		return "C-";
	} else if (Weight < 200) {
		return "C";
	} else if (Weight < 225) {
		return "C+";
	} else if (Weight < 250) {
		return "C++";
	} else if (Weight < 275) {
		return "B-";
	} else if (Weight < 300) {
		return "B";
	} else if (Weight < 325) {
		return "B+";
	} else if (Weight < 350) {
		return "B++";
	} else if (Weight < 375) {
		return "A-";
	} else if (Weight < 400) {
		return "A";
	} else if (Weight < 425) {
		return "A+";
	} else if (Weight < 450) {
		return "A++";
	} else {
		return "A*";
	}
}
		
void BPMiniGame::TickMiniGame() {	
	Tick();
}
		
void BPMiniGame::RenderMiniGame() {
	if (FinishedTime != 0 && FinishedTime + 250 < this->TheGame->TickCount) {
		RenderScore();
	} else {
		Render();
				
		TheGame->DrawImage(TheGame->sfcBottomBar, 0, 416);
		
		if (WantToQuit != -1) {
			float diff = TheGame->TickCount - WantToQuit;
			diff /= 200.0f;
			diff = TheGame->Clamp(diff, 0.0f, 1.0f);
			
			Colour col = Colour(1.0f, 1.0f, 1.0f, TheGame->SmoothStep(0.0f, 1.0f, diff));
			
			TheGame->DrawImage(TheGame->sfcQuitTest, 0, 0, col);
		}
	}
}
		
void BPMiniGame::RenderScore() {
	switch (FinalRank) {
		case FAIL:
			TheGame->DrawImage(TheGame->sfcResultsFail, 0, 0);
			break;
					
		case BRONZE:
			TheGame->DrawImage(TheGame->sfcResultsBronze, 0, 0);
			break;
					
		case SILVER:
			TheGame->DrawImage(TheGame->sfcResultsSilver, 0, 0);
			break;
					
		case GOLD:
			TheGame->DrawImage(TheGame->sfcResultsGold, 0, 0);
			break;
					
		case PLATINUM:
			TheGame->DrawImage(TheGame->sfcResultsPlatinum, 0, 0);
			break;
					
		default: // no score has been assigned yet; just fill it in black
			TheGame->Clear(TheGame->Black);
			return;
	}

	TheGame->DrawString(sfcFinalWeight, WHITE, 0, 10);
	TheGame->DrawString(sfcFinalGrade, WHITE, 0, 50);
}
		
void BPMiniGame::Success() {
	TheGame->StopMusic();
	
	// minigame was completed successfully!
	if (FinishedTime == 0) {
		FinishedTime = TheGame->TickCount;
		CalculateResult();
		
		if (FinalRank == FAIL) {
			TheGame->PlaySound("glass_break");
		} else if (FinalRank == PLATINUM) {
			TheGame->PlaySound("result");
			
			if (TheGame->Secret1 == false && strcmp(GameTitle, "Balloon Blaster") == 0) {
				// they just unlocked a secret
				MessageBox::Show("Congratulations on getting a platinum rating in Balloon Blaster - as a special bonus you've also unlocked a secret! Visit the Results screen to see what you've unlocked.", "Congratulations!");
				TheGame->Secret1 = true;
				TheGame->SaveSettings();
			} else if (TheGame->Secret2 == false && strcmp(GameTitle, "Jewel Jam") == 0) {
				// they just unlocked a secret
				MessageBox::Show("Congratulations on getting a platinum rating in Jewel Jam - as a special bonus you've also unlocked a secret! Visit the Results screen to see what you've unlocked.", "Congratulations!");
				TheGame->Secret2 = true;
				TheGame->SaveSettings();				
			} else if (TheGame->Secret3 == false && strcmp(GameTitle, "Odd One Out") == 0) {
				// they just unlocked a secret
				MessageBox::Show("Congratulations on getting a platinum rating in Odd One Out - as a special bonus you've also unlocked a secret! Visit the Results screen to see what you've unlocked.", "Congratulations!");
				TheGame->Secret3 = true;
				TheGame->SaveSettings();
			} else if (TheGame->Secret4 == false && strcmp(GameTitle, "Untangler") == 0) {	
				// they just unlocked a secret
				MessageBox::Show("Congratulations on getting a platinum rating in Untangler - as a special bonus you've also unlocked a secret! Visit the Results screen to see what you've unlocked.", "Congratulations!");
				TheGame->Secret4 = true;
				TheGame->SaveSettings();
			}
		} else {
			TheGame->PlaySound("result");			
		}
	}
}
		
void BPMiniGame::Failure() {
	TheGame->StopMusic();
	
	// minigame was not completed successfully!
	if (FinishedTime == 0) {
		FinishedTime = TheGame->TickCount;
		FinalWeight = 0;
		FinalRank = GetRank(0);
		FinalGrade = GetGrade(0);
		
		TheGame->PlaySound("glass_break");
	}
}
		
void BPMiniGame::CalculateResult() {
	FinalWeight = GetWeight();
	FinalRank = GetRank(FinalWeight);
	FinalGrade = GetGrade(FinalWeight);
	
	ostringstream brainweight;
	brainweight << "Brain weight: " << FinalWeight << "g";
	TheGame->AllocString(&sfcFinalWeight, brainweight.str().c_str(), LARGE, 320, 120, CENTRED);
	
	ostringstream finalgrade;
	finalgrade << "Grade: " << FinalGrade;
	TheGame->AllocString(&sfcFinalGrade, finalgrade.str().c_str(), LARGE, 320, 80, CENTRED);
}
		
void BPMiniGame::HandleMouseUp(BPPoint e) {		
	TouchEvent = e;
	
	if (FinishedTime != 0) {
		if (FinishedTime + 900 < TheGame->TickCount) {
			// showing results screen; this delay is important because it stops people skipping the results screen by accident
			ContinueGame();
		} else {
			// do nothing; waiting for results screen to appear
		}
	} else {
		if (WantToQuit != -1) {
			// player has asked to quit the test - check their confirmation result
			if (TheGame->PointOverRect(e.X, e.Y, 0, 274, 320, 65)) {
				// don't quit!
				WantToQuit = -1;
				TheGame->PlaySound("mouse_click");
			} else if (TheGame->PointOverRect(e.X, e.Y, 0, 370, 320, 65)) {
				TheGame->CancelTest();
				TheGame->PlaySound("mouse_click");
			}
		} else {
			if (e.Y > 416 && (BackDown || HelpDown)) {
				// user clicked on either Back or Help; if they didn't mouse down + mouse up on back or help, pass the click to the mini game for handling
				if (e.X < 89) {
					if (BackDown) {
						// only go back if we have mouse down + mouse up
						if (TheGame->InTestMode) {
							WantToQuit = TheGame->TickCount;
						} else {
							GoBack();
						}
						
						TheGame->PlaySound("mouse_click");
					}
				} else if (e.X > 228) {
					if (HelpDown) {
						ShowHelp(); // only go back if we have mouse down + mouse up
						TheGame->PlaySound("mouse_click");
					}
				}
			} else {
				OnMouseUp();
			}
					
			BackDown = false;
			HelpDown = false;
		}
	}
}
		
void BPMiniGame::HandleMouseDown(BPPoint e) {
	if (WantToQuit != -1) return; // user is trying to quit; ignore these taps
	
	TouchEvent = e;
	
	if (FinishedTime != 0) return; // ignore these clicks
			
	if (e.Y > 416) {
		// user clicked on either Back or Help
		if (e.X < 89) {
			BackDown = true;
		} else if (e.X > 228) {
			HelpDown = true;
		}
	} else {
		// Uncomment to quickly test whizzing through games	
		//Success();
		//return;
		OnMouseDown();
	}
}
		
void BPMiniGame::HandleMouseMove(BPPoint e) {
	if (WantToQuit != -1) return; // user is trying to quit; ignore these taps
	
	TouchEvent = e;
	
	if (e.Y < 416) {
		OnMouseMove();
	}
}
		
int BPMiniGame::DivRem(int Num, int Div, int &Rem) {
	Rem = Num % Div;
	return (int)floor(Num / Div);
}
		
void BPMiniGame::ShowHelp() {
	MessageBox::Show(GameHelp2, GameTitle);
}
		
void BPMiniGame::GoBack() {
	switch (ReturnType) {
		case RT_NORMAL:
			if (MarathonMode) {
				TheGame->GameState = MARATHON;
				TheGame->PlayMusic("theme");
			} else {
				if (TheGame->InTestMode) {
					TheGame->GameState = TEST_STATUS;
				} else {
					TheGame->GameState = PRACTISE_MENU;
					TheGame->PlayMusic("theme");
				}
			}
			
			break;
			
		case RT_SECRET:
			TheGame->GameState = HISTORY;
			TheGame->PlayMusic("theme");

			break;
			
		case RT_BRAINBOOST:
			TheGame->GameState = BRAINBOOST;
			TheGame->PlayMusic("theme");
			break;
	}
}
		
void BPMiniGame::ContinueGame() {
	if (TheGame->InTestMode) {
		TheGame->AddTestScore(FinalWeight);
	} else {
		GoBack();
	}
}
		
int BPMiniGame::Round(double num) {
	return (int)round(num);
}
		
int BPMiniGame::MinMax(int num) {
	if (num <= 0) return 0;
			
	float fnum = num;
			
	if (fnum > 500) return 500;

	return round(fnum);
}
		
void BPMiniGame::SetMarathon() {
	// do nothing by default
}

void BPMiniGame::DrawProfessorText() {
	TheGame->DrawString(sfcGameTitle, BLACK, 23, 20);
	TheGame->DrawString(sfcGameHelp, BLACK, 23, 53);
}

bool BPMiniGame::RedrawClock() {
	// used to redraw clocks in minigames that use them at a regular pace
	static int ClockTime = 0;
		
	ClockTime += TheGame->ElapsedTickCount;
		
	if (ClockTime > TIMERREDRAW) {
		ClockTime -= TIMERREDRAW;
		return true;
	} else {
		return false;
	}
}


void BPMiniGame::RenderCorrect() {
	TheGame->DrawImage(TheGame->sfcCorrect, 0, 172);
}

void BPMiniGame::RenderWrong() {
	TheGame->DrawImage(TheGame->sfcWrong, 0, 172);
}

void BPMiniGame::PlayMusic() {
	switch (MiniGameType) {
		case ACTION:
			switch (TheGame->RandomRange(0, 3)) {
				case 0:
					TheGame->PlayMusic("starmarch");
					break;
					
				case 1:
					TheGame->PlayMusic("nevertoolate");
					break;
					
				case 2:
					TheGame->PlayMusic("chekolake");
					break;
					
				case 3:
					TheGame->PlayMusic("highway");
					break;
			}
			
			break;
			
		case LIVELY:
			switch (TheGame->RandomRange(0, 2)) {
				case 0:
					TheGame->PlayMusic("lively");
					break;
					
				case 1:
					TheGame->PlayMusic("electricity");
					break;
					
				case 2:
					TheGame->PlayMusic("nevertoolate");
					break;
			}
			
			break;
			
		case PUZZLE:
			switch (TheGame->RandomRange(0, 2)) {
				case 0:
					TheGame->PlayMusic("ambient");
					break;
					
				case 1:
					TheGame->PlayMusic("brainrace");
					break;
					
				case 2:
					TheGame->PlayMusic("morningwave");
					break;
			}
			 
			break;
	}
}

void BPMiniGame::GenerateStarfield(BPPList<BPMiniGame_BGStar*> &list) {
	for (int i = 0; i < 100; ++i) {
		BPMiniGame_BGStar* star = new BPMiniGame_BGStar();
		star->Type = TheGame->RandomRange(0, 2);
		star->Pos = BPPoint(TheGame->RandomRange(0, 320), TheGame->RandomRange(0, 416));
		star->Speed = (star->Type + TheGame->RandomRange(0, 2)) / 3.0f;
		if (star->Speed < 0.2f) star->Speed = 0.2f;
		list.Add(star);
	}
}

void BPMiniGame::UpdateStarfield(BPPList<BPMiniGame_BGStar*> &list) {
	for (int i = 0; i < list.Count; ++i) {
		BPMiniGame_BGStar* star = list[i];
		
		star->Pos.X -= star->Speed;
		if (star->Pos.X < -2) {
			star->Pos = BPPoint(322, TheGame->RandomRange(0, 416));			
		}
	}
}

void BPMiniGame::DrawStarfield(BPPList<BPMiniGame_BGStar*> &list) {
	for (int i = 0; i < 100; ++i) {
		BPMiniGame_BGStar* star = list[i];

		TheGame->DrawImage(TheGame->sfcStarTypes[star->Type], star->Pos.X, star->Pos.Y);
	}
}
