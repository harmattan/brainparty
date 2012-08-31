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

#include "wordsmash.h"
#include "Minigame.h"

BPMiniGame_WordSmash::BPMiniGame_WordSmash(BPGame* game) : BPMiniGame(game) {
	TheGame = game;
	
	sfcBackgroundTop = TheGame->LoadBitmap("wordsmash_top", 320, 480);
	sfcBackgroundBottom = TheGame->LoadBitmap("wordsmash_bottom", 320, 480);
	
	SelectedRow = NULL;
	Locked = false; // when true disallow movement
	TimeStarted = LastColour = 0;
	SuccessTime = -1;
	
	GotMovement = JustMoved = false;
	
	DisappearTime = 250;
	
	sfcLetters["a"] = TheGame->LoadBitmap("a", 44, 44);
	sfcLetters["b"] = TheGame->LoadBitmap("b", 44, 44);
	sfcLetters["c"] = TheGame->LoadBitmap("c", 44, 44);
	sfcLetters["d"] = TheGame->LoadBitmap("d", 44, 44);
	sfcLetters["e"] = TheGame->LoadBitmap("e", 44, 44);
	sfcLetters["f"] = TheGame->LoadBitmap("f", 44, 44);
	sfcLetters["g"] = TheGame->LoadBitmap("g", 44, 44);
	sfcLetters["h"] = TheGame->LoadBitmap("h", 44, 44);
	sfcLetters["i"] = TheGame->LoadBitmap("i", 44, 44);
	sfcLetters["j"] = TheGame->LoadBitmap("j", 44, 44);
	sfcLetters["k"] = TheGame->LoadBitmap("k", 44, 44);
	sfcLetters["l"] = TheGame->LoadBitmap("l", 44, 44);
	sfcLetters["m"] = TheGame->LoadBitmap("m", 44, 44);
	sfcLetters["n"] = TheGame->LoadBitmap("n", 44, 44);
	sfcLetters["o"] = TheGame->LoadBitmap("o", 44, 44);
	sfcLetters["p"] = TheGame->LoadBitmap("p", 44, 44);
	sfcLetters["qu"] = TheGame->LoadBitmap("qu", 44, 44);
	sfcLetters["r"] = TheGame->LoadBitmap("r", 44, 44);
	sfcLetters["s"] = TheGame->LoadBitmap("s", 44, 44);
	sfcLetters["t"] = TheGame->LoadBitmap("t", 44, 44);
	sfcLetters["u"] = TheGame->LoadBitmap("u", 44, 44);
	sfcLetters["v"] = TheGame->LoadBitmap("v", 44, 44);
	sfcLetters["w"] = TheGame->LoadBitmap("w", 44, 44);
	sfcLetters["x"] = TheGame->LoadBitmap("x", 44, 44);
	sfcLetters["y"] = TheGame->LoadBitmap("y", 44, 44);
	sfcLetters["z"] = TheGame->LoadBitmap("z", 44, 44);
	
	Words = new WordList("/opt/brainparty/Content/wordlist.txt");
	
	for (int j = 0; j < RowCount; ++j) {
		vector<BPMiniGame_WordSmash_Letter*>* row = new vector<BPMiniGame_WordSmash_Letter*>();
		
		for (int i = 0; i < ColCount; ++i) {
			BPMiniGame_WordSmash_Letter* box = new BPMiniGame_WordSmash_Letter();
			box->X = LeftIndent + (i * BoxSize);
			box->Y = -BoxSize;
			box->DestY = 57 + (j * BoxSize);
			
			box->Letter = GetLetter();
			row->push_back(box);
		}
		
		Boxes.push_back(row);
	}
	
	GameTitle = "Word Smash";
	GameHelp = "Drag the rows around, then tap squares to make words of four letters or more. Double-tap the last letter when you want to submit it; if you change your mind, just tap the first letter again to clear it.";
	GameHelp2 = "You can spell words in any direction, even changing direction part way if you want to. For example, you could go down a letter, right a letter, down a letter, then right one more then tap the last letter again to submit it. You get more points for long words, so be creative!";
	
	MiniGameType = PUZZLE;
	
	Score = 0;
	sfcScoreStr = sfcClock = NULL;
	SetScore();
}

