 __           _       _             __       _                
/ _\_   _  __| | ___ | | __ _   _  / _\ ___ | |_   _____ _ __ 
\ \| | | |/ _` |/ _ \| |/ /| | | | \ \ / _ \| \ \ / / _ \ '__|
_\ \ |_| | (_| | (_) |   < | |_| | _\ \ (_) | |\ V /  __/ |   
\__/\___/ \____|\___/|_|\_\ \___/  \__/\___/|_| \_/ \___|_|   


			By Timothy Sassone
			  Version 1.0


The program will start with a default puzzle already typed in.  Press [SPACE] at the main menu to edit the current puzzle or a number key [1-9] to start solving.


The puzzles are solved in three steps.

In the first step, the program prepares the puzzle by moving the information into structures and then loops through all the solved squares and tells all the other squares in the same row, column and sub-grid that they cannot have the same value.

In the second step the program loops through all the unsolved squares and looks for any that only have one possible value, solving any it finds.

In the third step the program loops through each row, column and sub-grid for any that have only possible location for a number, solving the appropriate squares as it goes.  (E.G. The second row is missing 5, 7, and 9.  Only one of the three open squares can hold the 9, while either of the other two could hold the 5 or 7.  The program knows the 9 has to go in the one square it does fit it.)


When you press a number, the program runs each of these steps that number of times.

When editing a puzzle, use the arrow keys to move the cursor, and either the number keys or the +/- keys to adjust the value of the selected square.