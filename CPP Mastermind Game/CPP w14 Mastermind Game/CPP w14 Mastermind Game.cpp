#include <iostream>
#include <string>
#include <iomanip>
#include <ctime>
#include <vector>
#include <algorithm>

using std::cin;
using std::cout;
using std::string;
using std::endl;
using std::vector;

static const string colors[] = { "red", "green", "blue", "cyan", "magenta", "yellow" };
static const int PEGNUM = 4;

static int getRandomValue()
{
	static bool first = true;

	if (first)
	{
		first = false;
		srand((int)time(0));
	}

	return(rand());
}

static string easyToLower(string word)
{
	transform(word.begin(), word.end(), word.begin(), ::tolower);
	return word;
}

inline string trim(string& str) // courtesy of stack overflow
{
	str.erase(0, str.find_first_not_of(' '));       //prefixing spaces
	str.erase(str.find_last_not_of(' ') + 1);         //surfixing spaces
	return str;
}

static vector<string> setNewCode()
{
	vector<string> theCode;

	for (int i = 0; i < PEGNUM; i++)
	{
		theCode.push_back(colors[getRandomValue() % 6]);
	}

	return theCode;
}

static vector<string> getPlayerGuess()
{
	vector<string> words;

	string in = "", word="";
	bool okay = false;
	bool quitting = false;

	while (!okay)
	{

		cout << "Enter your guess (red, green, blue, cyan, magenta, yellow. Enter 4 colors): \n"<<endl;
		getline(cin, in);
		//cout << endl;

		int index = 0;
		while (in.find_first_of(" ") != -1 || in.length() > 0) //get input
		{

			in = trim(in);

			index = in.find_first_of(" ");
			
			if (index == -1)
			{
				word = in;
				in = "";
			}
			else
				word = in.substr(0, index);

			words.push_back(word);
			in = in.substr(index+1);

			if (words.size() == PEGNUM)
			{
				break;
			}
		}
		
		okay = true;
		for (int i = 0; i < words.size(); i++) // check and standardize inputs
		{
			words[i] = easyToLower(words[i]);

			if (words[i] == "red" || words[i] == "r")
			{
				words[i] = colors[0];
				//cout << words[i] << endl;
			}
			else if (words[i] == "green" || words[i] == "g")
			{
				words[i] = colors[1];
				//cout << words[i] << endl;
			}
			else if (words[i] == "blue" || words[i] == "b")
			{
				words[i] = colors[2];
				//cout << words[i] << endl;
			}
			else if (words[i] == "cyan" || words[i] == "c")
			{
				words[i] = colors[3];
				//cout << words[i] << endl;
			}
			else if (words[i] == "magenta" || words[i] == "m")
			{
				words[i] = colors[4];
				//cout << words[i] << endl;
			}
			else if (words[i] ==  "yellow" || words[i] == "y")
			{
				words[i] = colors[5];
				//cout << words[i] << endl;
			}
			else if (words[i] == "quit")
			{
				quitting = true;
				words.clear();
				words.push_back("quit");
				return words;
			}
			else
			{
				cout << words[i] << " is an invalid submittion" << endl;
				words.clear();
				okay = false;
			}

		}

		if (!okay)
			cout << "Invalid entry. Please try again\n" << endl;

		if (words.size() != PEGNUM) // check if 4 entries
		{
			cout << "Must enter 4 colors. Try again\n" << endl;
			words.clear();
			okay = false;
		}
			
	}

	return words;
}

static int setBlackPegs(vector<string> theCode, vector<string> guessCode)
{
	int bps = 0;

	for (int i = 0; i < PEGNUM; i++)
	{
		if (theCode[i] == guessCode[i])
			bps++;
	}

	return bps;
}

