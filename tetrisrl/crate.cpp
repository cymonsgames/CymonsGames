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
#include <fstream>
#include "crate.h"

using namespace std;

	crate::crate(string temp)
	{
		type = temp;
		string filename = "data/" + type + ".tet";
		ifstream fin(filename.c_str());

		fin >> color;

		for(int i=0; i<4; i++)
		{
			shape.push_back(new map(4,4,0));

			for(int t=0; t<4; t++)
			for(int n=0; n<4; n++)
				fin >> shape[i]->data[t][n];
		}

		rotation=0;
	}
