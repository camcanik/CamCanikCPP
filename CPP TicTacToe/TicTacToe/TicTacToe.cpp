// -----------------------
// Includes
// -----------------------

#include <iostream>
#include <sstream>
#include <string>
#include <cctype>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <vector>

// -----------------------
// Using Declarations
// -----------------------

using std::ostringstream;
using std::string;
using std::endl;
using std::cout;
using std::cin;
using std::vector;

// -----------------------
// Util
// -----------------------

struct Util
{
	static string ltrim(const string &s)
	{
		string result = s;

		// So long as our string has non-zero length and
		// the first char is whitespace, we remove it

		while (result.size() > 0 && isspace(result[0]))
			result = result.substr(1);

		return(result);
	}

	static string rtrim(const string &s)
	{
		string result = s;

		// So long as our string has non-zero length and
		// the last char is whitespace, we remove it

		for (;;)
		{
			int n = result.size();

			if (n == 0 || !isspace(result[n - 1]))
				break;

			result = result.substr(0, n - 1);
		}

		return(result);
	}

	static string trim(const string &s)
	{
		return(ltrim(rtrim(s)));
	}

	template<class T>
	static string toString(const T& t)
	{
		ostringstream out;

		out << t;

		return(out.str());
	}

	enum { MAX_INT32 = 0x7fffffff };

	static bool getInteger(
		const string &s,
		int          *valuePtr,
		int          minValue = 0,
		int          maxValue = MAX_INT32
		)
	{
		string theString = trim(s);

		// If 'theString' has leading zeros, remove all but one

		while (theString.size() > 1)
		{
			if (theString[0] == '0' && theString[1] == '0')
				theString = theString.substr(1);
			else
				break;
		}

		// How many digits in 'theString'?

		int nDigits = 0;

		for (auto i = theString.begin(); i != theString.end(); ++i)
		if (isdigit(*i))
			nDigits++;

		// If no digits, or less digits than the length
		// of 'theString' -> invalid input

		if (nDigits < 1 || nDigits < (int)theString.size())
			return(false);

		// Ok, is it too big to fit in a 32 bit integer?
		// Compare 'nDigits' to the length of MAX_INT32
		// as a string, if nDigits is larger, 'theString'
		// can't be converted to a 32-bit integer.

		static string maxInt32String = toString(MAX_INT32);

		int delta = theString.size() - maxInt32String.size();

		if (delta > 0)
			return(false);

		// The same length as MAX_INT32 as a string, so compare
		// directly (dictionary order, using strcmp) to see if
		// 'theString' will fit or not

		if (delta == 0)
		{
			if (strcmp(maxInt32String.c_str(), theString.c_str()) < 0)
				return(false);
		}

		int value = atoi(theString.c_str());

		// Final check, within minValue/maxValue or not?

		if (value < minValue || value > maxValue)
			return(false);

		*valuePtr = value;

		return(true);
	}

private:

	Util();
};

 ///////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//---------------------------My Part------------------------------\\
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\////////////////////////////////////


/////////////////////---- Grid Printing ----\\\\\\\\\\\\\\\\\\\\\\\\

static void setupGrid(char *grid, int size)
{
	for (int i = 0; i < size; i++)
		grid[i] = ' ';
}

static void setupDisplay(string *grid, int size)
{
	for (int i = 0; i < size; i++)
		grid[i] = std::to_string(i + 1);
}

static void printGrid(char *grid, int size)
{
	cout << "--------------\n The Grid \n--------------\n";

	int gridWidth = (int)sqrt(size);
	for (int i = 0; i < size; i++)
	{
		cout << grid[i];
		if (i % gridWidth == gridWidth - 1 && i + 1 != size) //if grid place is not the last on the row, and not the last place
		{
			cout << "\n";
			for (int x = 0; x < gridWidth; x++)
			{
				cout << "-";
				if (x + 1 != gridWidth)
				{
					cout << "+";
				}
			}
			cout << "\n";
		}
			
		else if (i + 1 != size)
		{
			cout << "|";
		}
			
	}
	cout << endl;
}
static void printGrid(string *grid, int size)
{
	int gridWidth = (int)sqrt(size);
	for (int i = 0; i < size; i++)
	{
		cout <<  grid[i];
		if (i % gridWidth == gridWidth - 1 && i + 1 != size) //if grid place is not the last on the row, and not the last place
		{
			cout << "\n";
			for (int x = 0; x < gridWidth; x++)
			{
				cout << "-";
				if (x + 1 != gridWidth)
				{
					cout << "+";
				}
			}
			cout << "\n";
		}

		else if (i + 1 != size)
		{
			cout << "|";
		}

	}
	cout << endl <<endl;
}

