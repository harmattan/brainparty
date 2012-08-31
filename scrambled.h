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

#ifndef __SCRAMBLED_H__
#define __SCRAMBLED_H__

#include "Minigame.h"

class BPMiniGame_Scrambled_Answer {
public:
	SpriteFont* Text;
	bool Correct;
	
	BPPoint Pos;
	
	BPMiniGame_Scrambled_Answer() {
		Text = NULL;
	}
	
	~BPMiniGame_Scrambled_Answer() {
		SAFE_DELETE(Text);
	}
};

class BPMiniGame_Scrambled : public BPMiniGame {
	BPList<string> Words;
	BPList<string> Numbers; // we use a list so each generated number is made up only of unique digits (eg 5335 is not allowed)
	BPList<string> Letters;
	BPList<int> Positions; // used to remove or switch random (but unique) individual letters/numbers
	BPPList<BPMiniGame_Scrambled_Answer*> Answers;
	BPList<BPPoint> AnswerPositions;
	
	SpriteFont* TheQuestion;
	
	int AnswersYOffset;
	
	int CurrentLevel;
	int Score;
	
	int RandSeed;
	
	string ExampleWord;
	string ExampleWordAnswer;
	string TestWord;
	
	string Answer1;
	string Answer2;
	string Answer3;
	string Answer4;
	
	Texture* sfcAnswer1;
	Texture* sfcAnswer2;
	Texture* sfcAnswer3;
	Texture* sfcAnswer4;
	
	MiniGameStates GameState;
	int LastStateChange;
	
	Texture* sfcBackground;
	
	int SelectedItem;
	
	int TimeStarted;
	
public:
	BPMiniGame_Scrambled(BPGame* game);
	~BPMiniGame_Scrambled();
	void Start();
	int GetWeight();
	void Render();
	string FlipLetters(string Word);
	string RemoveLetters(string Word, int Pos);
	string SwitchLetters(string Word, int Pos);
	string AddLetters(string Word, string Letter, int Pos);
	string GetUniqueNumber(string Word1, string Word2);
	string GetUniqueLetter(string Word1, string Word2);
	void Tick();
	void OnMouseDown();
	void OnMouseMove();
	void OnMouseUp();
	void LevelUp();
	void SetMarathon();
};

#endif
