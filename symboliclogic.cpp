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

#include "symboliclogic.h"
#include "Minigame.h"
#include <algorithm>
#include <cctype>
		
BPMiniGame_SymbolicLogic::BPMiniGame_SymbolicLogic(BPGame* game) : BPMiniGame(game) {
	sfcBackground = TheGame->LoadBitmap("symboliclogic", 320, 416);
	
	LastStateChange = CurrentLevel = Score = NumTries = 0;
	GameState = WAITING;
	
	sfcText = NULL;

	AnswerIsTrue = false;
	
	TimeStarted = 0;
	
	GameTitle = "Symbolic Logic";
	GameHelp = "I'm going to give you two statements that I'd like you to accept as truth. Based on those two facts, are my conclusions true or false?";
	GameHelp2 = "If you read the questions carefully, you should be OK, but remember: if the conclusion says \"A must be a B\" and you think \"A could be a B or it could not be a B; I don't know\", then clearly the conclusion is wrong!";
	
	MiniGameType = PUZZLE;
	
	LogicItems.Add(new BPMiniGame_SymbolicLogic_Item("can play the guitar", "can't play the guitar", "can play the guitar", "guitarists"));
	LogicItems.Add(new BPMiniGame_SymbolicLogic_Item("can play the piano", "can't play the piano", "can play the piano", "pianists"));
	LogicItems.Add(new BPMiniGame_SymbolicLogic_Item("can play the violin", "can't play the violin", "can play the violin", "violinists"));
	LogicItems.Add(new BPMiniGame_SymbolicLogic_Item("can speak Chinese", "can't speak Chinese", "can speak Chinese", "Chinese speakers"));
	LogicItems.Add(new BPMiniGame_SymbolicLogic_Item("can speak French", "can't speak French", "can speak French", "French speakers"));
	LogicItems.Add(new BPMiniGame_SymbolicLogic_Item("can speak Japanese", "can't speak Japanese", "can speak Japanese", "Japanese speakers"));
	LogicItems.Add(new BPMiniGame_SymbolicLogic_Item("can speak Latin", "can't speak Latin", "can speak Latin", "Latin speakers"));
	LogicItems.Add(new BPMiniGame_SymbolicLogic_Item("can speak Spanish", "can't speak Spanish", "can speak Spanish", "Spanish speakers"));
	LogicItems.Add(new BPMiniGame_SymbolicLogic_Item("drives a car", "can't drive a car", "drive a car", "car drivers"));
	LogicItems.Add(new BPMiniGame_SymbolicLogic_Item("eats mashed potato", "doesn't eat mashed potato", "like mashed potato", "mashed potato eaters"));
	LogicItems.Add(new BPMiniGame_SymbolicLogic_Item("enjoys cooking", "doesn't enjoy cooking", "enjoy cooking", "cooks"));
	LogicItems.Add(new BPMiniGame_SymbolicLogic_Item("exercises regularly", "doesn't exercise regularly", "exercise regularly", "regular exercisers"));
	LogicItems.Add(new BPMiniGame_SymbolicLogic_Item("goes to church", "doesn't go to church", "go to church", "churchgoers"));
	LogicItems.Add(new BPMiniGame_SymbolicLogic_Item("has brown hair", "doesn't have brown hair", "have brown hair", "brown-haired people"));
	LogicItems.Add(new BPMiniGame_SymbolicLogic_Item("has blue eyes", "doesn't have blue eyes", "have blue eyes", "blue-eyed people"));
	LogicItems.Add(new BPMiniGame_SymbolicLogic_Item("has curly hair", "doesn't have curly hair", "have curly hair", "curly-haired people"));
	LogicItems.Add(new BPMiniGame_SymbolicLogic_Item("hates computers", "doesn't hate computers", "hate computers", "computer haters"));
	LogicItems.Add(new BPMiniGame_SymbolicLogic_Item("is very clever", "isn't very clever", "are very clever", "very clever people"));
	LogicItems.Add(new BPMiniGame_SymbolicLogic_Item("is very popular", "isn't very popular", "are very popular", "very popular people"));
	LogicItems.Add(new BPMiniGame_SymbolicLogic_Item("likes bowling", "doesn't like bowling", "like bowling", "bowlers"));
	LogicItems.Add(new BPMiniGame_SymbolicLogic_Item("likes broccoli", "doesn't like broccoli", "like broccoli", "broccoli eaters"));
	LogicItems.Add(new BPMiniGame_SymbolicLogic_Item("likes eating carrots", "doesn't like eating carrots", "like eating carrots", "carrot eaters"));
	LogicItems.Add(new BPMiniGame_SymbolicLogic_Item("likes eating dessert", "doesn't like eating dessert", "like eating dessert", "dessert eaters"));
	LogicItems.Add(new BPMiniGame_SymbolicLogic_Item("likes puzzles", "doesn't like puzzles", "like puzzles", "people who like puzzles"));
	LogicItems.Add(new BPMiniGame_SymbolicLogic_Item("likes to sing", "doesn't like to sing", "like to sing", "singers"));
	LogicItems.Add(new BPMiniGame_SymbolicLogic_Item("loves animals", "doesn't like animals", "like animals", "animal lovers"));
	LogicItems.Add(new BPMiniGame_SymbolicLogic_Item("loves Shakespeare", "doesn't love Shakespeare", "love Shakespeare", "Shakespeare lovers"));
	LogicItems.Add(new BPMiniGame_SymbolicLogic_Item("owns a mansion", "doesn't own a mansion", "own a mansion", "mansion owners"));
	LogicItems.Add(new BPMiniGame_SymbolicLogic_Item("plays golf", "doesn't play golf", "play golf", "golfers"));
	LogicItems.Add(new BPMiniGame_SymbolicLogic_Item("plays Sudoku", "doesn't play Sudoku", "play Sudoku", "Sudoku players"));
	LogicItems.Add(new BPMiniGame_SymbolicLogic_Item("plays tennis", "doesn't play tennis", "play tennis", "tennis players"));
	LogicItems.Add(new BPMiniGame_SymbolicLogic_Item("owns a cat", "doesn't own a cat", "own a cat", "cat owners"));
	LogicItems.Add(new BPMiniGame_SymbolicLogic_Item("owns a cellphone", "doesn't own a cellphone", "own a cellphone", "cellphone owners"));
	LogicItems.Add(new BPMiniGame_SymbolicLogic_Item("owns a dog", "doesn't own a dog", "own a dog", "dog owners"));
	LogicItems.Add(new BPMiniGame_SymbolicLogic_Item("owns a goat", "doesn't own a goat", "own a goat", "goat owners"));
	LogicItems.Add(new BPMiniGame_SymbolicLogic_Item("owns a hamster", "doesn't own a hamster", "own a hamster", "hamster owners"));
	LogicItems.Add(new BPMiniGame_SymbolicLogic_Item("owns a parrot", "doesn't own a parrot", "own a parrot", "parrot owners"));
	LogicItems.Add(new BPMiniGame_SymbolicLogic_Item("owns a rabbit", "doesn't own a rabbit", "own a rabbit", "rabbit owners"));
	LogicItems.Add(new BPMiniGame_SymbolicLogic_Item("reads books", "doesn't read books", "read books", "people who read books"));
	LogicItems.Add(new BPMiniGame_SymbolicLogic_Item("reads newspapers", "doesn't read newspapers", "read newspapers", "newspaper readers"));
	LogicItems.Add(new BPMiniGame_SymbolicLogic_Item("rides a bike", "doesn't ride a bike", "ride a bike", "bicyclists"));
	LogicItems.Add(new BPMiniGame_SymbolicLogic_Item("walks to work", "doesn't walk to work", "walk to work", "people who walk to work"));
	LogicItems.Add(new BPMiniGame_SymbolicLogic_Item("watches TV", "doesn't watch TV", "watch TV", "TV watchers"));
	LogicItems.Add(new BPMiniGame_SymbolicLogic_Item("wears glasses", "doesn't wear glasses", "wear glasses", "people who wear glasses"));
	LogicItems.Add(new BPMiniGame_SymbolicLogic_Item("wrestles crocodiles", "doesn't wrestle crocodiles", "wrestle crocodiles", "crocodile wrestlers"));
}

