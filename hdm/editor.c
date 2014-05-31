#include "config.h"
#include "types.h"

#define BOOL unsigned char
#define TRUE 1
#define FALSE 0

extern unsigned char terrain [MAP_SIZE_Y][MAP_SIZE_X];
extern unsigned char terrain_map [MAP_SIZE_Y][MAP_SIZE_X];
extern unsigned char unit_placement [MAP_SIZE_Y][MAP_SIZE_X];
extern struct terrain_def terrain_prop[TERRAIN_MAX];
extern struct unit_def unit_prop[UNIT_TYPE_MAX];
extern void PlayLevel();
extern void TestPlaceUnits();
extern void TestConvertTerrain();
extern position cursor_x,cursor_y;
extern BOOL mouse_active;

unsigned char current_terrain=0;
unsigned char current_unit=0;
BOOL editing_terrain=TRUE;
BOOL editing_blue=TRUE;

int blue_units_left=UNITS_PER_PLAYER;
int red_units_left=UNITS_PER_PLAYER;

void LoadTerrainMap(char *map_name);
void LoadUnitPlacement(char *map_name);

void LoadLevel()
{
	FILE *fp;
	unsigned char tile;
	register position x,y;
	fp=fopen("level.ter","rt");
	if (fp==NULL)
		return;
	for (y=0;y<MAP_SIZE_Y;++y)
	{
		for (x=0;x<MAP_SIZE_X;++x)
		{		
			tile=fgetc(fp);
			terrain_map[y][x]=tile;
		}
		tile=fgetc(fp);
		if (tile=='\r')
			tile=fgetc(fp);
	}
	fclose(fp);

	TestConvertTerrain();
	blue_units_left=UNITS_PER_PLAYER;
	red_units_left=UNITS_PER_PLAYER;

	fp=fopen("level.uni","rt");
	if (fp==NULL)
		return;

	for (y=0;y<MAP_SIZE_Y;++y)
	{
		for (x=0;x<MAP_SIZE_X;++x)
		{
			tile=fgetc(fp);
			unit_placement[y][x]=tile;
			if (tile>='a' && tile<='z')
				--blue_units_left;
			else if (tile>='A' && tile<='Z')
				--red_units_left;
		}
		tile=fgetc(fp);
		if (tile=='\r')
			tile=fgetc(fp);
	}
	fclose(fp);
	editing_terrain=FALSE;
}

void SaveLevel()
{
	FILE *fp;
	register position x,y;
	fp=fopen("level.ter","wt+");
	for (y=0;y<MAP_SIZE_Y;++y)
	{
		for (x=0;x<MAP_SIZE_X;++x)
		{
			switch(terrain[y][x])
			{
			case TERRAIN_FOREST:
				terrain_map[y][x]='&';
				break;
			case TERRAIN_MOUNTAIN:
				terrain_map[y][x]='^';
				break;
			case TERRAIN_WATER:
				terrain_map[y][x]='~';
				break;
			case TERRAIN_WALL:
				terrain_map[y][x]='#';
				break;
			default:
				terrain_map[y][x]='.';
			}
			fputc(terrain_map[y][x],fp);
		}
		fputc('\n',fp);
	}
	fclose(fp);

	fp=fopen("level.uni","wt+");
	for (y=0;y<MAP_SIZE_Y;++y)
	{
		for (x=0;x<MAP_SIZE_X;++x)
		{
			fputc(unit_placement[y][x],fp);
		}
		fputc('\n',fp);
	}
	fclose(fp);
}

void ShowEditorMap()
{
	unsigned char x,y,tile;
	for (y=0;y<MAP_SIZE_Y;++y)
	{
		for (x=0;x<MAP_SIZE_X;++x)
		{
			attrset(COLOR_PAIR(terrain_prop[terrain[y][x]].color));
			mvaddch(y,x,terrain_prop[terrain[y][x]].tile);
		}
	}

	if (!editing_terrain)
	{
		for (y=0;y<MAP_SIZE_Y;++y)
		{
			for (x=0;x<MAP_SIZE_X;++x)
			{
				tile=unit_placement[y][x];
				if (tile>='a' && tile<='z')
				{
					attrset(COLOR_PAIR(COLOR_WHITE+COLOR_BLUE*8)|A_BOLD);
					mvaddch(y,x,tile);
				}
				else if (tile!='.')
				{
					attrset(COLOR_PAIR(COLOR_WHITE+COLOR_RED*8)|A_BOLD);
					mvaddch(y,x,tile+0x20);
				}
			}
		}
	}
	refresh();
}

