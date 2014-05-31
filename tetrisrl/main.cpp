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


#include "curses.h"
#include <vector>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

#include "map.h"
#include "crate.h"

#include "tetrisrl.h"

using namespace std;

void move_player(int direction);
void load_new_level();
bool is_crate_there(int x, int y);
bool push_crate(int direction);
bool attack(int x, int y);
void plant_piece();
void run_rats();
void create_piece();
void rotate_recenter();
void redraw();
void victory();

struct monster
{
	int x, y;
	int hp;
};

enum direction { UP, RIGHT, DOWN, LEFT , UPLEFT, UPRIGHT, DOWNLEFT, DOWNRIGHT } ;

unsigned int height, width;
map* current_map;
monster* player;
crate* current_crate;
int placed;
int target;
int filled_lines;
int xoffset;
int level_count;

vector<monster* > monsters;

vector<string > message;

int main()
{
	srand(time(NULL));

	initscr();
	raw();
	keypad(stdscr, TRUE);
	noecho();
	curs_set(0);
	start_color();
	use_default_colors();

	init_pair(1, -1, COLOR_RED);
	init_pair(2, -1, COLOR_WHITE);
	init_pair(3, -1, COLOR_MAGENTA);
	init_pair(4, -1, COLOR_BLUE);
	init_pair(5, -1, COLOR_GREEN);
	init_pair(6, -1, COLOR_YELLOW);
	init_pair(7, -1, COLOR_CYAN);

	target = 0;
	level_count = 0;
	xoffset = -11;
	player = new monster;
	load_new_level();
	player->hp = 10;

	mvprintw(0,0,"Ah ha!  I see you have finally awaken comrade!\n" \
		"Now we can begin ze experiment!\n\n" \
		"You will find next to you a nice hefty wrench and\n" \
		"a remote control device.  Your task is quite simple,\n" \
		"you must push a certain number of oddly shaped crates\n" \
		"into each room.  Once you have positioned a crate to\n" \
		"your liking you may signal me to drop another(D).\n" \
		"However, once you do zis you may no longer move any\n" \
		"previous crates.\n\nAdditionally, if you wish to have a crate rotated\n");
	mvprintw(12,0,"and reset to its starting location you can use your remote\n" \
		"to signal me(R).\n\n\nOh, and one last zing before we begin comrade!\n" \
		"A significate population of rats seems to have recently\n" \
		"escaped from my lab, if you run into any give zem a good\n" \
		"wack with ze wrench I left with you!  If you happen\n" \
		"to be injured I will dispense a medpack to you whenever\n" \
		"I drop a new crate.");
	refresh();
	getchar();
	erase();

	message.push_back("Now, lets begin! ");

	int key;
	redraw();
	refresh();

	while(key != 'Q')
	{
		if(player->hp <= 0)
			break;

		key = getch();

		switch(key)
		{
			case 'h': case 'a': case '4': case KEY_LEFT:
				move_player(LEFT);
				break;
			case 'l': case 'd': case '6': case KEY_RIGHT:
				move_player(RIGHT);
				break;
			case 'k': case 'w': case '8': case KEY_UP:
				move_player(UP);
				break;
			case 'j': case 's': case '2': case KEY_DOWN:
				move_player(DOWN);
				break;

			case 'y': case 'q': case '7':
				move_player(UPLEFT);
				break;
			case 'u': case 'e': case '9':
				move_player(UPRIGHT);
				break;
			case 'b': case 'z': case '1':
				move_player(DOWNLEFT);
				break;
			case 'n': case 'c': case '3':
				move_player(DOWNRIGHT);
				break;

			case '.': case '5':
				break;

			case 'D': case ' ':
				if(player->x >= 4 && player->x <= 7 && player->y >= 2 && player->y <= 5)
					message.push_back("Move out of ze way comrade! ");
				else
					plant_piece();
				break;
			case 'R': case 'x':
				if(player->x >= 4 && player->x <= 7 && player->y >= 2 && player->y <= 5)
					message.push_back("Move out of ze way comrade! ");
				else
					rotate_recenter();
				break;

			default:
				continue;
		}

		redraw();

		run_rats();
		if(rand()%1500 <= player->hp + target)
		{
			for(int i=0; i<20; i++)
			{
				int x = rand()%10 +1;
				int y = rand()%16 +5;

				if(current_map->data[y][x] == FLOOR && !is_crate_there(x,y) && (x != player->x && y != player->y))
				{
					monsters.push_back(new monster);
					monsters[monsters.size()-1]->hp = 3;

					monsters[monsters.size()-1]->x = x;
					monsters[monsters.size()-1]->y = y;

					break;
				}
			}
		}
	}
	message.push_back("You die... ");
	redraw();
	getch();

	endwin();

	return 0;
}

