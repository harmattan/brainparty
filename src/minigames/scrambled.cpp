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

#include "scrambled.h"
#include "Minigame.h"

BPMiniGame_Scrambled::BPMiniGame_Scrambled(BPGame* game) : BPMiniGame(game) {
	AnswersYOffset = 9;
	
	CurrentLevel = Score = RandSeed = SelectedItem = TimeStarted = 0;

	LastStateChange = -1;
	
	sfcBackground = TheGame->LoadBitmap("scrambled", 320, 416);
	
	sfcAnswer1 = sfcAnswer2 = sfcAnswer3 = sfcAnswer4 = NULL;
	TheQuestion = NULL;
	
	GameTitle = "Scrambled!";
	GameHelp = "I'm going to scramble words and numbers by adding, removing and switching letters. You need to figure out what I've done, then tap the correct answers to my questions.";
	GameHelp2 = "Have letters been added, swapped or deleted? Has the whole word been reversed? Figure out what I changed to solve the problems!";
	
	MiniGameType = PUZZLE;

	// note: each of these words have no duplicate letters, eg "DEERS" is not allowed
	Words.Add(string("ACORN"));
	Words.Add(string("AFTER"));
	Words.Add(string("BEACH"));
	Words.Add(string("BEAST"));
	Words.Add(string("BLACK"));
	Words.Add(string("BLADE"));
	Words.Add(string("BLAST"));
	Words.Add(string("BLAZE"));
	Words.Add(string("BLOCK"));
	Words.Add(string("BRAIN"));
	Words.Add(string("BRAKE"));
	Words.Add(string("BRICK"));
	Words.Add(string("BROWN"));
	Words.Add(string("CARDS"));
	Words.Add(string("CHAMP"));
	Words.Add(string("CHILD"));
	Words.Add(string("CLAMP"));
	Words.Add(string("CLEAR"));
	Words.Add(string("CLEFT"));
	Words.Add(string("CLOSE"));
	Words.Add(string("CODER"));
	Words.Add(string("COULD"));
	Words.Add(string("CRASH"));
	Words.Add(string("DAIRY"));
	Words.Add(string("DIRTY"));
	Words.Add(string("DOZEN"));
	Words.Add(string("DRINK"));
	Words.Add(string("ENJOY"));
	Words.Add(string("FAITH"));
	Words.Add(string("FIELD"));
	Words.Add(string("FINAL"));
	Words.Add(string("FLASH"));
	Words.Add(string("FLASK"));
	Words.Add(string("FLING"));
	Words.Add(string("FLIRT"));
	Words.Add(string("FRESH"));
	Words.Add(string("FUDGE"));
	Words.Add(string("GIANT"));
	Words.Add(string("GLINT"));
	Words.Add(string("GLITZ"));
	Words.Add(string("GRAPH"));
	Words.Add(string("GROWL"));
	Words.Add(string("GUARD"));
	Words.Add(string("GUIDE"));
	Words.Add(string("HEART"));
	Words.Add(string("HEAVY"));
	Words.Add(string("HORSE"));
	Words.Add(string("JUICY"));
	Words.Add(string("LEAST"));
	Words.Add(string("LEMON"));
	Words.Add(string("MAGIC"));
	Words.Add(string("MEANS"));
	Words.Add(string("MICRO"));
	Words.Add(string("MISTY"));
	Words.Add(string("MODEL"));
	Words.Add(string("MOUTH"));
	Words.Add(string("MOVIE"));
	Words.Add(string("OTHER"));
	Words.Add(string("PAIRS"));
	Words.Add(string("PARKS"));
	Words.Add(string("PARTY"));
	Words.Add(string("PHASE"));
	Words.Add(string("PHONE"));
	Words.Add(string("PLACE"));
	Words.Add(string("PLANK"));
	Words.Add(string("PLANT"));
	Words.Add(string("POWER"));
	Words.Add(string("PRICE"));
	Words.Add(string("PRINT"));
	Words.Add(string("QUICK"));
	Words.Add(string("QUIET"));
	Words.Add(string("RATED"));
	Words.Add(string("RIGHT"));
	Words.Add(string("RIVAL"));
	Words.Add(string("ROYAL"));
	Words.Add(string("RUSTY"));
	Words.Add(string("SCALE"));
	Words.Add(string("SCORE"));
	Words.Add(string("SHAKE"));
	Words.Add(string("SHAVE"));
	Words.Add(string("SNORE"));
	Words.Add(string("SPARK"));
	Words.Add(string("SPEAR"));
	Words.Add(string("SPEND"));
	Words.Add(string("STAND"));
	Words.Add(string("STALK"));
	Words.Add(string("STORE"));
	Words.Add(string("SUGAR"));
	Words.Add(string("SWAMP"));
	Words.Add(string("SWIFT"));
	Words.Add(string("SWING"));
	Words.Add(string("THING"));
	Words.Add(string("TRAMP"));
	Words.Add(string("UNDER"));
	Words.Add(string("WATER"));
	Words.Add(string("WHARF"));
	Words.Add(string("WHILE"));
	Words.Add(string("WHITE"));
	Words.Add(string("WHOLE"));
	Words.Add(string("WRITE"));
	
	Numbers.Add(string("0"));
	Numbers.Add(string("1"));
	Numbers.Add(string("2"));
	Numbers.Add(string("3"));
	Numbers.Add(string("4"));
	Numbers.Add(string("5"));
	Numbers.Add(string("6"));
	Numbers.Add(string("7"));
	Numbers.Add(string("8"));
	Numbers.Add(string("9"));
	
	Letters.Add(string("A"));
	Letters.Add(string("B"));
	Letters.Add(string("C"));
	Letters.Add(string("D"));
	Letters.Add(string("E"));
	Letters.Add(string("F"));
	Letters.Add(string("G"));
	Letters.Add(string("H"));
	Letters.Add(string("J"));
	Letters.Add(string("K"));
	Letters.Add(string("L"));
	Letters.Add(string("M"));
	Letters.Add(string("N"));
	Letters.Add(string("P"));
	Letters.Add(string("Q"));
	Letters.Add(string("R"));
	Letters.Add(string("S"));
	Letters.Add(string("T"));
	Letters.Add(string("U"));
	Letters.Add(string("V"));
	Letters.Add(string("W"));
	Letters.Add(string("X"));
	Letters.Add(string("Y"));
	Letters.Add(string("Z"));
	
	Positions.Add(0);
	Positions.Add(1);
	Positions.Add(2);
	Positions.Add(3);
	Positions.Add(4);
	
	for (int i = 0; i < 4; ++i) {
		BPMiniGame_Scrambled_Answer* answer = new BPMiniGame_Scrambled_Answer();
		Answers.Add(answer);
	}
	
	BPPoint point = BPPoint(29, 164);
	AnswerPositions.Add(point);
	
	point = BPPoint(29, 222);
	AnswerPositions.Add(point);
	
	point = BPPoint(29, 280);
	AnswerPositions.Add(point);
	
	point = BPPoint(29, 337);
	AnswerPositions.Add(point);
	
	LevelUp(); 
}

