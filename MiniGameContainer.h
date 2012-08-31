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

#ifndef __BPMINIGAME_H__
#define __BPMINIGAME_H__

class BPMiniGame_Container {
public:
	int GameCode;
	const char* ShortName;
	const char* FriendlyName;
	Texture* Pic;
	bool IsShowing; // is this currently visible on the practise screen?
	bool IsSecret;
	bool Unlocked;
	
	float X;
	float Y;
	
	float StartX;
	float StartY;
	
	float DestX;
	float DestY;
	
	BPMiniGame_Container(int gamecode, const char* shortname, const char* friendlyname, Texture* pic, bool issecret) {
		ShortName = shortname;
		FriendlyName = friendlyname;
		Pic = pic;
		GameCode = gamecode;
		IsSecret = issecret;
		Unlocked = false;
	}
};

#endif
