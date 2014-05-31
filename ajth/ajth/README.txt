A JOURNEY TO HELL
A roguelike by Jakub Wasilewski
in exactly 1kb of C source

INTRODUCTION
============

 "A hero, right?", the handsome devil at the entrance asked. "Sign here, please", he boomed,
presenting you with a sheet of paper.
 One thing's for certain - you sure didn't expect Hell to have such good service. Like many
before you, you were lured by stories of countless riches awaiting in the bowels of the hell
realm. And the nice lawyer at the gates assures you that you are free to take as much as you
can carry - as long as you are able to walk out with it.

COMPILATION
===========

The game can be compiled by simply invoking gcc:

gcc ajth.c -oajth -lcurses

The game was developed on Linux and the original source will work under Linux. Unfortunately,
I had to alter the source for Windows because of incompatibilities (different wchar_t
definition, and different colour assignment in curses) - the altered source is in the
'windows' directory alongside the library. The windows binary was compiled with MinGW and
pdcurses:

gcc ajth-windows.c -oajth -lpdcurses

GAMEPLAY
========

 Reach the seventh order of Hell, collecting as much gold as possible on the way. Each level
is full of gold and chests which contain numerous valuables.
 Of course, Hell also has its guardians. Demons roam freely, and the deeper you go, the more 
powerful they become. Fortunately, they are quite stupid - they are easily distracted and
are often found staring at a shiny object or contemplating a set of stairs. Once they get
close to you, though, they make up with brawn what they lack in brains. Be careful, they are
also very fast - unlike most roguelikes, the demons move BEFORE the PC.
 Hitting a demon will decrease its power, until it is dead. A vanquished demon leaves 
behind souls of all the heroes it devoured. These souls can enhance your own spirit and 
increase your resilience.

CONTROLS
========

1-9 on the numeric keypad is used for movement (NumLock HAS TO be on).
5 can be used to wait.
Q (that's SHIFT+q) quits the game.

Items are picked up automatically, and other features are used by walking into them.
After the game ends (due to death or winning), you still have to press SHIFT+Q to quit.

WHAT IS WHAT
============

@                 This is the player character.

a-z               These are demons, with 'a' being the weakest, and higher letters being
                  progressively more powerful.
                  
`                 This is a soul. Collect those to increase your maximum HP.

*                 These are power gems. Each level has one, and taking it will restore your 
                  HP to maximum (one use only).
                  
$                 This is gold. Collect as much as you can to increase your final score.

?                 These are magical chests, walk into them to open them. They usually 
                  contain gold or souls, but once in a while they can also have a nasty 
                  surprise inside...

TIPS
====

- always remember that the demons move BEFORE your character
- the souls in Hell are more powerful on deeper levels - they will grant a greater
  HP increase
- maximize the benefits of power gems by taking them as late as possible without dying
- never fight more than one monster at a time

KNOWN PROBLEMS
==============

- on some systems, randomization doesn't work properly - every run of the game on the same
terminal will give the same dungeon. To help with this, simply resize your terminal window
(this forces curses to reinitialize its WINDOW structure on which the randomization is
based).
- pressing special keys (ENTER, arrows and so on) can cause you to pass multiple turns
- curses interprets many actions as keypresses, so resizing the window can cause you to
  pass multiple turns

TRIVIA
======

This short readme is more than three and a half times as large as the source code for the 
game :).
