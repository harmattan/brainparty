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

#include "iqtest.h"
#include "Minigame.h"

BPMiniGame_IQTest::BPMiniGame_IQTest(BPGame* game) : BPMiniGame(game) {
	sfcBackground = TheGame->LoadBitmap("iqtest", 320, 416);
	
	CurrentQuestion = NULL;
	CurrentAnswer = false;
	
	sfcCurrentQuestion = sfcScore = NULL;
	
	Score = Tries = NumWrong = TimeStarted = 0;
	SetScore();
	
	GameState = WAITING;
	LastStateChange = 0;
	
	SuccessTime = -1;
	
	FourLeggedAnimals = new BPList<const char*>();
	TwoLeggedAnimals = new BPList<const char*>();
	
	GameTitle = "IQ Test";
	GameHelp = "Ah, mathematics. These simple questions might look easy, but you need to decide whether what I say is true or false. Take your time - you get a point for a correct answer, but you lose more when you get it wrong!";
	GameHelp2 = "To get the best score, try to group animals together: add up how many four-legged animals there are and multiply it by four, then add up how many two-legged animals there are and multiply it by two; now add those two numbers together to have the total number of legs.";
	
	MiniGameType = PUZZLE;
	
	FourLeggedAnimals->Add("dogs");
	FourLeggedAnimals->Add("cats");
	FourLeggedAnimals->Add("lions");
	FourLeggedAnimals->Add("tigers");
	FourLeggedAnimals->Add("bears");
	
	TwoLeggedAnimals->Add("chickens");
	TwoLeggedAnimals->Add("ducks");
	TwoLeggedAnimals->Add("birds");
	TwoLeggedAnimals->Add("penguins");
		
	AskQuestion();
}

BPMiniGame_IQTest::~BPMiniGame_IQTest() {
	SAFE_DELETE(sfcBackground);
	SAFE_DELETE(CurrentQuestion);

	FourLeggedAnimals->Clear();
	SAFE_DELETE(FourLeggedAnimals);
	
	TwoLeggedAnimals->Clear();
	SAFE_DELETE(TwoLeggedAnimals);
	
	SAFE_DELETE(sfcCurrentQuestion);
	SAFE_DELETE(sfcScore);
}

void BPMiniGame_IQTest::AskQuestion() {
	if (SuccessTime != -1) return;
	
	int i, j, k, l, m, n, o, p;
	ostringstream question;
	int totallegs = 0;
	
	FourLeggedAnimals->Shuffle();
	TwoLeggedAnimals->Shuffle();
	
	switch (Score) {
		case 1:
			// two four-legged animals
			i = TheGame->RandomRange(2, 4);
			j = TheGame->RandomRange(2, 4);
			
			totallegs = (i * 4) + (j * 4);
			
			question << i << " " << (*FourLeggedAnimals)[0] << " and " << j << " " << (*FourLeggedAnimals)[1];
			
			break;
			
		case 2:
			// two four-legged animals and a two-legged animal
			i = TheGame->RandomRange(2, 4);
			j = TheGame->RandomRange(2, 4);
			k = TheGame->RandomRange(2, 4);
			
			totallegs = (i * 4) + (j * 4) + (k * 2);
			
			question << i << " " << (*FourLeggedAnimals)[0] << ", " << j << " " << (*FourLeggedAnimals)[1] << " and " << k << " " << (*TwoLeggedAnimals)[0];
		
			break;
			
		case 3:
			// two four-legged animals and two two-legged animals
			i = TheGame->RandomRange(2, 4);
			j = TheGame->RandomRange(2, 4);
			k = TheGame->RandomRange(2, 4);
			l = TheGame->RandomRange(2, 4);
			
			totallegs = (i * 4) + (j * 4) + (k * 2) + (l * 2);

			question << i << " " << (*FourLeggedAnimals)[0] << ", " << j << " " << (*FourLeggedAnimals)[1] << ", " << k << " " << (*TwoLeggedAnimals)[0] << " and " << l << " " << (*TwoLeggedAnimals)[1];
			break;
			
		case 4:
			// three four-legged animals and three two-legged animals
			i = TheGame->RandomRange(2, 4);
			j = TheGame->RandomRange(2, 4);
			k = TheGame->RandomRange(2, 4);
			l = TheGame->RandomRange(2, 4);
			m = TheGame->RandomRange(2, 4);
			n = TheGame->RandomRange(2, 4);
			
			totallegs = (i * 4) + (j * 4) + (k * 4) + (l * 2) + (m * 2) + (n * 2);
			
			question << i << " " << (*FourLeggedAnimals)[0] << ", " << j << " " << (*FourLeggedAnimals)[1] << ", " << k << " " << (*FourLeggedAnimals)[2] << ", " << l << " " << (*TwoLeggedAnimals)[0] << ", " << m << " " << (*TwoLeggedAnimals)[1] << " and " << n << " " << (*TwoLeggedAnimals)[2];
			
			break;
			
		case 5:
			// four four-legged animals and four two-legged animals
			i = TheGame->RandomRange(2, 4);
			j = TheGame->RandomRange(2, 4);
			k = TheGame->RandomRange(2, 4);
			l = TheGame->RandomRange(2, 4);
			m = TheGame->RandomRange(2, 4);
			n = TheGame->RandomRange(2, 4);
			o = TheGame->RandomRange(2, 4);
			p = TheGame->RandomRange(2, 4);
			
			totallegs = (i * 4) + (j * 4) + (k * 4) + (l * 4) + (m * 2) + (n * 2) + (o * 2) + (p * 2);
			
			question << i << " " << (*FourLeggedAnimals)[0] << ", " << j << " " << (*FourLeggedAnimals)[1] << ", " << k << " " << (*FourLeggedAnimals)[2] << ", " << l << " " << (*FourLeggedAnimals)[3] << ", " << m << " " << (*TwoLeggedAnimals)[0] << ", " << n << " " << (*TwoLeggedAnimals)[1] << ", " << o << " " << (*TwoLeggedAnimals)[2] << " and " << p << " " << (*TwoLeggedAnimals)[3];
			break;
			
		default:
			// one four-legged animal
			i = TheGame->RandomRange(2, 4);
			totallegs = i * 4;
				
			question << i << " " << (*FourLeggedAnimals)[0];
			
			break;
	}
	
	if (TheGame->RandomRange(0, 1) == 0) {
		CurrentAnswer = true;
		question << " have " << totallegs << " legs.";
	} else {
		CurrentAnswer = false;
		question << " have " << totallegs + 4 << " legs.";
	}
	
	TheGame->AllocString(&sfcCurrentQuestion, question.str().c_str(), LARGE, 273, 275, CENTRED);
}

