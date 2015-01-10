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

#include "numbersnake.h"
#include "Minigame.h"
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iterator>

BPMiniGame_NumberSnake::BPMiniGame_NumberSnake(BPGame* game) : BPMiniGame(game) {
	sfcBackground = TheGame->LoadBitmap("numbersnake", 320, 416);
	sfcBlack = TheGame->LoadBitmap("numbersnake_black", 320, 128);

	CurrentLevel = 0;
	
	LastStateChange = LastQuestionChange = -1;
	NumCorrect = 0;
	
	sfcQuestionParts = NULL;
	Answer = 0;
	
	QuestionPos = 0;
	QuestionSpeed = 1250;
	
	TimeStarted = 0;	
	
	GameTitle = "Number Snake";
	GameHelp = "Watch carefully as simple mathematics questions flash on your screen, then see if you can figure out the answers!";
	GameHelp2 = "This is a game that tests your ability to perform simple sums very quickly. In fact, if you're not quick then you won't have finished the first sum by the time the second sum appears, so there really is no time to hang around!";
	
	MiniGameType = PUZZLE;
	
	NumStrings.Add("zero");
	NumStrings.Add("one");
	NumStrings.Add("two");
	NumStrings.Add("three");
	NumStrings.Add("four");
	NumStrings.Add("five");
	NumStrings.Add("six");
	NumStrings.Add("seven");
	NumStrings.Add("eight");
	NumStrings.Add("nine");
}

BPMiniGame_NumberSnake::~BPMiniGame_NumberSnake() {
	SAFE_DELETE(sfcBackground);
	SAFE_DELETE(sfcBlack);
	
	NumStrings.Clear();
	
	if (sfcQuestionParts != NULL) {
		for (int i = 0; i < QuestionLength; ++i) {
			SAFE_DELETE(sfcQuestionParts[i]);
		}
		
		delete[] sfcQuestionParts;
	}
}

void BPMiniGame_NumberSnake::OnMouseDown() {
	
}

void BPMiniGame_NumberSnake::OnMouseMove() {
	
}

void BPMiniGame_NumberSnake::OnMouseUp() {
	if (TouchEvent.Y < 100) {
		// possible click on score
		if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 0, 4, 52, 47)) SubmitAnswer(0);
		if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 66, 4, 52, 47)) SubmitAnswer(1);
		if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 134, 4, 52, 47)) SubmitAnswer(2);
		if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 201, 4, 52, 47)) SubmitAnswer(3);
		if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 272, 4, 52, 47)) SubmitAnswer(4);
		if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 0, 52, 52, 47)) SubmitAnswer(5);
		if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 66, 52, 52, 47)) SubmitAnswer(6);
		if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 134, 52, 52, 47)) SubmitAnswer(7);
		if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 201, 52, 52, 47)) SubmitAnswer(8);
		if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 272, 52, 52, 47)) SubmitAnswer(9);
	}
}

void BPMiniGame_NumberSnake::Start() {
	TimeStarted = TheGame->TickCount;
	
	LevelUp();
}

int BPMiniGame_NumberSnake::GetWeight() {
	return MinMax(100 * NumCorrect);
}

void BPMiniGame_NumberSnake::Render() {
	TheGame->DrawImage(sfcBackground, 0, 0);
	
	if (GameState == SHOWING) {
		if (QuestionPos == QuestionLength - 1) {
			float fadeamount = min(0.65f, Fader);
			glColor4f(1.0f, 1.0f, 1.0f, fadeamount);
			TheGame->DrawImage(sfcBlack, 0, 165);
		} else {
			glColor4f(1.0f, 1.0f, 1.0f, 0.65f);
			TheGame->DrawImage(sfcBlack, 0, 165);			
		}

		glColor4f(1.0f, 1.0f, 1.0f, Fader);
		TheGame->DrawString(sfcQuestionParts[QuestionPos], 0, 170);		
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}
	
	if (GameState == CORRECT) {
		RenderCorrect();
	} else if (GameState == WRONG) {
		RenderWrong();
	}
}

