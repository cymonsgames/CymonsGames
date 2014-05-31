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

#include <vector>
#include "map.h"

using namespace std;

	map::map(unsigned int new_h, unsigned int new_w, int fill)
	{
		vector<short int> blarg;

		for(unsigned int n=0; n<new_w; n++)
			blarg.push_back(fill);
		for(unsigned int n=0; n<new_h; n++)
			data.push_back(blarg);

		height = new_h;
		width = new_w;

		return;
	}

	void map::fill(int fill)
	{
		for(unsigned int n=0; n<height; n++)
			for(unsigned int i=0; i<width; i++)
				data[n][i] = fill;

		return;
	}
