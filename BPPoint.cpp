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

#include "BPPoint.h"

BPPoint::BPPoint() {
	X = 0;
	Y = 0;
}

BPPoint::BPPoint(float x, float y) {
	X = x;
	Y = y;
}

bool BPPoint::operator!= (BPPoint to) {
	return this->X != to.X || this->Y != to.Y;
}

float BPPoint::Distance(BPPoint from, BPPoint to) {
	return sqrt(DistanceSquared(from, to));
}

float BPPoint::DistanceSquared(BPPoint from, BPPoint to) {
	return (from.X - to.X) * (from.X - to.X) + (from.Y - to.Y) * (from.Y - to.Y);
}

BPPoint BPPoint::Zero = BPPoint();