///////////////////////---- Player Section-----\\\\\\\\\\\\\\\\\\\\\\\\\

static bool PlayerMoveOK(char *grid, int size, int place) // is move a vaild input?
{
	if (place>size) //above range
		return false;
	else if (place <= 0) // below range
		return false;
	else if (grid[place - 1] != ' ') // place taken
		return false;
	else
		grid[place - 1] = 'x';

	return true;
}

static void PlayersTurn(char* grid, int gSize, string* display) // get move, check move, apply, check if game over
{
	printGrid(grid, gSize);
	cout << "\nPick a spot, or enter q to quit\n";
	printGrid(display, gSize);

	cout << "\nEntry: ";

	string line;
	getline(cin, line);

	if (line.size() > 0 && tolower(line[0]) == 'q')
		exit(0);

	int entry = 0;
	if (!Util::getInteger(line, &entry))
	{
		cout << "invalid input\n";
	}
	while (!PlayerMoveOK(grid, gSize, entry))
	{
		cout << "Invalid position. Try Again or q to quit\n";
		cout << "\nEntry: ";
		getline(cin, line);

		if (line.size() > 0 && tolower(line[0]) == 'q')
			exit(0);

		if (!Util::getInteger(line, &entry))
		{
			cout << "invalid input";
		}

		if (PlayerMoveOK(grid, gSize, entry))
			break;
	}
}

////////////////// Check if Game is Over \\\\\\\\\\\\\\\\\\\

static bool rowVictory(vector<int> v, int gSize)
{
	int gDim = (int) sqrt(gSize);
	for (int i = 0; i < v.size(); i++)
	{
		v[i] /= gDim;
	}

	int count = 0;
	for (int i = 0; i < gDim; i++)
	{
		for (int j = 0; j < v.size(); j++)
		{
			if (v[j] == i)
				count++;
		}

		if (count == gDim)
			return true;
		else
			count = 0;
	}

	return false;
}

static bool colVictory(vector<int> v, int gSize)
{
	int gDim = (int)sqrt(gSize);
	for (int i = 0; i < v.size(); i++)
	{
		v[i] %= gDim;
	}
	int count = 0;
	for (int i = 0; i < gDim; i++)
	{
		for (int j = 0; j < v.size(); j++)
		{
			if (v[j] == i)
				count++;
		}

		if (count == gDim)
			return true;
		else
			count = 0;
	}

	return false;
}

static bool diagVictory(vector<int> v, int gSize)
{
	int gDim = (int)sqrt(gSize);
	vector<int> vCopy=v;

	int count = 0;
	for (int i = 0; i < gDim; i++)
	{
		for (int j = 0; j < v.size(); j++)
		{
			if (v[j] == (i*(gDim + 1)))
			{
				count++;
				continue;
			}
		}
	}
	if (count == gDim)
		return true;

	count = 0;
	for (int i = 0; i < gDim; i++)
	{
		for (int j = 0; j < v.size(); j++)
		{
			if (v[j] == ((i*gDim)+(gDim-i)-1))
			{
				count++;
				continue;
			}
		}
	}
	if (count == gDim)
		return true;

	return false;
}

static bool isGameOver(char* grid, int gSize)
{
	int gDim = (int)sqrt(gSize);
	vector<int> xPlaces, oPlaces;

	for (int i = 0; i < gSize; i++)
	{
		if (grid[i] == 'x')
			xPlaces.push_back(i);
		else if (grid[i] == 'o')
			oPlaces.push_back(i);
	}

	if (xPlaces.size() < gDim && oPlaces.size() < gDim) // not enough moves made to be finished
		return false;

	vector<int> xTemp;
	xTemp = xPlaces;

	if (rowVictory(xTemp, gSize) || colVictory(xTemp, gSize) || diagVictory(xTemp, gSize))
	{
		
		return true;
	}

	vector<int> oTemp;
	oTemp = oPlaces;

	if (rowVictory(oTemp, gSize) || colVictory(oTemp, gSize) || diagVictory(oTemp, gSize))
	{
		
		return true;
	}

	int count = 0; // board is full
	for (int i = 0; i < gSize; i++)
	{
		if (grid[i] == ' ')
			count++;
	}

	if (count > 0)
		return false;
	else
	{
		return true;
	}
		
}