BPMiniGame_Scrambled::~BPMiniGame_Scrambled() {
	SAFE_DELETE(sfcBackground);
	Answers.Clear();
}

void BPMiniGame_Scrambled::Start() {
	TimeStarted = TheGame->TickCount;
}

int BPMiniGame_Scrambled::GetWeight() {
	float TimePassed = (TheGame->TickCount - TimeStarted) / 1000.0f;
	return MinMax((Score * 75) - round(TimePassed * 4));
}

void BPMiniGame_Scrambled::Render() {
	TheGame->DrawImage(sfcBackground, 0, 0);
	
	TheGame->DrawString(TheQuestion, BLACK, 29, 29);
	
	for (int i = 0; i < Answers.Count; ++i) {
		BPMiniGame_Scrambled_Answer* answer = Answers[i];
		TheGame->DrawString(answer->Text, BLACK, answer->Pos.X, answer->Pos.Y + AnswersYOffset);
	}
	
	if (GameState == CORRECT) {
		RenderCorrect();
	} else if (GameState == WRONG) {
		RenderWrong();
	}
}

string BPMiniGame_Scrambled::FlipLetters(string Word) {
	string retval = Word;
	reverse(retval.begin(), retval.end());
	return retval;
}

string BPMiniGame_Scrambled::RemoveLetters(string Word, int Pos) {
	string retval = Word;
	
	retval = retval.erase(Pos, 1);
	return retval;
}

