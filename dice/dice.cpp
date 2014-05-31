#include <iostream>
#include <cstdlib>
#include <ctime>

inline void Pause(bool roll)
{
	if(roll)
		std::cout << "Press any key to roll..." << std::endl;
	else
		std::cout << "Press any key to continue..." << std::endl;
	char dummy;
	std::cin.get(dummy);
}

class Dice
{
	short m_die1;
	short m_die2;

public:

	/* This function will make a nice animation and assign a random number
	   between 1 and 6 to each die.
	*/

	void DiceRoll()
	{
		using namespace std;
		Pause(1);
		short temp_die1;
		short temp_die2;

		/* This for-loop handles the 'animation' for rolling dice,
		   the '\r' moves the
		   cursor back to the beginning of the line, so the values
		   of the two dice will be printed
		   on the same line over and over again. The final value
		   is assigned to the member variables.
		*/

		for(int lll = 0; lll < 30000; lll++)
		{
			temp_die1 = (rand() % (6 - 1 + 1)) + 1;
			temp_die2 = (rand() % (6 - 1 + 1)) + 1;
			cout << '\r' << temp_die1 << '\t' << temp_die2;
		}
		cout << endl << endl;
		m_die1 = temp_die1;
		m_die2 = temp_die2;
	}

	/* This is for the computer's roll, it asks for no user
	   input. In all other respects it is identical to DiceRoll().
	*/

	void ComputerDiceRoll()
	{
		using namespace std;
		short temp_die1;
		short temp_die2;
		for(int lll = 0; lll < 30000; lll++)
		{
			temp_die1 = (rand() % (6 - 1 + 1)) + 1;
			temp_die2 = (rand() % (6 - 1 + 1)) + 1;
			cout << '\r' << temp_die1 << '\t' << temp_die2;
		}
		cout << endl << endl;
		m_die1 = temp_die1;
		m_die2 = temp_die2;
	}

	short GetDice() { return m_die1 + m_die2; }
};

short PlayBattle()
{
	using namespace std;
	short playerscore = 0;
	short computerscore = 0;
	short rounds = 1;

	Dice computer;
	Dice player;
	cin.ignore(); // Keeps our next DiceRoll from getting bitten by stale input
	cout << "== DICE BATTLE ==" << endl << endl;
	while(playerscore < 5 && computerscore < 5)
	{
		cout << "Round " << rounds << endl;
		cout << "Scores are: You: " << playerscore << '\t' << "Me: " <<
		computerscore << "\n" << endl;
		cout << "Okay, I'll roll..." << endl;
		computer.ComputerDiceRoll();
		cout << "I rolled " << computer.GetDice() << ", now it's your turn!" << endl << endl;
		player.DiceRoll();
		cout << "You rolled " << player.GetDice() << endl;
		if(computer.GetDice() > player.GetDice())
		{
			cout << computer.GetDice() << " is greater than " << player.GetDice() <<
			", I win this round." << endl << endl;
			computerscore++;
		}
		if(player.GetDice() > computer.GetDice())
		{
			cout << player.GetDice() << " is greater than " << computer.GetDice() <<
			", you win this round." << endl << endl;
			playerscore++;
		}
		if(player.GetDice() == computer.GetDice())
			cout << "Draw round, let's go again..." << endl << endl;
		rounds++;
		cout << "Round finished." << endl;
		Pause(0);
	}
	cout << "Final score: You: " << playerscore << '\t' << "Me: " <<
	computerscore << "\n" << endl;
	if(playerscore > computerscore)
	{
		cout << "You Win!" << endl;
		return 0;
	}
	if(computerscore > playerscore)
	{
		cout << "I Win!" << endl;
		return 1;
	}
	return -1;
}


short PlayCraps()
{
	using namespace std;
	int point = -1;
	cin.ignore(); // Keeps our next DiceRoll from getting bitten by stale input
	cout << "\n== CLASSIC GAME ==" << endl << endl;
	Dice player;
	player.DiceRoll();
	cout << "You rolled " << player.GetDice() << '.' << endl;
	switch(player.GetDice())
	{
		case 2:
			cout << "Snakeyes, you lose!" << endl << endl;
			return 1; // We will return 1 for a loss.
		case 3:
			cout << "Cockeyes, you lose!" << endl << endl;
			return 1;
		case 12:
			cout << "Boxcars, you lose!" << endl << endl;
			return 1;
		case 7:
			cout << "You rolled a natural, you win!" << endl << endl;
			return 0; // We will return 0 for a win.
		case 11:
			cout << "You rolled a natural, you win!" << endl << endl;
			return 0;
		default:
			cout << "Your point is " << player.GetDice() <<
			", try and make it!" << endl << endl;
			point = player.GetDice();
			break;
	}
	short win = -1;
	while(1)
	{
		player.DiceRoll();
		if(player.GetDice() == point)
		{
			cout << "You made your point, you win!" << endl << endl;
			win = 0;
			break;
		}
		if(player.GetDice() == 7)
		{
			cout << "You rolled 7, you lose!" << endl << endl;
			win = 1;
			break;
		}
		if(player.GetDice() != point)
		{
		cout << "You missed your point, roll again." << endl << endl;
		}
	}
	return win;
}


short PlayGAR()
{
	using namespace std;
	short guess;
	Dice player;
	cout << "\n== GNR ==" << endl << endl;
	cout << "Okay, first you need to guess a number... ";
	cin >> guess;
	cin.ignore(); // Keeps the next DiceRoll from getting bitten by stale input
	for(short lll = 1; lll <= 5; lll++)
	{
		cout << "Roll #" << static_cast<short>(lll) << endl;
		player.DiceRoll();
		cout << endl << "You rolled " << player.GetDice() << '.' << endl;
		if(player.GetDice() == guess)
		{
			if(lll == 5)
			{
				cout << "Whew! That was close!" << endl;
			}
			cout << "You Win!" << endl;
			return 0;
		}
	}
	cout << "Out of guesses, you lose!" << endl;
	return 1;
}

short DoMainMenu()
{
	using namespace std;
	short input;
	cout << "Welcome to Dice!" << endl << endl;
	cout << "There are several different dice games you can play here." << endl;
	cout << "1. Simple Craps - A highly simplified version of craps." << endl;
	cout << "2. Guess ans Roll - Guess a number and try to roll it." << endl;
	cout << "3. DiceBattle - We each roll a number, the higest wins." << endl << endl;
	do
	{
		cout << "Which one do you want to play? ";
		cin >> input;
	} while(input == !1 && input == !2 && input == !3);
	return input;
}

int main()
{
	using namespace std;
	srand(time(0));
	short win;
	short input = DoMainMenu();
	switch(input)
	{
		case 1:
			win = PlayCraps();
			break;
		case 2:
			win = PlayGAR();
			break;
		case 3:
			win = PlayBattle();
			break;
		default:
			cout << "Invalid input (something's wrong)!" << endl;
			return -1;
	}
	if(win == 0)
		cout << "Good game! See you next time!" << endl;
	if(win == 1)
		cout << "Better luck next time!" << endl;
	if(win =! 1 && win != 0 || win == -1)
	{
		cout << "You found a bug!" << endl;
		return -1;
	}
	return 0;
}