void ShowEditorWindow()
{
	unsigned int i;
	clear();
	ShowEditorMap();
	attrset(COLOR_PAIR(COLOR_WHITE));
	mvaddstr(20,0,"F1-terrain  F2-units  F5-test");
	mvaddstr(21,0,"F8-save     F9-load   F12-clear/fill");
	if (editing_terrain)
	{		
		for (i=0;i<TERRAIN_MAX;++i)
		{
			if (terrain_prop[i].color!=COLOR_WHITE)
				attrset(COLOR_PAIR(terrain_prop[i].color|COLOR_WHITE*8));
			else
				attrset(COLOR_PAIR(COLOR_BLACK|COLOR_WHITE*8));
			mvprintw(22,i,"%c",terrain_prop[i].tile);
			if (i==current_terrain)
			{
				attrset(COLOR_PAIR(COLOR_WHITE));
				mvprintw(23,0,"Current:");
				mvprintw(23,11,"%s",terrain_prop[i].name);
				attrset(COLOR_PAIR(terrain_prop[i].color));
				mvprintw(23,9,"%c", terrain_prop[i].tile);
			}			
		}		
	}
	else
	{
		for (i=0;i<UNIT_TYPE_MAX;++i)
		{
			if (editing_blue)
				attrset(COLOR_PAIR(COLOR_WHITE|COLOR_BLUE*8)|A_BOLD);
			else
				attrset(COLOR_PAIR(COLOR_WHITE|COLOR_RED*8)|A_BOLD);
			mvprintw(22,i,"%c",unit_prop[i].tile);
			if (i==current_unit)
			{
				mvprintw(23,9,"%c", unit_prop[i].tile);
				attrset(COLOR_PAIR(COLOR_WHITE));
				mvprintw(23,0,"Current:");
				mvprintw(23,11,"%s",unit_prop[i].name);
			}			
		}	
		if (current_unit==UNIT_TYPE_MAX)
		{
			attrset(COLOR_PAIR(COLOR_WHITE));
			mvprintw(23,0,"Current: Remove");
		}
		if (editing_blue)
		{
			attrset(COLOR_PAIR(COLOR_WHITE|COLOR_RED*8)|A_BOLD);
			mvaddch(22,i+1,' ');
		}
		else
		{
			attrset(COLOR_PAIR(COLOR_WHITE|COLOR_BLUE*8)|A_BOLD);
			mvaddch(22,i+1,' ');
		}
		attrset(COLOR_PAIR(COLOR_WHITE));
		mvprintw(22,30,"Blue: %d",blue_units_left);
		mvprintw(23,30,"Red:  %d",red_units_left);

	}
	move(cursor_y,cursor_x);
	refresh();
}

void ChooseUnit()
{

}

void ClearAfterPlaying()
{
	unsigned char x,y;
	for (y=0;y<MAP_SIZE_Y;++y)
		for (x=0;x<MAP_SIZE_X;++x)
		{
			terrain[y][x]=terrain[y][x]&TERRAIN_MASK;
		}
}

void FillMap()
{
	unsigned char x,y;
	if (editing_terrain)
	{
		for (y=0;y<MAP_SIZE_Y;++y)
			for (x=0;x<MAP_SIZE_X;++x)
			{
				terrain[y][x]=current_terrain;
			}
	}
	else
	{
		for (y=0;y<MAP_SIZE_Y;++y)
			for (x=0;x<MAP_SIZE_X;++x)
			{
				unit_placement[y][x]='.';
			}
		red_units_left=UNITS_PER_PLAYER;
		blue_units_left=UNITS_PER_PLAYER;
	}
	ShowEditorMap();
	ShowEditorWindow();
}

void EditorMoveCursor(position x, position y)
{
	cursor_x=x;
	cursor_y=y;
	move(y,x);
	refresh();
}