void victory()
{
	erase();

	mvprintw(0, 0, "Very good comrade!  You have completed your task!\n\nNow kindly allow the guards to escort you back to your cell...");
	refresh();
	getch();
	endwin();

	exit(0);
}

void move_player(int direction)
{
	int x = player->x;
	int y = player->y;

	switch(direction)
	{
		case UP:
			y--;
			if(current_map->data[y][x] == FLOOR)
			{
				if(is_crate_there(x, y))
				{
					if(!push_crate(UP))
						return;
				}
				if(attack(x,y)) return;

				player->y = y;
				return;
			}
			break;
		case DOWN:
			y++;
			if(current_map->data[y][x] == FLOOR)
			{
				if(is_crate_there(x, y))
				{
					if(!push_crate(DOWN))
					return;
				}
				if(attack(x,y)) return;

				player->y = y;
				return;
			}
			break;
		case LEFT:
			x--;
			if(current_map->data[y][x] == FLOOR)
			{
				if(is_crate_there(x, y))
				{
					if(!push_crate(LEFT))
					return;
				}
				if(attack(x,y)) return;

				player->x = x;
				return;
			}
			break;
		case RIGHT:
			x++;
			if(current_map->data[y][x] == FLOOR)
			{
				if(is_crate_there(x, y))
				{
					if(!push_crate(RIGHT))
						return;
				}
				if(attack(x,y)) return;

				player->x = x;
				return;
			}
			break;
		case UPLEFT:
			x--;
			y--;
			if(current_map->data[y][x] == FLOOR)
			{
				if(is_crate_there(x, y))
				{
					message.push_back("You don't have enough leverage to push the block that way");
					return;
				}
				if(attack(x,y)) return;

				player->x = x;
				player->y = y;
				return;
			}
			break;

		case UPRIGHT:
			x++;
			y--;
			if(current_map->data[y][x] == FLOOR)
			{
				if(is_crate_there(x, y))
				{
					message.push_back("You don't have enough leverage to push the block that way");
					return;
				}
				if(attack(x,y)) return;

				player->x = x;
				player->y = y;
				return;
			}
			break;

		case DOWNLEFT:
			x--;
			y++;
			if(current_map->data[y][x] == FLOOR)
			{
				if(is_crate_there(x, y))
				{
					message.push_back("You don't have enough leverage to push the block that way");
					return;
				}
				if(attack(x,y)) return;

				player->x = x;
				player->y = y;
				return;
			}
			break;

		case DOWNRIGHT:
			x++;
			y++;
			if(current_map->data[y][x] == FLOOR)
			{
				if(is_crate_there(x, y))
				{
					message.push_back("You don't have enough leverage to push the block that way");
					return;
				}
				if(attack(x,y)) return;

				player->x = x;
				player->y = y;
				return;
			}
			break;
	}

	if(current_map->data[y][x] == CLOSED_DOOR)
		message.push_back("Ze door is locked comrade, complete your task! ");
	else if(current_map->data[y][x] == OPEN_DOOR)
	{
		level_count++;
		if(level_count == 5)
			victory();

		redraw();
		mvprintw(player->y+1, player->x+xoffset, ".");
		for(unsigned int i=0; i<monsters.size(); i++)
			mvprintw(monsters[i]->y+1, monsters[i]->x+xoffset, ".");
		message.push_back("You continue on... ");
		load_new_level();
	}

	return;
}

