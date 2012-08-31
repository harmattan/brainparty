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

#ifndef __BOMBHUNT_H__
#define __BOMBHUNT_H__

#include "Minigame.h"

class BPMiniGame_BombHunt: public BPMiniGame {
	
private:
	Texture* sfcBackground;
	
	BPPList<Texture*> Components;
	BPPList<Texture*> BombItems;
	
	Texture* Selected1;
	Texture* Selected2;
	Texture* Selected3;
	
	int SuccessTime;
	int LastGuessTime;
	
	int GuessesLeft;
	int LastNumCorrect;
	
	SpriteFont* sfcLastNumCorrect;
	SpriteFont* sfcGuessesLeft;
	
	int TimeStarted;
	
protected:
	void Start();
	int GetWeight();
	void Render();
	void Tick();
	
	virtual void OnMouseDown();
	virtual void OnMouseUp();
	virtual void OnMouseMove();
	
	void ClearForNextTry();
	void MouseClick(BPPoint e);
	void SelectItem(Texture* item);
	void CheckAnswer();
	
public:
	BPMiniGame_BombHunt(BPGame* game);
	~BPMiniGame_BombHunt();
};

#endif
