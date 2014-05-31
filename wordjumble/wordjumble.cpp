// by TheGuyNextDoor96 on YouTube.
// http://www.youtube.com/user/TheGuyNextDoor96

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

int main()
{
    enum fields{WORD, HINT, NUM_FIELDS};
    const int NUM_WORDS = 10;
    const string WORDS[NUM_WORDS][NUM_FIELDS] =
    {
          {"thesaurus", "The dinosaur dictionary."},
          {"emotion", "Your inner feeling"},
          {"affection", "The feeling for the girl down the street"},
          {"math", "Your worst subject"},
          {"valuable", "That's worth a lot"},
          {"detest", "This so hard, I hate it"},
          {"repulsive", "This food is nasty"},
          {"nutrition", "It's good to eat broccoli"},
          {"anatomy", "I love medical shows"},
          {"sympathy", "I'm don't feel sorry for you"}

    };

    srand(time(0));
    int choice = (rand() % NUM_WORDS);
    string theWord = WORDS[choice][WORD];
    string theHint = WORDS[choice][HINT];

    string jumble = theWord;
    int length = jumble.size();
    for (int i = 0; i < length; ++i)
    {
        int index1 = (rand() % length);
        int index2 = (rand() % length);
        char temp = jumble[index1];
        jumble[index1] = jumble[index2];
        jumble[index2] = temp;
    }

    cout << "\t\tWelcome to Word Jumble!\n";
    cout << "\n\nThe rules are simple\n";
    cout << "A word scrambled will appear on the screen.\n";
    cout << "It is your job to unscramble it.\n";
    cout << "If it's too hard, you can ask for hint by typing 'hint'.\n";
    cout << "If you want to quit just type 'quit'.  \n";
    cout << "The word is " << jumble;

    string guess;
    cout << "\n\nEnter your guess\n";
    cin >> guess;

    while ((guess != theWord) && (guess != "quit"))
    {
        if (guess == "hint")
        {
         cout << "Your hint is " << theHint << endl;
        }

        else
            cout << "Sorry that's not it\n";

        cout << "Enter your guess.\n";
        cin >> guess;

    }

    if (guess == theWord)
       cout << "That's it you got it\n";

    cout << "Thanks for playing\n";

    system("pause");
    return 0;
}