void run_rats()
{
	for(unsigned int i=0; i<monsters.size(); i++)
	{
		if(rand()%2 != 1)
			continue;

		int x = monsters[i]->x;
		int y = monsters[i]->y;
		int move;

		if(abs(monsters[i]->x - player->x) <= 1 && abs(monsters[i]->y - player->y) <= 1)
		{
			if(abs(monsters[i]->x - player->x) == 1 && abs(monsters[i]->y - player->y) == 1)
			{
				if(monsters[i]->x > player->x)
					move = LEFT;
				else
					move = RIGHT;
			}
			else
			{
				message.push_back("The lab rat bites you! ");
				move = 9;

				player->hp -= 1;
			}
		}
		else if(abs(monsters[i]->x - player->x) > 4 && abs(monsters[i]->y - player->y) > 4) // && rand()%4 != 1)
		{
			if(abs(monsters[i]->x - player->x) > abs(monsters[i]->y - player->y))
			{
				if(player->x > monsters[i]->x)
					move = RIGHT;
				else
					move = LEFT;
			}
			else
			{
				if(player->y > monsters[i]->y)
					move = DOWN;
				else
					move = UP;
			}
		}
		else
		{
			move = rand()%4;
		}
		switch(move)
		{
		case UP:
			y--;
			if(current_map->data[y][x] == FLOOR)
			{
				if(is_crate_there(x, y))
					break;

				monsters[i]->y--;
			}
			break;
		case DOWN:
			y++;
			if(current_map->data[y][x] == FLOOR)
			{
				if(is_crate_there(x, y))
					break;

				monsters[i]->y++;
			}
			break;
		case LEFT:
			x--;
			if(current_map->data[y][x] == FLOOR)
			{
				if(is_crate_there(x, y))
					break;

				monsters[i]->x--;
			}
			break;
		case RIGHT:
			x++;
			if(current_map->data[y][x] == FLOOR)
			{
				if(is_crate_there(x, y))
					break;

				monsters[i]->x++;
			}
			break;
		default:
			break;
		}

		if(monsters[i]->y <= 5)
			monsters[i]->y++;
	}
}

bool attack(int x, int y)
{
	for(unsigned int t=0; t<monsters.size(); t++)
		if(monsters[t]->x == x && monsters[t]->y == y)
		{
			monsters[t]->hp--;
			if(monsters[t]->hp <= 0)
			{
				message.push_back("You kill the lab rat with your wrench! ");

				delete monsters[t];
				monsters.erase(monsters.begin()+t);
			}
			else
				message.push_back("You hit the lab rat with your wrench ");
			return true;
		}
	return false;
}

bool is_crate_there(int x, int y)
{
	if(x < current_crate->x || y < current_crate->y || x > current_crate->x+4 || y > current_crate->y+4)
		return false;

	for(int n=0; n<4; n++)
	for(int i=0; i<4; i++)
	{
		if(current_crate->shape[current_crate->rotation]->data[n][i])
			if(i+current_crate->x == x && n+current_crate->y == y)
				return true;
	}

	return false;
}

bool push_crate(int direction)
{
	int x = 0;
	int y = 0;

	switch(direction)
	{
		case UP:
			y--;
			break;
		case DOWN:
			y++;
			break;
		case RIGHT:
			x++;
			break;
		case LEFT:
			x--;
			break;
	}

	int offset_x = current_crate->x + x;
	int offset_y = current_crate->y + y;


	for(int n=0; n<4; n++)
	for(int i=0; i<4; i++)
	{
		if(current_crate->shape[current_crate->rotation]->data[n][i])
		{
			if(current_map->data[n+offset_y][i+offset_x] != FLOOR)
				return false;

			for(unsigned int t=0; t<monsters.size(); t++)
				if(monsters[t]->x == i+offset_x && monsters[t]->y == n+offset_y)
				{
					message.push_back("The lab rat gets in the way! ");
					return false;
				}
		}
	}

	current_crate->x = offset_x;
	current_crate->y = offset_y;

	return true;
}

void plant_piece()
{
	if(current_crate->y <= 5)
		return;

	for(int n=0; n<4; n++)
	for(int i=0; i<4; i++)
	{
		if(current_crate->shape[current_crate->rotation]->data[n][i])
			current_map->data[current_crate->y+n][current_crate->x+i] = current_crate->color + 3;
	}

	int lines = 0;

	for(unsigned int n=0; n<current_map->height;n++)
	{
		int count = 0;
		for(unsigned int i=0; i<current_map->width; i++)
			if(current_map->data[n][i] >= 4)
				count++;
		if(count == 10)
			lines++;
	}

	if(lines > filled_lines)
	{
		message.push_back("Excellent, keep up the good work comrade! ");
		player->hp += (lines - filled_lines)*5;

		filled_lines = lines;
	}

	player->hp += 2;
	placed++;

	if(placed == target)
	{
		current_map->data[6][11] = OPEN_DOOR;
		message.push_back("Vary good comrade! Proseed to ze exit. ");
	}
	else
		create_piece();

	return;
}

void create_piece()
{
	int num = rand() % 7;

	string peice;

	switch(num)
	{
		case 0: peice = "i"; break;
		case 1: peice = "j"; break;
		case 2: peice = "l"; break;
		case 3: peice = "o"; break;
		case 4: peice = "s"; break;
		case 5: peice = "t"; break;
		case 6: peice = "z"; break;
	}

	delete current_crate;
	current_crate = new crate(peice);
	current_crate->x = 4;
	current_crate->y = 2;

	return;
}

