
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


#include "Minigame.h"
#include "strangerdanger.h"

BPMiniGame_StrangerDanger::BPMiniGame_StrangerDanger(BPGame* game) : BPMiniGame(game) {
	Level = NumWrong = TimeStarted = NumTries = 0;
	
	GameTitle = "Stranger Danger";
	GameHelp = "Remember the items on the screen, because when you tap the screen I'll hide them all and add one new item. Can you figure out which item is new?";
	GameHelp2 = "You need to remember all the different shapes you see, then tap on the one that wasn't there before. Remember: the colours change as well, so look closely!";
	
	MiniGameType = PUZZLE;
	
	GameState = REMEMBER;
	LastStateChange = 0;
	
	FlipBackground = false;
	AnimPos = 0.0f;
	
	sfcBackground = TheGame->LoadBitmap("strangerdanger", 320, 480);
	sfcRemember = TheGame->LoadBitmap("strangerdanger_remember", 320, 480);
	sfcFlash = TheGame->LoadBitmap("lensflare", 512, 512);
	sfcWhatsChanged = TheGame->LoadBitmap("whichisstranger", 512, 512);
	
	for (int i = 1; i <= 59; ++i) {
		ostringstream filename;
		filename << "patch" << i;
		CardTypes.Add(TheGame->LoadBitmap(filename.str().c_str(), 48, 48));
	}
	
	for (int i = 0; i < 6; ++i) {
		for (int j = 3; j < 8; ++j) {
			Coordinates.Add(new BPPoint(10 + (i * 50), -30 + (j * 50)));
		}
	}
}

BPMiniGame_StrangerDanger::~BPMiniGame_StrangerDanger() {
	SAFE_DELETE(sfcBackground);
	SAFE_DELETE(sfcRemember);
	SAFE_DELETE(sfcFlash);
	SAFE_DELETE(sfcWhatsChanged);
	
	CardTypes.Clear();
	Coordinates.Clear();
	Items.Clear();
}

void BPMiniGame_StrangerDanger::Start() {
	LevelUp();
	CreateLevel();
	
	TimeStarted = TheGame->TickCount;
}

int BPMiniGame_StrangerDanger::GetWeight() {
	float TimePassed = (TheGame->TickCount - TimeStarted) / 1000.0f;
	return MinMax(600 - (NumWrong * 75) - round(TimePassed) / 4);
}

void BPMiniGame_StrangerDanger::Render() {
	switch (GameState) {
		case REMEMBER:
		case GUESSING:
		case CORRECT:
		case WRONG:
			RenderItems();
			break;
			
		case FADE_IN:
			RenderItems();
			RenderGlow();			
			break;
			
		case FADE_OUT:
			RenderItems();
			RenderWhiteFade();
			break;
			
		case SHOWING:
			TheGame->FillRectangle((*TheGame->White), 0, 0, 320, 480);
			
			Colour* col;
			float scale;
			
			if (AnimPos < 0.333f) {
				col = TheGame->ColorLerp(TheGame->TransparentWhite, TheGame->White, AnimPos * 3);
				scale = TheGame->SmoothStep(4.0f, 0.6f, AnimPos * 3);
			} else {
				col = new Colour(1.0f, 1.0f, 1.0f, 1.0f);
				scale = TheGame->SmoothStep(0.6f, 0.5f, AnimPos - 0.333f);
			}
			
			sfcWhatsChanged->Draw(160, 208, 0.0f, scale, (*col));
			SAFE_DELETE(col);
			
			break;
	}
}

