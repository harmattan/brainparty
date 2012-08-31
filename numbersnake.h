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

#ifndef __NUMBERSNAKE_H__
#define __NUMBERSNAKE_H__

#include "Minigame.h"

class BPMiniGame_NumberSnake : public BPMiniGame {
public:
	BPMiniGame_NumberSnake(BPGame* game);
	~BPMiniGame_NumberSnake();
	void OnMouseUp();
	void OnMouseMove();
	void OnMouseDown();
	void Start();
	int GetWeight();
	void Render();
	void Tick();
	void SubmitAnswer(int answer);
	void LevelUp();
	void SetMarathon();
protected:
	Texture* sfcBackground;
	Texture* sfcBlack;
	int CurrentLevel;
	MiniGameStates GameState;
	
	BPList<const char*> NumStrings;
	
	int LastStateChange;
	int LastQuestionChange;
	int NumCorrect;
	float Fader;
	
	SpriteFont** sfcQuestionParts;
	int QuestionLength;
	int Answer;
	
	int QuestionPos;
	int QuestionSpeed;
	
	static const int HalfHeight;
	
	int TimeStarted;
};

#endif