void BPMiniGame_NumberSnake::Tick() {
	if (GameState == SHOWING) {
		if (LastQuestionChange + QuestionSpeed < TheGame->TickCount) {
			++QuestionPos;
			LastQuestionChange = TheGame->TickCount;
			
			if (QuestionPos == QuestionLength) {
				GameState = WAITING;
			}
		}
		
		float start = TheGame->TickCount + QuestionSpeed;
		float end = LastQuestionChange + QuestionSpeed;
		float alpha =  (start - end) / QuestionSpeed;
		Fader = TheGame->SmoothStep(1.0f, 0.0f, alpha);
	}
	
	if (LastStateChange != -1 && LastStateChange + 500 < TheGame->TickCount) {
		if (!MarathonMode && CurrentLevel >= 5) {
			Success();
		} else {
			LevelUp();
		}
	}
}

void BPMiniGame_NumberSnake::SubmitAnswer(int answer) {
	if (GameState != WAITING) return;
	
	if (answer == Answer) {
		TheGame->PlaySound("correct");
		GameState = CORRECT;
		++NumCorrect;
	} else {
		TheGame->PlaySound("wrong");
		GameState = WRONG;
	}
	
	LastStateChange = TheGame->TickCount;
}

void BPMiniGame_NumberSnake::LevelUp() {
	++CurrentLevel;
	QuestionPos = 0;
	LastQuestionChange = TheGame->TickCount;
	QuestionSpeed -= 50;
	
	BPList<int> Numbers;
	
	Answer = TheGame->RandomRange(1, 6);
	
	ostringstream PrintMe;	
	PrintMe << "What is " << NumStrings[Answer];
	
	for (int i = 0; i < 4 + CurrentLevel; ++i) {
		Numbers.Add(TheGame->RandomRange(1, 6));
	}
	
	int RandNum;
	
	for (int i = 0; i < Numbers.Count; ++i) {
		int num = Numbers[i];
		
		// we want to allow subtraction only if it doesn't dip below zero!
		if (Answer - num > 0) {
			// only allow multiplication if the total doesn't go above 25
			if (Answer * num < 26) {
				RandNum = TheGame->RandomRange(0, 2);
			} else {
				RandNum = TheGame->RandomRangeExcept(0, 2, 1);
			}
		} else {
			// only allow multiplication if the total doesn't go above 25
			if (Answer * num < 26) {
				RandNum = TheGame->RandomRange(0, 1);
			} else {
				RandNum = 0;
			}
		}
		
		switch (RandNum) {
			case 0:
				// add
				PrintMe << " plus " << NumStrings[num];
				Answer += num;
				break;
				
			case 1:
				PrintMe << " times " << NumStrings[num];
				Answer *= num;
				break;
				
			case 2:
				// subtract
				PrintMe << " minus " << NumStrings[num];
				Answer -= num;
				break;
		}
	}
	
	while (Answer >= 10) {
		int newnum = TheGame->RandomRange(5, 9);

		PrintMe << " minus " << NumStrings[newnum];
		Answer -= newnum;
	}
	
	PrintMe << " ?";

	istringstream iss(PrintMe.str());	
	vector<string> parts;
	copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string> >(parts));
	
	if (sfcQuestionParts != NULL) {
		for (int i = 0; i < QuestionLength; ++i) {
			SAFE_DELETE(sfcQuestionParts[i]);
		}
		
		delete[] sfcQuestionParts;
	}
	
	QuestionLength = parts.size();
		
	sfcQuestionParts = new SpriteFont*[QuestionLength];
	
	int i = 0;
	for (int i = 0; i < parts.size(); ++i) {
		sfcQuestionParts[i] = NULL;
		TheGame->AllocString(&sfcQuestionParts[i], parts[i].c_str(), EPIC, 320, 227, CENTRED);
	}
	
	GameState = SHOWING;
	LastStateChange = -1;
}

void BPMiniGame_NumberSnake::SetMarathon() {
	MarathonMode = true;
	GameHelp = "Watch carefully as simple mathematics questions flash on your screen, then see if you can figure out the answers!";
}
