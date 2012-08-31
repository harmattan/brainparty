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

#ifndef __SYMBOLICLOGIC_H__
#define __SYMBOLICLOGIC_H__

#include "Minigame.h"

enum PremiseTypes { ALL_JOB, NONE_JOB, ALL_ITEM, NONE_ITEM, PERSON };

class BPMiniGame_SymbolicLogic_Item {
public:
	string SingularDesc;
	string Negative;
	string PluralDesc;
	string Noun;
	
	BPMiniGame_SymbolicLogic_Item(const char* singular, const char* negative, const char* plural, const char* noun) {
		SingularDesc = string(singular);
		Negative = string(negative);
		PluralDesc = string(plural);
		Noun = string(noun);
	}
	
	~BPMiniGame_SymbolicLogic_Item() {

	}
};

class BPMiniGame_SymbolicLogic_Premise {
public:
	PremiseTypes Type;
	BPMiniGame_SymbolicLogic_Item* Item;
	BPMiniGame_SymbolicLogic_Item* Item2; // for example, "all cat owners own a dog"
	string Job;
	string Text;
	
	BPMiniGame_SymbolicLogic_Premise() {
		Item = Item2 = NULL;
	}
	
	~BPMiniGame_SymbolicLogic_Premise() {
//		if (Text != NULL) {
//			SAFE_DELETE(Text);
//		}
	}
};

class BPMiniGame_SymbolicLogic : public BPMiniGame {
public:
	BPMiniGame_SymbolicLogic(BPGame* game);
	~BPMiniGame_SymbolicLogic();
	void Start();
	int GetWeight();
	void Render();
	const char* FlattenPremise(BPMiniGame_SymbolicLogic_Premise* premise);
	void Tick();
	
	void OnMouseDown();
	void OnMouseMove();
	void OnMouseUp();
	void LevelUp();
	PremiseTypes GetPremiseType();
	string GetRandomJob();
	void SetMarathon();
		
protected:
	Texture* sfcBackground;
	
	MiniGameStates GameState;
	
	int LastStateChange;
	int CurrentLevel;
	int NumTries;
	int Score;
	
	BPPList<BPMiniGame_SymbolicLogic_Item*> LogicItems;
	BPPList<BPMiniGame_SymbolicLogic_Premise*> Premises;
	
	string* Conclusion;
	bool AnswerIsTrue;
	
	SpriteFont* sfcText;
	
	int TimeStarted;
};

#endif