BPMiniGame_WordSmash::~BPMiniGame_WordSmash() {
	SAFE_DELETE(sfcBackgroundTop);
	SAFE_DELETE(sfcBackgroundBottom);
	
	BPMiniGame_WordSmash_Letter* box;
	
	for (int i = 0; i < Boxes.size(); ++i) {
		vector<BPMiniGame_WordSmash_Letter*>* row = Boxes[i];
		
		for (int i = row->size() - 1; i >= 0; --i) {
			box = (*row)[i];
			SAFE_DELETE(box);
		}
		
		row->clear();
		SAFE_DELETE(row);
	}
		
	Boxes.clear();
	
	for(map<string, Texture*>::iterator it = sfcLetters.begin(); it != sfcLetters.end(); ++it) {
		SAFE_DELETE(it->second);
	}
		
	sfcLetters.clear();
	
	SAFE_DELETE(Words);
	
	SAFE_DELETE(sfcScoreStr);
	SAFE_DELETE(sfcClock);
}

int BPMiniGame_WordSmash::GetWeight() {
	return Score / 100;
}

void BPMiniGame_WordSmash::OnMouseMove() {
	if (Locked) return;
	if (SelectedRow == NULL) return;
	
	if (!GotMovement) {
		Moves.Clear();
	}
	
	GotMovement = true;
	
	BPMiniGame_WordSmash_Letter* box;
	
	if (LastPos != BPPoint::Zero && LastPos != TouchEvent) {
		if (TouchEvent.X < LastPos.X) {
			// move left
			
			for (int i = SelectedRow->size() - 1; i >= 0; --i) {
				box = (*SelectedRow)[i];
				
				box->X += TouchEvent.X - LastPos.X;
				
				if (box->X <= -BoxSize) {
					box->X = GameWidth + box->X - LeftIndent - RightIndent;
					SelectedRow->erase(SelectedRow->begin() + i);
					SelectedRow->push_back(box);
				}
			}
			
		} else {
			// move right
			
			for (int i = 0; i < SelectedRow->size(); ++i) {
				box = (*SelectedRow)[i];
				
				box->X += TouchEvent.X - LastPos.X;
				
				if (box->X >= GameWidth - RightIndent) {
					box->X -= GameWidth - LeftIndent - RightIndent;
					SelectedRow->erase(SelectedRow->begin() + i);
					SelectedRow->insert(SelectedRow->begin() + 0, box);
				}
			}
		}
	}
	
	LastPos = TouchEvent;
}

void BPMiniGame_WordSmash::OnMouseDown() {
	if (Locked) return;
	GotMovement = false;
	
	LastPos = TouchEvent;
	
	for (int i = 0; i < Boxes.size(); ++i) {
		if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, 0, 37 + (i * BoxSize) + HalfBoxSize, 320, BoxSize)) {
			// move this row
			SelectedRow = Boxes[i];
			break;
		}
	}
}