string BPMiniGame_Scrambled::SwitchLetters(string Word, int Pos) {
	string retval = Word;
	const char letter = retval[Pos];
	
	if (Pos == retval.size() - 1) {
		// this was the last letter; insert it at the front
		retval[4] = retval[0];
		retval[0] = letter;
	} else {
		retval[Pos] = retval[Pos + 1];
		retval[Pos + 1] = letter;
	}
	
	return retval;
}

string BPMiniGame_Scrambled::AddLetters(string Word, string Letter, int Pos) {
	string retval = Word;
	retval.insert(Pos, Letter);
	
	return retval;
}

string BPMiniGame_Scrambled::GetUniqueNumber(string Word1, string Word2) {
	for (int i = 0; i < Numbers.Count; ++i) {
		string Number = Numbers[i];
		if (Word1.find(Number) == string::npos && Word2.find(Number) == string::npos) return Number;
	}
	
	return "D"; // no unique numbers are around; send back a letter
}

string BPMiniGame_Scrambled::GetUniqueLetter(string Word1, string Word2) {	
	while (true) {
		string Letter = Letters[TheGame->RandomRange(0, Letters.Count - 1)];
		if (Word1.find(Letter) == string::npos && Word2.find(Letter) == string::npos) return Letter;		
	}
}

void BPMiniGame_Scrambled::Tick() {
	if (LastStateChange != -1 && LastStateChange + 500 < TheGame->TickCount) {
		LevelUp();
	}
}

void BPMiniGame_Scrambled::OnMouseDown() {
	
}

void BPMiniGame_Scrambled::OnMouseMove() {
	
}

void BPMiniGame_Scrambled::OnMouseUp() {
	switch (GameState) {
		case CORRECT:
			LevelUp();
			break;
			
		case WRONG:
			LevelUp();
			break;
			
		case WAITING:
			for (int i = 0; i < Answers.Count; ++i) {
				BPMiniGame_Scrambled_Answer* answer = Answers[i];
				if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, answer->Pos.X, answer->Pos.Y, 264.0f, 47.0f)) {
					if (answer->Correct) {
						++Score;
						GameState = CORRECT;
						LastStateChange = TheGame->TickCount;
						TheGame->PlaySound("correct");
					} else {
						--Score;
						GameState = WRONG;
						LastStateChange = TheGame->TickCount;
						TheGame->PlaySound("wrong");
					}
					
					return;
				}
			}
			
			break;
	}
}