void EditorAction()
{
	unsigned char tile, previous;
	if (editing_terrain)
	{
		if (cursor_y<MAP_SIZE_Y)
		{
			terrain[cursor_y][cursor_x]=current_terrain;
			attrset(COLOR_PAIR(terrain_prop[current_terrain].color));
			mvaddch(cursor_y,cursor_x,terrain_prop[current_terrain].tile);
			refresh();
		}
		else if (cursor_y==22 && cursor_x<TERRAIN_MAX)
		{
			current_terrain=cursor_x;
			ShowEditorWindow();
		}
	}
	else
	{
		if (cursor_y<MAP_SIZE_Y)
		{
			previous=unit_placement[cursor_y][cursor_x];
			if (previous>='a' && previous<='z')
				++blue_units_left;
			else if (previous>='A' && previous<='Z')
				++red_units_left;

			if (current_unit==UNIT_TYPE_MAX) // removing
			{
				unit_placement[cursor_y][cursor_x]='.';
				ShowEditorMap();
				ShowEditorWindow();
				return;
			}
			else
			{
				if (editing_blue)
				{
					if (blue_units_left==0)
						return;
					else
						--blue_units_left;
				}
				else
				{
					if (red_units_left==0)
						return;
					else
						--red_units_left;
				}
			}

			tile = unit_prop[current_unit].tile - (editing_blue?0:0x20);
			unit_placement[cursor_y][cursor_x]=tile;
			if (editing_blue)
				attrset(COLOR_PAIR(COLOR_WHITE+COLOR_BLUE*8)|A_BOLD);
			else
				attrset(COLOR_PAIR(COLOR_WHITE+COLOR_RED*8)|A_BOLD);
			mvaddch(cursor_y,cursor_x,unit_prop[current_unit].tile);
			ShowEditorWindow();
		}
		else if (cursor_y==22)
		{
			if (cursor_x<UNIT_TYPE_MAX)
			{
				current_unit=cursor_x;
				ShowEditorWindow();
			}
			else if (cursor_x==UNIT_TYPE_MAX)
			{
				current_unit=UNIT_TYPE_MAX;
				ShowEditorWindow();
			}
			else if (cursor_x==UNIT_TYPE_MAX+1)
			{
				editing_blue=!editing_blue;
				ShowEditorWindow();
			}
		}

	}
}

void MapEditor()
{
	int key;
	clear();
	ShowEditorWindow();

	editing_terrain=FALSE;
	FillMap();
	editing_terrain=TRUE;
	FillMap();

	LoadLevel();
	ShowEditorMap();
	ShowEditorWindow();

	mouse_on(ALL_MOUSE_EVENTS);
	nodelay(stdscr,TRUE);
	cbreak();	

	for(;;)
	{
		key=getch(); 
		request_mouse_pos();
		if (mouse_active && (MOUSE_X_POS!=cursor_x || MOUSE_Y_POS!=cursor_y))
		{
			curs_set(0);
			EditorMoveCursor(MOUSE_X_POS,MOUSE_Y_POS);
			curs_set(2);
		}
		switch(key)
		{

		case KEY_F0+1:
			editing_terrain=TRUE;
			ShowEditorWindow();
			break;
		case KEY_F0+2:
			editing_terrain=FALSE;
			ShowEditorWindow();
			break;
		case KEY_F0+5:
			clear();
			PlayLevel();

			ClearAfterPlaying();
			ShowEditorWindow();

			mouse_on(ALL_MOUSE_EVENTS);
			nodelay(stdscr,TRUE);
			cbreak();	
			break;
		case KEY_F0+8:
			SaveLevel();
			break;
		case KEY_F0+9:
			LoadLevel();
			ShowEditorMap();
			ShowEditorWindow();
			break;
		case KEY_F0+12:
			FillMap();
			break;
		case KEY_MOVE_LEFT:
			EditorMoveCursor(cursor_x-1,cursor_y);
			break;
		case KEY_MOVE_RIGHT:
			EditorMoveCursor(cursor_x+1,cursor_y);
			break;
		case KEY_MOVE_UP:
			EditorMoveCursor(cursor_x,cursor_y-1);
			break;
		case KEY_MOVE_DOWN:
			EditorMoveCursor(cursor_x,cursor_y+1);
			break;
		case KEY_MOUSE:
		case KEY_ACTION:
			curs_set(0);
			EditorAction();
			move(cursor_y,cursor_x);
			curs_set(2);
		}
	}
}