BPMiniGame_SymbolicLogic::~BPMiniGame_SymbolicLogic() {
	SAFE_DELETE(sfcBackground);
	
	LogicItems.Clear();
	Premises.Clear();
	
	SAFE_DELETE(sfcText);
}

void BPMiniGame_SymbolicLogic::Start() {
	TimeStarted = TheGame->TickCount;
	LevelUp();
}

int BPMiniGame_SymbolicLogic::GetWeight() {
	float TimePassed = (TheGame->TickCount - TimeStarted) / 1000.0f;
	return MinMax(Score - round(TimePassed));
}

void BPMiniGame_SymbolicLogic::Render() {
	TheGame->DrawImage(sfcBackground, 0, 0);
	
	TheGame->DrawString(sfcText, WHITE, 20, 19);
	
	if (GameState == CORRECT) {
		RenderCorrect();
	} else if (GameState == WRONG) {
		RenderWrong();
	}
}

const char* BPMiniGame_SymbolicLogic::FlattenPremise(BPMiniGame_SymbolicLogic_Premise* premise) {
	ostringstream result;
	
	switch (premise->Type) {
		case ALL_JOB:
			result << "All " << premise->Job << "s " << premise->Item->PluralDesc;
			break;
			
		case NONE_JOB:
			result << "No " << premise->Job << " " << premise->Item->SingularDesc;
			break;
			
		case ALL_ITEM:
			result << "All " << premise->Item->Noun << " are " << premise->Job << "s";
			break;
			
		case NONE_ITEM:
			result << "No " << premise->Item->Noun << " are " << premise->Job << "s";
			break;
			
		case PERSON:
			if (premise->Item == NULL) {
				if (TheGame->StartsWithVowel(&premise->Job)) {
					result << premise->Text << " is an " << premise->Job;
				} else {
					result << premise->Text << " is a " << premise->Job;
				}
			} else {
				result << premise->Text << " " << premise->Item->SingularDesc;
			}
			
			break;
	}
		
	return result.str().c_str();
}

