/* Copyright (C) 2009 John L. Greco.

   This file is part of TetRLs.

   TetRLs is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 2 of the License, or
   (at your option) any later version.

   TetRLs is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with TetRLs.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __MAP_
#define __MAP_

#include <vector>

using namespace std;

class map
{
	public:
	vector<vector<short int> > data;
	unsigned int height, width;

	map(unsigned int new_h, unsigned int new_w, int fill);

	void resize(int new_h, int new_w);
	void fill(int fill);
};

#endif
