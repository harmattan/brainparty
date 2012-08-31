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

#ifndef __MINIGAME_H__
#define __MINIGAME_H__

#include <vector>
#include <algorithm>
#include <map>
#include <string>

#include "BPGame.h"
#include "BPPoint.h"
#include "SpriteFont.h"
#include "BPList.h"

enum RankTypes { NONE, FAIL, BRONZE, SILVER, GOLD, PLATINUM }; // "NONE" is required, otherwise the default is "FAIL"
enum MiniGameStates { SHOWING, GUESSING, PAUSING, THINKING, WAITING, CHANGING, MOVES, MOVING, REMEMBER, CORRECT, WRONG, SUCCESS, FAILURE, FADE_IN, FADE_OUT };
enum MiniGameTypes { PUZZLE, LIVELY, ACTION };
enum ReturnTypes { RT_NORMAL, RT_SECRET, RT_BRAINBOOST };

class BPGame;
class SpriteFont;


class BPMiniGame_BGStar {
public:
	int Type;
	float Speed;
	BPPoint Pos;
};



class BPMiniGame {
public:
	ReturnTypes ReturnType; // controls which game screens return to y
	int FinishedTime; // set to Environment.TickCount when game should no longer have Tick() called
	const char* GameTitle;
	const char* GameHelp;
	const char* GameHelp2;
	
	SpriteFont* sfcGameTitle;
	SpriteFont* sfcGameHelp;
	MiniGameTypes MiniGameType;
	
		
	int FinalWeight;
	RankTypes FinalRank;
	const char* FinalGrade;
	
	SpriteFont* sfcFinalWeight;
	SpriteFont* sfcFinalGrade;
	
	BPPoint TouchEvent;
	
	BPGame* TheGame;

	BPMiniGame(BPGame* game);
	virtual ~BPMiniGame();
	void Init(); // called after the constructor so that any descendent constructors have finished
	virtual void Start() = 0;
	virtual int GetWeight() = 0;
	virtual void SetMarathon();
	static RankTypes GetRank(int Weight);
	const char* GetGrade(int Weight);
	void TickMiniGame();
	void RenderMiniGame();
	void RenderScore();
	
	virtual void OnMouseDown() = 0;
	virtual void OnMouseUp() = 0;
	virtual void OnMouseMove() = 0;
	
	void HandleMouseUp(BPPoint e);
	void HandleMouseDown(BPPoint e);
	void HandleMouseMove(BPPoint e);
	
	void DrawProfessorText();
	void PlayMusic();
	
protected:
	static const int MiniGameWidth = 320;
	static const int MiniGameHalfWidth = 160;
	static const int MiniGameHeight   = 480; // note: needs to be screen height minus the height of the info bar
	static const int MiniGameHalfHeight = 240;

	bool RedrawClock();
	
	virtual void Render() = 0;
	virtual void Tick() = 0;
	
	void Success();
	void Failure();
	void CalculateResult();
	void ContinueGame();
	void GoBack();
	void ShowHelp();
	int DivRem(int Num, int Div, int &Rem);
	int Round(double num);
	int MinMax(int num);
	void RenderCorrect();
	void RenderWrong();
	void GenerateStarfield(BPPList<BPMiniGame_BGStar*> &list);
	void UpdateStarfield(BPPList<BPMiniGame_BGStar*> &list);
	void DrawStarfield(BPPList<BPMiniGame_BGStar*> &list);

	bool MarathonMode; // used by some games to hide/stop the timer so players can play as long as they want to

private:
	bool InTick;
	bool BackDown;
	bool HelpDown;
	int WantToQuit;
};

#endif
