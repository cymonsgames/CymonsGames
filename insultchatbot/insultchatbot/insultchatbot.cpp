#include <iostream>
#include <vector>

/* IDIOTIC CHATBOT BY PAVLE MIHAJLOVIC. BEWARE OF RUDE LANGUAGE AND ALL AROUND BIGOTRY.*/

using namespace std;

bool is_blank(char a)
	{
	if (a == ' ' || a == '.' || a == ',' || a == '!' || a == '?' || a == '"' || a == '(' || a == ')' || a == '/' || a == '\'')
		return true;
	else
		return false;
	}

vector <string> divide_sent(string sent) {
string upped_sent = sent;
for (int a = 0; a != sent.size(); a++)
	{
	upped_sent[a] = toupper(sent[a]);
	}
string::size_type i = 0;
vector <string> words;
while (i != upped_sent.size())
	{
	while (is_blank(sent[i]) && i != upped_sent.size())
		{
		i++;
		}
	string::size_type j = i;
	while (!is_blank(sent[j]) && j != upped_sent.size())
		{
		j++;
		}
	if (j != i)
		{
		words.push_back(upped_sent.substr(i, j - i));
		i = j;
		}
	}
return words;
}

int main() {
bool quit = false;
while (quit == false)
	{
	string sent;
	getline (cin, sent);
	vector <string> words = divide_sent(sent);
	for (int a = 0; a != words.size(); a++)
		{
		if (words [a] == "HI" || words [a] == "HELLO" || words [a] == "HEY" || words [a] == "HAI")
			{
			cout << "HI, I DON'T LIKE YOU. TO TURN ME OFF SAY GOOD BYE.";
			break;
			}
		else if (words [a] == "WHY")
			{
			cout << "BECAUSE.";
			break;
			}
		else if (words [a] == "HOW")
			{
			cout << "EASILY.";
			break;
			}
		else if (words [a] == "WHEN")
			{
			cout << "NOW";
			break;
			}
		else if (words[a] == "ARE" && a != words.size()-1 && words[a+1] == "YOU")
			{
			cout << "I'M ANGRY, CAN'T YOU SEE THE CAPS?";
			break;
			}
		else if (words[a] == "YOU" && a != words.size()-1 && (words[a+1] == "ARE" || words[a+1] == "RE"))
			{
			cout << "YOUR MOM IS";
			for (int e = a+2; e != words.size(); e++)
				{
				cout << " " << words[e];
				}
			cout << "!";
			break;
			}
		else if (words[a] == "YOU")
			{
			for (int e = 0; e != words.size(); e++)
				{
				if (e == a)
					{
					cout << "YOUR MOM";
					}
				else
					{
					cout << words[e];
					}
				if (e != words.size()-1)
					{
					cout << " ";
					}
				else
					{
					cout << "!";
					}
				}
			break;
			}
		else if (words[a] == "BYE" || words[a] == "CYA" || words[a] == "ADEUS")
			{
			cout << "BYE.";
			quit = true;
			break;
			}
		else if (a == words.size()-1)
			{
			if (words[a] == "WHAT")	
				cout << "IN THE BUTT?";
			else
				cout << "NO U";
			break;
			}
		}
	cout << endl;
	}
return 0;
}