void BPMiniGame_StrangerDanger::Tick() {
	if (AnimPos != 1.0f) {
		switch (GameState) {
			case SHOWING:
				AnimPos += 0.5f * TheGame->ElapsedSeconds;
				break;
				
			default:
				AnimPos += 1.5f * TheGame->ElapsedSeconds;
		}
		
		if (AnimPos >= 1.0f) {
			AnimPos = 1.0f;
			
			// finished animating - now which state should we switch to?
			switch (GameState) {
				case FADE_IN:
					FlipBackground = !FlipBackground;
					
					if (FlipBackground) {
						SetGameState(SHOWING);
					} else {
						SetGameState(FADE_OUT);
						CreateLevel();
					}
					break;
					
				case SHOWING:
				{
					SetGameState(FADE_OUT);
					
					Coordinates.Shuffle();
					
					// move all the items
					for (int i = 0; i < Items.Count; ++i) {
						BPMiniGame_StrangerDanger_Item* item = Items[i];
						item->X = Coordinates[i]->X;
						item->Y = Coordinates[i]->Y;
					}
					
					// now add a new one
					BPMiniGame_StrangerDanger_Item* item = new BPMiniGame_StrangerDanger_Item();
					item->X = Coordinates[Coordinates.Count - 1]->X;
					item->Y = Coordinates[Coordinates.Count - 1]->Y;
					item->CardType = CardTypes.Count - 1;
					Items.Add(item);
					
					break;
				}
					
				case FADE_OUT:					
					if (FlipBackground) {
						SetGameState(GUESSING);
					} else {
						SetGameState(REMEMBER);
					}
					break;
			}
		}
	}
	
	if (GameState == CORRECT || GameState == WRONG) {
		if (LastStateChange + 500 < TheGame->TickCount) {
			if (GameState == CORRECT) {
				LevelUp();
			} else {
				LevelDown();
			}
			
			if (NumTries > 8) {
				Success();
			} else {			
				SetGameState(FADE_IN);
			}
		}
	}
}

void BPMiniGame_StrangerDanger::OnMouseDown() {
	
}

void BPMiniGame_StrangerDanger::OnMouseMove() {
	
}

void BPMiniGame_StrangerDanger::OnMouseUp() {
	if (GameState == REMEMBER) {
		SetGameState(FADE_IN);
	} else if (GameState == GUESSING) {	
		for (int i = 0; i < Items.Count; ++i) {
			BPMiniGame_StrangerDanger_Item* item = Items[i];
			
			if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, item->X, item->Y, 48, 48)) {
				// clicked this item!
				if (i == Items.Count - 1) {
					TheGame->PlaySound("correct");
					SetGameState(CORRECT);
				} else {
					TheGame->PlaySound("wrong");
					SetGameState(WRONG);
				}
				
				break;
			}
		}
	}
}

void BPMiniGame_StrangerDanger::LevelUp() {
	++Level;
	++NumTries;
}

void BPMiniGame_StrangerDanger::LevelDown() {
	--Level;
	++NumTries;
	if (Level < 1) Level = 1;
	++NumWrong;
}

void BPMiniGame_StrangerDanger::CreateLevel() {	
	if (NumTries > 8) {
		Success();
	}

	Items.Clear();
	CardTypes.Shuffle();
	Coordinates.Shuffle();
	
	int num_items = 1 + Level;
	
	for (int i = 0; i < num_items; ++i) {
		BPMiniGame_StrangerDanger_Item* item = new BPMiniGame_StrangerDanger_Item();
		item->X = Coordinates[i]->X;
		item->Y = Coordinates[i]->Y;
		item->CardType = i;
		Items.Add(item);
	}
}

void BPMiniGame_StrangerDanger::RenderItems() {
	switch (GameState) {
		case FADE_IN:
		case REMEMBER:
			if (FlipBackground) {
				TheGame->DrawImage(sfcBackground, 0, 0);
			} else {
				TheGame->DrawImage(sfcRemember, 0, 0);				
			}
			
			break;

			
		case FADE_OUT:
		case GUESSING:
		case CORRECT:
		case WRONG:
			if (FlipBackground) {
				TheGame->DrawImage(sfcBackground, 0, 0);
			} else {
				TheGame->DrawImage(sfcRemember, 0, 0);				
			}
			break;
	}
	
	for (int i = 0; i < Items.Count; ++i) {
		BPMiniGame_StrangerDanger_Item* item = Items[i];
		TheGame->DrawImage(CardTypes[item->CardType], item->X, item->Y);
	}
	
	if (GameState == CORRECT) {
		RenderCorrect();
	} else if (GameState == WRONG) {
		RenderWrong();
	}
}

void BPMiniGame_StrangerDanger::RenderGlow() {
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	sfcFlash->Draw(160, 208, 0.0f, AnimPos * 4, (*TheGame->White));
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
}

void BPMiniGame_StrangerDanger::RenderWhiteFade() {
	Colour col = Colour(1.0f, 1.0f, 1.0f, 1 - AnimPos);
	TheGame->FillRectangle(col, 0, 0, 320, 480);
}

void BPMiniGame_StrangerDanger::SetGameState(MiniGameStates state) {
	GameState = state;
	AnimPos = 0.0f;
	LastStateChange = TheGame->TickCount;
}