void BPMiniGame_WordSmash::OnMouseUp() {
	if (Locked) return;
	if (SelectedRow == NULL) return;
	
	if (GotMovement) {
		
		BPMiniGame_WordSmash_Letter* box;
		
		// we've let go of a row - snap it into place!
		int xoffset = (*SelectedRow)[0]->X - LeftIndent;
		
		int remainder = xoffset % BoxSize;
		if (remainder != 0) {
			int adjust = 0;
			
			if (remainder > HalfBoxSize || (remainder < 0 && remainder > -HalfBoxSize)) {
				// snap to the right
				if (remainder > 0) {
					adjust = BoxSize - remainder;
				} else {
					adjust = abs(remainder);
				}
				
				for (int i = 0; i < SelectedRow->size(); ++i) {
					box = (*SelectedRow)[i];
					
					box->X += adjust;
					
					if (box->X >= GameWidth - RightIndent) {
						box->X -= GameWidth;
						SelectedRow->erase(SelectedRow->begin() + i);
						SelectedRow->insert(SelectedRow->begin() + 0, box);
					}
				}
			} else {
				// snap to the left
				if (remainder < 0) {
					adjust = abs(remainder) - BoxSize;
				} else {
					adjust = -remainder;
				}
				
				for (int i = SelectedRow->size() - 1; i >= 0; --i) {
					box = (*SelectedRow)[i];
					
					box->X += adjust;
					
					if (box->X <= LeftIndent - BoxSize) {
						box->X = GameWidth + box->X;
						SelectedRow->erase(SelectedRow->begin() + i);
						SelectedRow->push_back(box);
					}
				}
				
			}
		}
		
		SelectedRow = NULL;
	} else {
		// find the box that was clicked on
		BPList<BPMiniGame_WordSmash_Letter*> Possibles;
		BPMiniGame_WordSmash_Letter* box;
		
		for (int i = 0; i < Boxes.size(); ++i) {
			vector<BPMiniGame_WordSmash_Letter*>* row = Boxes[i];
			
			for (int j = row->size() - 1; j >= 0; --j) {
				box = (*row)[j];
				
				if (TheGame->PointOverRect(TouchEvent.X, TouchEvent.Y, box->X - HalfBoxSize, box->Y - HalfBoxSize, BoxSize * 2, BoxSize * 2)) {
					Possibles.Add(box);
				}
			}
		}
		
		box = NULL;
		int maxdist = 999;
		
		for (int i = 0; i < Possibles.Count; ++i) {
			int centrex = Possibles[i]->X + HalfBoxSize;
			int centrey = Possibles[i]->Y + HalfBoxSize;
			
			int distance = abs((int)round(TouchEvent.X - centrex)) + abs((int)round(TouchEvent.Y - centrey));
			if (distance < maxdist) {
				box = Possibles[i];
				maxdist = distance;
			}
		}
		
		// if we clicked on nothing, bail out
		if (box == NULL) return;
		
		if (Moves.Contains(box)) {
			if (Moves[Moves.Count - 1] == box) {
				// they selected the last square
				if (Moves.Count == 1) {
					// if this is the first square (ie, the only square), remove it
					Moves.Remove(box);
				} else {
					// otherwise, see if this matches a word!
					string check;
					
					for (int i = 0; i < Moves.Count; ++i) {
						check.append(Moves[i]->Letter);
					}
					
					if (IsWord(check)) {
						for (int i = 0; i < Moves.Count; ++i) {
							Moves[i]->MatchTime = TheGame->TickCount;
						}
						
						int ScoreAdd = (Moves.Count * Moves.Count) * 100;
						ModifyScore(ScoreAdd);
						TheGame->PlaySound("correct");
						++LastColour;
						
						if (LastColour > 3) LastColour = 0;
						
						Moves.Clear();
					}
					
				}
				
			} else {
				// they selected a square other than the last one - remove all moves after this one
				int pos = Moves.IndexOf(box);
				
				for (int i = Moves.Count - 1; i > pos; --i) {
					Moves.RemoveAt(i);
				}
				
				TheGame->PlaySound("wrong3");
			}
		} else {
			if (Moves.Count > 0) {
				BPMiniGame_WordSmash_Letter* last = Moves[Moves.Count - 1];
				
				if (CanMove(box, last)) {
					Moves.Add(box);
					TheGame->PlaySound("gem_select");
				} else {
					// illegal move!
					//	[MessageBox Show:"You can only move to a square that's horizontally or vertically next to your previous move.":"Oops!"];
				}
			} else {
				Moves.Add(box);
				TheGame->PlaySound("gem_select");
			}
		}
	}
}

bool BPMiniGame_WordSmash::CanMove(BPMiniGame_WordSmash_Letter* square1, BPMiniGame_WordSmash_Letter* square2) {
	// return true if this square is adjacent to the last move square
	
	return (abs(square1->X - square2->X) + abs(square1->Y - square2->Y)) == BoxSize;
}

void BPMiniGame_WordSmash::Start() {
	TimeStarted = TheGame->TickCount;
}


