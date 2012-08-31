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

#ifndef __WORDSMASH_H__
#define __WORDSMASH_H__

#include "Minigame.h"
#include "WordList.h"

class BPMiniGame_WordSmash_Letter  {
public:
	int X;
	int Y;
	int DestY;
	int YSpeed;
	int MatchTime;
	string Letter;
	
	BPMiniGame_WordSmash_Letter() {
		Letter = string("A");
		X = Y = DestY = YSpeed = 0;
		MatchTime = -1;
	}
};


class BPMiniGame_WordSmash : public BPMiniGame {
public:
	BPMiniGame_WordSmash(BPGame* game);
	~BPMiniGame_WordSmash();
	void OnMouseMove();
	void OnMouseDown();
	void OnMouseUp();
	void Start();
	void Render();
	void Tick();
	int GetWeight();
	void CheckForMatches();
	bool IsWord(string &word);
	string GetLetter();
	bool CanMove(BPMiniGame_WordSmash_Letter* square1, BPMiniGame_WordSmash_Letter* square2);
	void ModifyScore(int adjust);
	void SetScore();
	
protected:	
	static const int GameWidth = 320;
	static const int GameHeight = 480;
	
	static const int RowCount = 8;
	static const int ColCount = 7;
	
	static const int MinWordLength = 4;
	
	static const int LeftIndent = 6;
	static const int RightIndent = 6;
	
	WordList* Words;
	
	int LastColour;
	
	Texture* sfcBackgroundTop;
	Texture* sfcBackgroundBottom;
	
	vector<vector<BPMiniGame_WordSmash_Letter*>*> Boxes;
	BPList<BPMiniGame_WordSmash_Letter*> Moves; // this needs to be a BPList to avoid double-freeing boxes in Boxes
	map<string, Texture*> sfcLetters;
	
	vector<BPMiniGame_WordSmash_Letter*>* SelectedRow;
	
	bool GotMovement; // used to track whether a drag took place
	bool JustMoved; // used to track whether to check for words when dragging has finished
	
	int DisappearTime;
	BPPoint LastPos;
	
	int Score;
	SpriteFont* sfcScoreStr;
	
	SpriteFont* sfcClock;
	
	static const int BoxSize = 44;
	static const int HalfBoxSize = 22;
	
	bool Locked; // when true disallow movement
	
	int TimeStarted;
	
	int SuccessTime;
};

#endif