void BPMiniGame_Scrambled::LevelUp() {
	LastStateChange = -1;
	GameState = WAITING;
	
	if (!MarathonMode && CurrentLevel >= 10) {
		Success();
		return;
	}
	
	++CurrentLevel;
	
	Numbers.Shuffle();
	Words.Shuffle();
	Positions.Shuffle();
	Answers.Shuffle();
	
	bool NumbersOnly = false;
	
	switch (TheGame->RandomRange(0, 3)) {
		case 0:
		case 1:
			// word to number comparison
			ExampleWord = Words[0];
			TestWord.clear();
			
			TestWord.append(Numbers[0]);
			TestWord.append(Numbers[1]);
			TestWord.append(Numbers[2]);
			TestWord.append(Numbers[3]);
			TestWord.append(Numbers[4]);
			break;
			
		case 2:
			// word to word comparison
			ExampleWord = Words[0];
			TestWord = Words[1];
			break;
			
		case 3:
			// number to number comparison
			NumbersOnly = true; // so we add a number in AddLetters() rather than adding a letter
			
			ExampleWord.clear();
			
			ExampleWord.append(Numbers[0]);
			ExampleWord.append(Numbers[1]);
			ExampleWord.append(Numbers[2]);
			ExampleWord.append(Numbers[3]);
			ExampleWord.append(Numbers[4]);
			
			Numbers.Shuffle();
			
			TestWord.clear();
			
			TestWord.append(Numbers[0]);
			TestWord.append(Numbers[1]);
			TestWord.append(Numbers[2]);
			TestWord.append(Numbers[3]);
			TestWord.append(Numbers[4]);
			
			break;
	}
	
	switch (TheGame->RandomRange(0, 3)) {
		case 0:
		{
			string LetterToAdd;
			
			if (NumbersOnly) {
				LetterToAdd = GetUniqueNumber(ExampleWord, TestWord);
			} else {
				LetterToAdd = GetUniqueLetter(ExampleWord, TestWord);
			}
			
			ExampleWordAnswer = AddLetters(ExampleWord, LetterToAdd, Positions[0]);
			Answer1 = AddLetters(TestWord, LetterToAdd, Positions[0]);
			Answer2 = AddLetters(TestWord, LetterToAdd, Positions[1]);
			Answer3 = AddLetters(TestWord, LetterToAdd, Positions[2]);
			Answer4 = AddLetters(TestWord, LetterToAdd, Positions[3]);
			
			break;
		}
			
		case 1:
			ExampleWordAnswer = FlipLetters(ExampleWord);
			Answer1 = FlipLetters(TestWord);
			
			// we're going to be cruel, and flip these words then switch a letter so they look vaguely right at first glance
			Answer2 = FlipLetters(SwitchLetters(TestWord, Positions[0]));
			Answer3 = FlipLetters(SwitchLetters(TestWord, Positions[1]));
			Answer4 = FlipLetters(SwitchLetters(TestWord, Positions[2]));
			break;
			
		case 2:
			ExampleWordAnswer = RemoveLetters(ExampleWord, Positions[0]);
			Answer1 = RemoveLetters(TestWord, Positions[0]);
			Answer2 = RemoveLetters(TestWord, Positions[1]);
			Answer3 = RemoveLetters(TestWord, Positions[2]);
			Answer4 = RemoveLetters(TestWord, Positions[3]);
			break;
			
		case 3:
			ExampleWordAnswer = SwitchLetters(ExampleWord, Positions[0]);
			Answer1 = SwitchLetters(TestWord, Positions[0]);
			
			// to make this a bit harder, we're going to switch two letters in the others
			Answer2 = SwitchLetters(TestWord, Positions[0]);
			Answer2 = SwitchLetters(Answer2, Positions[1]);
			
			Answer3 = SwitchLetters(TestWord, Positions[0]);
			Answer3 = SwitchLetters(Answer3, Positions[2]);
			
			Answer4 = SwitchLetters(TestWord, Positions[0]);
			Answer4 = SwitchLetters(Answer4, Positions[3]);
			break;
	}
	
	Answers[0]->Correct = true;
	Answers[1]->Correct = false;
	Answers[2]->Correct = false;
	Answers[3]->Correct = false;
	
	TheGame->AllocString(&Answers[0]->Text, Answer1.c_str(), NORMAL, 264.0f, 47.0f, CENTRED);
	TheGame->AllocString(&Answers[1]->Text, Answer2.c_str(), NORMAL, 264.0f, 47.0f, CENTRED);
	TheGame->AllocString(&Answers[2]->Text, Answer3.c_str(), NORMAL, 264.0f, 47.0f, CENTRED);
	TheGame->AllocString(&Answers[3]->Text, Answer4.c_str(), NORMAL, 264.0f, 47.0f, CENTRED);
	
	Answers.Shuffle();
	
	Answers[0]->Pos = AnswerPositions[0];
	Answers[1]->Pos = AnswerPositions[1];
	Answers[2]->Pos = AnswerPositions[2];
	Answers[3]->Pos = AnswerPositions[3];
	
	ostringstream question;
	question << "If " << ExampleWord.c_str() << " is " << ExampleWordAnswer.c_str() << " then " << TestWord.c_str() << " is...";
	TheGame->AllocString(&TheQuestion, question.str().c_str(), LARGE, 264.0f, 135.0f, CENTRED);
}

void BPMiniGame_Scrambled::SetMarathon() {
	MarathonMode = true;
	GameHelp = "I'm going to scramble words and numbers by adding, removing and switching letters. Can you figure out what's changed?";
}