void BPMiniGame_WordSmash::Render() {
	sfcBackgroundBottom->Draw(0, -20);
	
	int TimePassed = TheGame->TickCount - TimeStarted;
	TimePassed = 300000 - TimePassed;
	
	if (TimePassed <= 0) {
		if (SuccessTime == -1) {
			SuccessTime = TheGame->TickCount;
			Success();
		}
	}
	
	if (sfcClock == NULL || RedrawClock()) {
		TheGame->AllocString(&sfcClock, TheGame->TicksToTime(TimePassed)->c_str(), LARGE, 80, 47, LEFT);
	}
	
	TheGame->DrawString(sfcClock, WHITE, 235, 10);
	
	TheGame->DrawString(sfcScoreStr, WHITE, 15, 10);
	
	BPMiniGame_WordSmash_Letter* box;
	
	for (int i = 0; i < Boxes.size(); ++i) {
		vector<BPMiniGame_WordSmash_Letter*>* row = Boxes[i];
		
		for (int j = row->size() - 1; j >= 0; --j) {
			box = (*row)[j];
			
			// HACK: There's an unknown bug in this minigame that occasionally puts boxes in the wrong X pos.
			// To reproduce, just keep matching stuff while dragging around on the screen manically.
			// This force-resets the X position for each box to the correct value. What a waste of CPU time!
			if (SelectedRow != row) box->X = LeftIndent + (j * BoxSize);
			
			if (box->MatchTime == -1) {
				if (Moves.Contains(box)) {
					switch (LastColour) {
						case 0:
							sfcLetters[box->Letter]->Draw(box->X + HalfBoxSize, box->Y + HalfBoxSize, 0.0f, 1.0f, (*TheGame->LightRed));
							break;
							
						case 1:
							sfcLetters[box->Letter]->Draw(box->X + HalfBoxSize, box->Y + HalfBoxSize, 0.0f, 1.0f, (*TheGame->Yellow));
							break;
							
						case 2:
							sfcLetters[box->Letter]->Draw(box->X + HalfBoxSize, box->Y + HalfBoxSize, 0.0f, 1.0f, (*TheGame->Green));
							break;
							
						case 3:
							sfcLetters[box->Letter]->Draw(box->X + HalfBoxSize, box->Y + HalfBoxSize, 0.0f, 1.0f, (*TheGame->Cyan));
							break;
					}
				} else {
					sfcLetters[box->Letter]->Draw(box->X + HalfBoxSize, box->Y + HalfBoxSize, 0.0f, 1.0f, (*TheGame->White));
					
					if (box->X < LeftIndent) {
						sfcLetters[box->Letter]->Draw(GameWidth + box->X + HalfBoxSize - LeftIndent - RightIndent, box->Y + HalfBoxSize, 0.0f, 1.0f, (*TheGame->White));
					} else if (box->X > GameWidth - BoxSize - RightIndent) {
						sfcLetters[box->Letter]->Draw(HalfBoxSize + LeftIndent + RightIndent + box->X - GameWidth, box->Y + HalfBoxSize, 0.0f, 1.0f, (*TheGame->White));
					}
				}
			} else {
				float diff = TheGame->TickCount - box->MatchTime;
				
				if (diff <= DisappearTime) {
					float step = diff / DisappearTime; // get a value between 0 and 1
					Colour col = Colour(1.0f, 1.0f, 1.0f, 1 - step);
					
					sfcLetters[box->Letter]->Draw(box->X + HalfBoxSize, box->Y + HalfBoxSize, 0.0f, TheGame->SmoothStep(1.0f, 3.0f, step), col);
				}
			}
		}
	}
	
	// draw the background last to mask the hidden letters
	sfcBackgroundTop->Draw(0, -20);
}

void BPMiniGame_WordSmash::Tick() {
	vector<BPMiniGame_WordSmash_Letter*>* row;
	BPMiniGame_WordSmash_Letter* box;
	BPMiniGame_WordSmash_Letter* copybox;
	
	int MatchTimeout = TheGame->TickCount - DisappearTime;
	
	bool AllStill = true;
	
	for (int i = Boxes.size() - 1; i >= 0; --i) {
		row = Boxes[i];
		
		for (int j = row->size() - 1; j >= 0; --j) {
			box = (*row)[j];
			
			if (box->MatchTime != -1) AllStill = false;
			
			if (box->Y != box->DestY) {
				AllStill = false;
				
				box->YSpeed += 2.0f;
				box->Y += box->YSpeed;
				if (box->Y > box->DestY) {
					box->Y = box->DestY;
					box->YSpeed = 0;
				}
			}
			
			if (box->MatchTime != -1 && box->MatchTime < MatchTimeout) {
				row->erase(row->begin() + j);
				// move all boxes down above me
				
				for (int k = i - 1; k >= 0; --k) {
					copybox = (*Boxes[k])[j];
					Boxes[k]->erase(Boxes[k]->begin() + j);
					Boxes[k + 1]->insert(Boxes[k + 1]->begin() + j, copybox);
					copybox->DestY = 57 + ((k + 1) * BoxSize);
				}
				
				BPMiniGame_WordSmash_Letter* newbox = new BPMiniGame_WordSmash_Letter();
				newbox->X = LeftIndent + j * BoxSize;
				newbox->Y = -BoxSize;
				newbox->DestY = 57;
				newbox->Letter = GetLetter();
				Boxes[0]->insert(Boxes[0]->begin() + j, newbox);
			}
		}
	}
	
	if (AllStill) {
		Locked = false;
		JustMoved = false;
	} else {
		Locked = true;
	}
}