void BPMiniGame_SymbolicLogic::Tick() {
	if (LastStateChange != -1 && LastStateChange + 500 < TheGame->TickCount) {
		if (NumTries >= 10 && !MarathonMode) {
			Success();
			return;
		} else {
			LevelUp();
		}
	}
}

void BPMiniGame_SymbolicLogic::OnMouseDown() {
	
}

void BPMiniGame_SymbolicLogic::OnMouseMove() {
	
}

void BPMiniGame_SymbolicLogic::OnMouseUp() {
	switch (GameState) {
		case CORRECT:
		case WRONG:
			LevelUp();
			break;
			
		case WAITING:
			// have we just answered the question?
			
			if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 11, 342, 143, 63)) {
				// True clicked!
				LastStateChange = TheGame->TickCount;
				
				if (AnswerIsTrue) {
					TheGame->PlaySound("correct");
					GameState = CORRECT;
					Score += 50;
				} else {
					TheGame->PlaySound("wrong");
					GameState = WRONG;
					Score -= 100;
				}
			} else if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 166, 342, 143, 63)) {
				// False clicked!
				LastStateChange = TheGame->TickCount;
				
				if (!AnswerIsTrue) {
					TheGame->PlaySound("correct");
					GameState = CORRECT;
					Score += 50;
				} else {
					TheGame->PlaySound("wrong");
					GameState = WRONG;
					Score -= 50;
				}
			}
			
			break;
	}
}