void BPMiniGame_IQTest::FalseClicked() {
	if (GameState == CORRECT || GameState == WRONG) return;
	
	++Tries;
	
	if (CurrentAnswer == false) {
		TheGame->PlaySound("correct");
		GameState = CORRECT;
		LastStateChange = TheGame->TickCount;
	} else {
		TheGame->PlaySound("wrong");
		GameState = WRONG;
		LastStateChange = TheGame->TickCount;
	}
}

void BPMiniGame_IQTest::TrueClicked() {
	if (GameState == CORRECT || GameState == WRONG) return;
	
	++Tries;
	
	if (CurrentAnswer == true) {
		TheGame->PlaySound("correct");
		GameState = CORRECT;
		LastStateChange = TheGame->TickCount;
	} else {
		TheGame->PlaySound("wrong");
		GameState = WRONG;
		LastStateChange = TheGame->TickCount;
	}
}

void BPMiniGame_IQTest::UpdateProgress() {
	if (Score < 0) Score = 0;
	
	if (Tries >= 6 && SuccessTime == -1) {
		SuccessTime = TheGame->TickCount;
	}
	
	SetScore();
}

void BPMiniGame_IQTest::Start() {
	TimeStarted = TheGame->TickCount;
}

int BPMiniGame_IQTest::GetWeight() {
	float TimePassed = (TheGame->TickCount - TimeStarted) / 1000.0f;
	return MinMax(550 - (NumWrong * 150) - floor(TimePassed * 1.5f));
}

void BPMiniGame_IQTest::Render() {	
	TheGame->DrawImage(sfcBackground, 0, 0);
	
	if (SuccessTime == -1) {
		TheGame->DrawString(sfcCurrentQuestion, WHITE, 24, 35);
	}
	
	TheGame->DrawString(sfcScore, WHITE, 48, 364);
	
	if (GameState == CORRECT) {
		RenderCorrect();
	} else if (GameState == WRONG) {
		RenderWrong();
	}
}

void BPMiniGame_IQTest::Tick() {	
	if (SuccessTime != -1 && SuccessTime + 250 < TheGame->TickCount) {
		Success();
		return;
	}
	
	if (GameState != WAITING) {
		if (LastStateChange + 500 < TheGame->TickCount) {
			if (GameState == CORRECT) {
				++Score;
				AskQuestion();
			} else {
				++NumWrong;
				AskQuestion();
			}
			
			LastStateChange = TheGame->TickCount;
			GameState = WAITING;
			
			UpdateProgress();
		}
	}
}

void BPMiniGame_IQTest::OnMouseUp() {
	
}

void BPMiniGame_IQTest::OnMouseMove() {
	
}

void BPMiniGame_IQTest::OnMouseDown() {
	if (SuccessTime != -1) {
		if (SuccessTime + 750 < TheGame->TickCount) {
			Success();
		}
		
		return;
	}
	
	if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 31, 312, 127, 49)) {
		TrueClicked();
	} else if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 164, 312, 127, 49)) {
		FalseClicked();
	}
}

void BPMiniGame_IQTest::SetScore() {
	ostringstream score;
	
	score << "Score: " << Score << "/6";
	TheGame->AllocString(&sfcScore, score.str().c_str(), NORMAL, 229, 28, CENTRED);
}