bool BPMiniGame_WordSmash::IsWord(string &word) {
	return Words->Contains(word);
}


string BPMiniGame_WordSmash::GetLetter() {
	int freq_a = 8167;
	int freq_b = freq_a + 1492;
	int freq_c = freq_b + 2782;
	int freq_d = freq_c + 4253;
	int freq_e = freq_d + 12702;
	int freq_f = freq_e + 2228;
	int freq_g = freq_f + 2015;
	int freq_h = freq_g + 6094;
	int freq_i = freq_h + 6966;
	int freq_j = freq_i + 153;
	int freq_k = freq_j + 772;
	int freq_l = freq_k + 4025;
	int freq_m = freq_l + 2406;
	int freq_n = freq_m + 6749;
	int freq_o = freq_n + 7507;
	int freq_p = freq_o + 1929;
	int freq_q = freq_p + 95;
	int freq_r = freq_q + 5987;
	int freq_s = freq_r + 6327;
	int freq_t = freq_s + 9056;
	int freq_u = freq_t + 2758;
	int freq_v = freq_u + 978;
	int freq_w = freq_v + 2360;
	int freq_x = freq_w + 150;
	int freq_y = freq_x + 1974;
	//	int freq_z = freq_y + 74;
	
	int random = TheGame->RandomRange(0, 100000);
	
	if (random < freq_a) {
		return string("a");
	} else if (random < freq_b) {
		return string("b");
	} else if (random < freq_c) {
		return string("c");
	} else if (random < freq_d) {
		return string("d");
	} else if (random < freq_e) {
		return string("e");
	} else if (random < freq_f) {
		return string("f");
	} else if (random < freq_g) {
		return string("g");
	} else if (random < freq_h) {
		return string("h");
	} else if (random < freq_i) {
		return string("i");
	} else if (random < freq_j) {
		return string("j");
	} else if (random < freq_k) {
		return string("k");
	} else if (random < freq_l) {
		return string("l");
	} else if (random < freq_m) {
		return string("m");
	} else if (random < freq_n) {
		return string("n");
	} else if (random < freq_o) {
		return string("o");
	} else if (random < freq_p) {
		return string("p");
	} else if (random < freq_q) {
		return string("qu");
	} else if (random < freq_r) {
		return string("r");
	} else if (random < freq_s) {
		return string("s");
	} else if (random < freq_t) {
		return string("t");
	} else if (random < freq_u) {
		return string("u");
	} else if (random < freq_v) {
		return string("v");
	} else if (random < freq_w) {
		return string("w");
	} else if (random < freq_x) {
		return string("x");
	} else if (random < freq_y) {
		return string("y");
	} else {
		return string("z");
	}
	
	return string("");
}

void BPMiniGame_WordSmash::ModifyScore(int adjust) {
	Score += adjust;
	
	if (Score < 0) Score = 0;
	
	SetScore();
}

void BPMiniGame_WordSmash::SetScore() {
	if (Score > 0) {
		ostringstream score;
		score << "Score: " << TheGame->SeparateThousands(Score);
		TheGame->AllocString(&sfcScoreStr, score.str().c_str(), LARGE, 250, 50, LEFT);
	} else {
		TheGame->AllocString(&sfcScoreStr, "Score: 0", LARGE, 250, 50, LEFT);
	}
}