////////////////////////----- Computer Player Section -----\\\\\\\\\\\\\\\\\\\\\\\

static bool CompMoveOK(char *grid, int size, int place)
{
	if (place>size) //above range
		return false;
	else if (place <= 0) // below range
		return false;
	else if (grid[place - 1] != ' ') // place taken
		return false;
	else
		grid[place - 1] = 'o';

	return true;
}
static bool canCompWin(char* grid, int gSize)
{
	char* tempGrid = new char[gSize];
	for (int i = 0; i < gSize; i++)
	{
		tempGrid[i] = grid[i];
	}

	for (int i = 0; i < gSize; i++)
	{
		if (CompMoveOK(tempGrid, gSize, i + 1)) // if position is not taken, and taking it results in a win/game over, do it
		{
			if (isGameOver(tempGrid, gSize))
				return CompMoveOK(grid, gSize, i + 1);
			else //revert choice if not win/game over
				tempGrid[i] = ' ';
		}
	}

	delete tempGrid;

	return false;
}
static bool canEnemyWin(char* grid, int gSize)
{
	char* tempGrid = new char[gSize];
	for (int i = 0; i < gSize; i++)
	{
		tempGrid[i] = grid[i];
	}

	for (int i = 0; i < gSize; i++)
	{
		if (PlayerMoveOK(tempGrid, gSize, i + 1)) // if position is not taken, and taking it results in a win/game over, do it
		{
			if (isGameOver(tempGrid, gSize))
				return CompMoveOK(grid, gSize, i + 1);
			else //revert choice if not win/game over
				tempGrid[i] = ' ';
		}
	}

	delete tempGrid;
	return false;
}

static void CompsTurn(char* grid, int gSize)
{
	if (gSize % 2 == 1 && CompMoveOK(grid, gSize, (gSize / 2 + 1)))
	{
		return;
	}
	else if (canCompWin(grid, gSize)) //if comp can win, take it
	{
		return;
	}

	else if (canEnemyWin(grid, gSize)) // if enemy can win, stop it
	{
		return;
	}

	else if (!canCompWin(grid, gSize) && !canEnemyWin(grid, gSize)) // if neither player can win this/next turn, pick a place at random
	{
		srand(time(NULL));
		int choice = rand() % gSize;

		while (!CompMoveOK(grid, gSize, choice))
		{
			choice = rand() % gSize;
		}
	}
	
}


//////////////////////////------ Main ------\\\\\\\\\\\\\\\\\\\\\\\\\

int main()
{
	int gridNum = 0;
	// Get grid size
	for (;;)
	{
		cout << "Enter a non-negative number greater than 3 or \'q\' to quit: ";

		string line;
		getline(cin, line);

		if (line.size() > 0 && tolower(line[0]) == 'q')
			return(0);

		int value = 0;
		if (!Util::getInteger(line, &value))
		{
			cout << "invalid input";
		}
		else
		{
			if (value > 2)
			{
				gridNum = value;
				break;
			}
			else
			{
				cout << "Entry is less than 3. Setting to default value: 3\n";
				gridNum = 3;
				break;
			}
		}
		cout << endl << endl;
	}

	//Now we have the dimensions. Set up a new grid with dimensions
	cout << "Grid dimensions: " << gridNum << "x" << gridNum << endl;
	int gSize = gridNum*gridNum;

	char* grid = new char[gSize];
	string* DisplayGrid = new string[gSize];
	setupGrid(grid, gSize);
	setupDisplay(DisplayGrid, gSize);

	bool gameover = false;

	////////---- grid is set. Let's play ----\\\\\\\\\\\

	while (!gameover)
	{
		cout << "\n";
		
		PlayersTurn(grid, gSize, DisplayGrid);
		

		if (isGameOver(grid, gSize))
		{
			gameover = true;

			cout << "\nX Wins! (or ties....)\n";

			printGrid(grid, gSize);

			getchar();
			return(0);
		}

		CompsTurn(grid, gSize);
		

		if (isGameOver(grid, gSize))
		{
			gameover = true;

			cout << "\nO Wins! (or ties....)\n";

			printGrid(grid, gSize);

			getchar();
			return(0);
		}
	}

	delete grid;
	delete DisplayGrid;

	getchar();

	return(0);
}