void BPMiniGame_SymbolicLogic::LevelUp() {
	if (NumTries >= 10 && !MarathonMode) {
		return;
	}
			
	++CurrentLevel;
	++NumTries;
	LastStateChange = -1;
	GameState = WAITING;
		
	Premises.Clear();
	LogicItems.Shuffle();
	
	BPMiniGame_SymbolicLogic_Premise* FirstPremise;
	BPMiniGame_SymbolicLogic_Premise* SecondPremise;
	
	string Job;
	string PersonName;
	ostringstream NegativeConclusion;
	ostringstream Conclusion;
	
	bool is_negative = false;
	
	if (TheGame->RandomRange(0, 1) == 1) {
		NegativeConclusion << "not ";
		is_negative = true;
	}
	
	Job = GetRandomJob();
	PersonName = TheGame->GetName();
	
	FirstPremise = new BPMiniGame_SymbolicLogic_Premise();
	FirstPremise->Type = GetPremiseType();
	FirstPremise->Job = Job;
	FirstPremise->Item = LogicItems[0];
	Premises.Add(FirstPremise);
	
	SecondPremise = new BPMiniGame_SymbolicLogic_Premise();
	SecondPremise->Type = PERSON;
	SecondPremise->Text = PersonName;	
	switch (TheGame->RandomRange(0, 1)) {
		case 0:
			SecondPremise->Item = LogicItems[0];
			Premises.Add(SecondPremise);
			
			if (TheGame->StartsWithVowel(&Job)) {
				Conclusion << "Therefore, " << PersonName << " is " << NegativeConclusion.str() << "an " << Job;
			} else {
				Conclusion << "Therefore, " << PersonName << " is " << NegativeConclusion.str() << "a " << Job;
			}
			
			break;
			
		case 1:
			SecondPremise->Job = Job;
			Premises.Add(SecondPremise);
			
			if (!is_negative) {
				Conclusion << "Therefore, " << PersonName << " " << LogicItems[0]->SingularDesc;
			} else {
				Conclusion << "Therefore, " << PersonName << " " << LogicItems[0]->Negative;
			}
			
			break;
	}
	
	if (FirstPremise->Type == ALL_JOB) {
		if (SecondPremise->Job.size() != 0) {
			AnswerIsTrue = true;
			if (is_negative) AnswerIsTrue = !AnswerIsTrue;
		} else {
			AnswerIsTrue = false;
		}
	} else if (FirstPremise->Type == ALL_ITEM) {
		if (SecondPremise->Item != NULL) {
			AnswerIsTrue = true;
			if (is_negative) AnswerIsTrue = !AnswerIsTrue;
		} else {
			AnswerIsTrue = false;
		}
	} else if (FirstPremise->Type == NONE_JOB) {
		if (SecondPremise->Job.size() != 0) {
			AnswerIsTrue = false;
		} else {
			AnswerIsTrue = false;
		}
		
		if (is_negative) AnswerIsTrue = !AnswerIsTrue;
	} else {
		if (SecondPremise->Item != NULL) {
			AnswerIsTrue = false;
		} else {
			AnswerIsTrue = false;
		}
		
		if (is_negative) AnswerIsTrue = !AnswerIsTrue;
	}
	
	string PrintMe = string();
	
	for (int i = 0; i < Premises.Count; ++i) {
		BPMiniGame_SymbolicLogic_Premise* premise = Premises[i];
		string str = FlattenPremise(premise);
		PrintMe.append(str);
		PrintMe.append(". ");
	}
	
	PrintMe.append(Conclusion.str());
	PrintMe.append(". ");
	
	TheGame->AllocString(&sfcText, PrintMe.c_str(), LARGE, 282, 306, LEFT);
}

PremiseTypes BPMiniGame_SymbolicLogic::GetPremiseType() {
	switch (TheGame->RandomRange(0, 3)) {
		case 0:
			return ALL_JOB;
		case 1:
			return NONE_JOB;
		case 2:
			return ALL_ITEM;
		default:
			return NONE_ITEM;
	}
}

string BPMiniGame_SymbolicLogic::GetRandomJob() {
	string meh = TheGame->TestBrainJobs[TheGame->RandomRange(0, 51)];
	transform(meh.begin(), meh.end(), meh.begin(), (int(*)(int)) tolower);
	return meh;
}

void BPMiniGame_SymbolicLogic::SetMarathon() {
	MarathonMode = true;
}