static int setWhitePegs(vector<string> theCode, vector<string> guessCode, int bps)
{
	int RGBCMYnumsTheCode[6];
	int RGBCMYnumsGuessCode[6];

	int wps=0;

	for (int i = 0; i < 6; i++)
	{
		RGBCMYnumsTheCode[i] = 0;
		RGBCMYnumsGuessCode[i] = 0;
	}

	for (int i = 0; i < PEGNUM; i++)
	{
		if (theCode[i] == colors[0])
		{
			RGBCMYnumsTheCode[0]++;
		}
		else if (theCode[i] == colors[1])
		{
			RGBCMYnumsTheCode[1]++;
		}
		else if (theCode[i] == colors[2])
		{
			RGBCMYnumsTheCode[2]++;
		}
		else if (theCode[i] == colors[3])
		{
			RGBCMYnumsTheCode[3]++;
		}
		else if (theCode[i] == colors[4])
		{
			RGBCMYnumsTheCode[4]++;
		}
		else if (theCode[i] == colors[5])
		{
			RGBCMYnumsTheCode[5]++;
		}
	}

	//cout << "counted theCode" << endl;

	//for (int i = 0; i < 6; i++)
	//{
	//	cout << colors[i] << ": " << RGBCMYnumsTheCode[i] <<endl;
	//}

	for (int i = 0; i < PEGNUM; i++)
	{
		if (guessCode[i] == colors[0])
		{
			RGBCMYnumsGuessCode[0]++;
		}
		else if (guessCode[i] == colors[1])
		{
			RGBCMYnumsGuessCode[1]++;
		}
		else if (guessCode[i] == colors[2])
		{
			RGBCMYnumsGuessCode[2]++;
		}
		else if (guessCode[i] == colors[3])
		{
			RGBCMYnumsGuessCode[3]++;
		}
		else if (guessCode[i] == colors[4])
		{
			RGBCMYnumsGuessCode[4]++;
		}
		else if (guessCode[i] == colors[5])
		{
			RGBCMYnumsGuessCode[5]++;
		}
	}

	//cout << "counted guess" << endl;

	for (int i = 0; i < 6; i++)
	{
		//cout << "checking "<< colors[i] << endl;
		if (RGBCMYnumsTheCode[i] > 0 && RGBCMYnumsGuessCode[i] > 0)
		{
			//cout << "both non 0" << endl;
			while (RGBCMYnumsTheCode[i] != 0 && RGBCMYnumsGuessCode[i] != 0)
			{
				RGBCMYnumsTheCode[i]--;
				RGBCMYnumsGuessCode[i]--;
				wps++;

				//cout << RGBCMYnumsTheCode[i]<< " " <<RGBCMYnumsGuessCode[i] << endl;
			}
		}
	}

	wps -= bps;

	return wps;
}

int main()
{
	bool quit = false;
	string in = "";

	cout << "Ready to play? (Type \'yes\' or \'y\', or \'no\' or \'n\')";
	getline(cin, in);
	
	in = easyToLower(in);
	if (in == "no" || in == "n")
		quit = true;

	vector<string> theCode = setNewCode();
	vector<string> guessCode;

	while (!quit)
	{
		//Start new game with fresh variables
		theCode = setNewCode();
		
		int guesses = 0;
		int blackpegs = 0;
		int whitepegs = 0;

		//Repeat until solved or quit
		bool solved = false;
		while (!solved)
		{
			//for (int i = 0; i < theCode.size(); i++)
			//{
			//	cout << theCode[i] << " ";	
			//}

			//cout << endl << endl;

			guesses++;
			guessCode = getPlayerGuess();

			if (guessCode[0] == "quit")
			{
				cout << "The Code was: ";
				for (int i = 0; i < theCode.size(); i++)
				{
					cout << theCode[i] << " ";	
				}

				cout << endl;

				break;
			}

			cout << "You Entered: ";
			for (int i = 0; i < guessCode.size(); i++)
			{
				cout << guessCode[i] << " ";
			}

			cout << endl;

			blackpegs = setBlackPegs(theCode,guessCode);
			whitepegs = setWhitePegs(theCode,guessCode, blackpegs);

			cout << "Black Pegs: " << blackpegs << " | White Pegs: " << whitepegs << endl << endl;

			if (blackpegs == PEGNUM)
				solved = true;
		}


		//Game is over, show guesses, play again
		cout << "Game took " << guesses << " rounds!" << endl;

		cout << "\nWant to play again? (Type \'yes\' or \'y\')";
		getline(cin,in);

		cout << endl;

		if (in == "yes" || in == "y")
			quit = false;
		else
			quit = true;

		guessCode.clear();
		theCode.clear();
	}

	cout << "\nThanks for playing! \n";

	getchar();
	return 0;
}

/*

==========
   Flow   
==========

p1 (computer) sets code
p2 (you) enter 4 colors guess

check against the code
check BlackPegs -> if p2 code has mathing slots with p1, add bps
check WhitePegs -> determine how many of each color are in p1 code and p2 code, compare numbers, subtract bps, count wps

p2 guess again until bps = 4
	


*/