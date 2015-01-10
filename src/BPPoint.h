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

#ifndef __BPPoint_H__
#define __BPPoint_H__

#include "math.h"

class BPPoint {
public:
	float X;
	float Y;
	
	static BPPoint Zero;
		
	BPPoint();
	BPPoint(float x, float y);
	
	static float Distance(BPPoint from, BPPoint to);
	static float DistanceSquared(BPPoint from, BPPoint to);
	
	bool operator!= (BPPoint to);
};

#endif
