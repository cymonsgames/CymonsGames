#include <stdio.h>

#define numoptions 5

struct roomtype {
    int id;
    char name[80];
    char description[800];
    char options[numoptions][80];
    int optionid[numoptions];
};

#define numrooms (sizeof(room) / sizeof *room)
#define START 1
struct roomtype room[] = {
{ START,
"Darkness",
"Today you were going to make plans for a new shed.  Instead, you found\n"
"yourself in a cold, pitch black area.",
    { "Feel around for a light.",
      "Lie down",
      "", "", ""},
    { 3, 2, 0, 0, 0}
},
{ 2,
"Darkness",
"You are lying on the ground.  It's even colder down here.",
    { "Get up",
      "Stay on the ground",
      "", "", ""},
    { START, 2, 0, 0, 0}
},
{ 3,
"Darkness",
"You feel around for several minutes before you trip over a rock.  After you\n"
"get up again, you find a lantern on the ground.",
    { "Turn the lantern on",
      "Walk away",
      "", "", ""},
    { 4, START, 0, 0, 0}
},
{ 4,
"Dirt room",
"Turning on the lantern, you take a minute to look at your surroundings.\n"
"You see that you are in a squarish dirt room.  There is a hole in the\n"
"ceiling.",
    { "Go up",
      "Turn the lantern off",
      "", "", ""},
    { 5, START, 0, 0, 0}
},
{ 5,
"Hallway",
"After three tries, you finally manage to climb up the side of the room.\n"
"You are in a dirt hallway.  About thirty feet ahead there is a sharp bend\n"
"to the left.  There is a small rock on the ground.",
    { "Go around the bend",
      "Take the rock",
      "Go down",
      "", ""},
    { 6, 8, 7, 0, 0}
},
{ 6,
"Hallway",
"You go around the bend in the hallway and are spotted by a man wearing a\n"
"business suit and wielding a crossbow.  This is both the strangest and the\n"
"last thing you ever see, as he shoots you dead moments after spotting you.\n"
"***YOU HAVE DIED***",
    { "Start over",
      "Quit",
      "", "", ""},
    { START, -1, 0, 0, 0}
},
{ 7,
"Dirt room",
"You jump down into the dirt room below.  Unfortunately, you hit your head\n"
"on a rock very hard.\n"
"***YOU HAVE DIED***",
    { "Start over",
      "Quit",
      "", "", ""},
    { START, -1, 0, 0, 0}
},
{ 8,
"Hallway",
"You take the rock.",
    { "Go around the bend",
      "Go down",
      "", "", ""},
    { 9, 7, 0, 0, 0}
},
{ 9,
"Hallway",
"You go around the corner, only to find a man wearing a business suit and\n"
"wielding a crossbow!",
    { "Throw the rock at him",
      "Run back",
      "", "", ""},
    { 11, 10, 0, 0, 0}
},
{ 10,
"Hallway",
"You try to run away, but it is too late.  With his crossbow bolt, you are\n"
"quite summarily killed.\n"
"***YOU HAVE DIED***",
    { "Start over",
      "Quit",
      "", "", ""},
    { START, -1, 0, 0, 0}
},
{ 11,
"Hallway",
"You throw the rock at the man's head, knocking him unconscious.  Behind\n"
"him, you see tile covers the walls of the hallway, and a staircase leads up.",
    { "Take the crossbow",
      "Proceed down the hallway",
      "", "", ""},
    { 12, 13, 0, 0, 0}
},
{ 12,
"Hallway",
"You take the crossbow.  Having no idea how one works, you accidentally\n"
"shoot yourself in the chest with it.\n"
"***YOU HAVE DIED***",
    { "Start over",
      "Quit",
      "", "", ""},
    { START, -1, 0, 0, 0}
},
{ 13,
"Hallway",
"You keep walking through the hallway.  The tile is refreshing; it is a nice\n"
"change from bare dirt.  As you reach the top of the stairs, you notice a\n"
"turn to the right.",
    { "Go right",
      "Go back",
      "", "", ""},
    { 15, 14, 0, 0, 0}
},
{ 14,
"Hallway",
"You trip down the stairs and hit your head.\n"
"***YOU HAVE DIED***",
    { "Start over",
      "Quit",
      "", "", ""},
    { START, -1, 0, 0, 0}
},
{ 15,
"Hallway",
"You run around the corner and go two feet before bumping face first into a\n"
"door.  Why you decided to run around that corner, I'll never know.",
    { "Open the door",
      "Go back",
      "", "", ""},
    { 16, 14, 0, 0, 0}
},
{ 16,
"Dining room",
"Well, what do you know?  That was behind that door you've always been\n"
"to open.  Other than the man with the crossbow, this can all be explained\n"
"quite easily.",
    { "Look out a window",
      "Enter the living room",
      "", "", ""},
    { 17, 18, 0, 0, 0}
},
{ 17,
"Dining room",
"It seems to be night still; you should go back to bed.",
    { "Back",
      "", "", "", ""},
    { 16, 0, 0, 0, 0}
},
{ 18,
"Living room",
"Lots of furnature is in here.  You almost trip over a footrest on your way\n"
"through.  There is a stairway leading up.",
    { "Enter the dining room",
      "Go up the stairs",
      "", "", ""},
    { 16, 19, 0, 0, 0}
},
{ 19,
"Upstairs hallway",
"You enter the hallway from the stairs.  There is a bedroom at the other end.",
    { "Enter the bedroom.",
      "Go back down the stairs",
      "", "", ""},
    { 20, 18, 0, 0, 0}
},
{ 20,
"Bedroom",
"As you enter the bedroom, you pass out on the bed.  In the morning, you\n"
"wake up and start planning your shed.  Within three weeks, the shed is\n"
"built.  The man with the crossbow has been arrested and is on trial for\n"
"attempted murder.  You considered making a second shed, and some of your\n"
"friends started calling you \"Two Sheds\".  In the end, you decide not to\n"
"build the shed.\n"
"***YOU WIN***",
    { "Restart",
      "Quit",
      "", "", ""},
    { START, -1, 0, 0, 0}
}
};

int find_room_by_id(int id) {
    int c;

    for (c = 0; c < numrooms; c++)
        if (room[c].id == id) {
            return c;
        }
    return -1;
}

int do_room(int id) {
    int i, c, input, dest = 0;

    i = find_room_by_id(id);
    if (i < 0) 
		return i;
    printf("\n%s\n", room[i].name);
    printf("%s\n", room[i].description);
    while (!dest) {
        for (c = 0; c < numoptions; c++) {
            if (room[i].options[c][0])
                printf("%d - %s\n", c + 1, room[i].options[c]);
        }
        printf("\nType and option and press enter > ");
        scanf("%d", &input);
        if ((input > 0) && (input <= numoptions))
            dest = room[i].optionid[input - 1];
        if (!dest)
            printf ("Invalad choice. Please try again.\n");
    }
    return dest;
}

int main () {
    int room = START;

    while (room > 0)
        room = do_room(room);
    printf ("Thanks for playing\n");
    return 0;
}
