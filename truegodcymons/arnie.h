#ifndef ARNIE_H_INCLUDED
#define ARNIE_H_INCLUDED
#define NTHOUGHT 45
#define NWHIMS 10
#define NPLAYS 14

#define KILLTWO 0
#define KILLSEVEN 1
#define GOFAST 2
#define GOSLOW 3
#define PACIFIST 4
#define SLAUGHTER 5
#define NOITEMS 6
#define ALLITEMS 7
#define GOBACK 8
#define GETOUT 9



char thought[NTHOUGHT][80]=
{
"A kitten",
"A girl named Moo",
"Chewing gum and baling wire",
"Nachos",
"Nothing much, What are you thinking?",
"Commie invaders",
"Get Out",
"Buttons and such",
"Lemons",
"#rgrd on quakenet irc",
"Existance",
"Nonexistance",
"Who would win in a fight, me or kos?",
"http://cymonsgames.retroremakes.com/commieinvaders",
"Pumpkin pie spice",
"Pretty pretty princess",
"playing harmonica",
"How much wood could a woodchuck chuck if i had a beer?",
"Ice cold bile",
"Who would win in a fight, Tesla or Mr. T?",
"Playing chess with checkers pieces",
"I'm on a roll baby",
"A guy that does not exist",
"Changing his name to '><(0)|\\/|'",
"Changing his name to 'Dolly'",
"Yo Quiero Taco bell",
"Quien es la mas fea, Hilary Clinton o Brittany Spears?",
"A one-eyed picanese",
"War is bad",
"War is good",
"I wonder what it is like to be mortal",
":)",
":(",
"Should i go to KFC or kill a chicken myself?",
"Sex and candy",
"7DRL winner Whoooo!!",
"Why do people go to work all the time?",
"How many of thes stupid phrases do I really need?",
"Watching an episode of 'Doug'",
"A song to make me stop killing you",
"Margays can turn their hands backwards",
"Haydn's 94th Symphony in G",
"I wonder what you taste like",
"A wizard did it",
"If you blame anyone, blame Bunny.",
};

int wi;
bool noquestion;
bool whim[NWHIMS];
int turncount,killcount,itemcount;
int luck;
int boredom;
int question();
void newwhim();
void checkitems(int desired);

void checkspeed(bool desired);
void checkkills(int desired);
void getout();
int arniemod();
void bored();


#endif // ARNIE_H_INCLUDED