void rotate_recenter()
{
	current_crate->rotation++;
	if(current_crate->rotation == 4)
		current_crate->rotation = 0;

	current_crate->x = 4;
	current_crate->y = 2;

	return;
}

void load_new_level()
{
	ifstream fin("data/std.map");

	fin >> height >> width;

//	delete current_map;
//	delete current_crate;

	player->x = 1;
	player->y = 6;

	monsters.clear();

	current_map = new map(height, width, FLOOR);

	for(unsigned int n=0; n<height; n++)
		for(unsigned int i=0; i<width; i++)
			fin >> current_map->data[n][i];

	create_piece();

	placed = 0;
	target += 5;

	xoffset += 11;

	filled_lines = 0;

	return;
}

void redraw()
{
	void *dumb;

	for(unsigned int n=0; n<current_map->height; n++)
	for(unsigned int i=0; i<current_map->width; i++)
	{
		switch(current_map->data[n][i])
		{
			case FLOOR:
				mvprintw(n+1, i+xoffset, ".");
				break;
			case WALL:
				mvprintw(n+1, i+xoffset, "#");
				break;
			case CLOSED_DOOR:
				mvprintw(n+1, i+xoffset, "+");
				break;
			case OPEN_DOOR:
				mvprintw(n+1, i+xoffset, "-");
				break;
			case RED_BLOCK:
				mvprintw(n+1, i+xoffset, " ");
				mvchgat(n+1, i+xoffset, 1, COLOR_PAIR(1), 1, dumb);
				break;
			case WHITE_BLOCK:
				mvprintw(n+1, i+xoffset, " ");
				mvchgat(n+1, i+xoffset, 1, COLOR_PAIR(2), 2, dumb);
				break;
			case MAGEN_BLOCK:
				mvprintw(n+1, i+xoffset, " ");
				mvchgat(n+1, i+xoffset, 1, COLOR_PAIR(3), 3, dumb);
				break;
			case BLUE_BLOCK:
				mvprintw(n+1, i+xoffset, " ");
				mvchgat(n+1, i+xoffset, 1, COLOR_PAIR(4), 4, dumb);
				break;
			case GREEN_BLOCK:
				mvprintw(n+1, i+xoffset, " ");
				mvchgat(n+1, i+xoffset, 1, COLOR_PAIR(5), 5, dumb);
				break;
			case YELLOW_BLOCK:
				mvprintw(n+1, i+xoffset, " ");
				mvchgat(n+1, i+xoffset, 1, COLOR_PAIR(6), 6, dumb);
				break;
			case CYAN_BLOCK:
				mvprintw(n+1, i+xoffset, " ");
				mvchgat(n+1, i+xoffset, 1, COLOR_PAIR(7), 7, dumb);
				break;

			default:
				break;
		}
	}

	for(int n=0; n<4; n++)
	for(int i=0; i<4; i++)
	{
		if(current_crate->shape[current_crate->rotation]->data[n][i])
		{
			mvprintw(n+current_crate->y+1, i+current_crate->x+xoffset, " ");
			mvchgat(n+current_crate->y+1, i+current_crate->x+xoffset, 1, COLOR_PAIR(current_crate->color), current_crate->color, dumb);
		}
	}

	for(unsigned int i=0; i<monsters.size(); i++)
	{
		mvprintw(monsters[i]->y+1, monsters[i]->x+xoffset, "r");
	}

	mvprintw(10, 60, "        ");
	mvprintw(10, 60, "HP: %d", player->hp);
	mvprintw(11, 60, "                ");
	mvprintw(11, 60, "Placed: %d/%d", placed,target);

	mvprintw(player->y+1, player->x+xoffset, "@");

	for(int n=0; n<COLS; n++)
		mvprintw(0, n, " ");

	while(message.size() != 0)
	{
		string output = "";
		unsigned int width;
		int total = 0;
		if(COLS < 80)
			width = 80;
		else
			width = COLS;

		output += message[0];
		total += message[0].length();
		message.erase(message.begin());

		while(message.size() != 0)
		{
			if(total + message[0].length() > width - 10)
				break;

			output += message[0];
			total += message[0].length();
			message.erase(message.begin());
		}

		if(message.size() != 0)
			output += " - MORE -";

		for(unsigned int n=0; n<width; n++)
			mvprintw(0, n, " ");

		mvprintw(0, 0, (char* )output.c_str());

		refresh();

		if(message.size() != 0)
		{
				getch();
		}
	}

	touchwin(stdscr);
	refresh();

	return;
}

