#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>

using std::ostringstream;
using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::ifstream;

typedef enum
{
	SUCCESS,
	FILE_NOT_FOUND,

} StatusCode;

typedef enum
{
	NORTH,
	EAST,
	SOUTH,
	WEST,
	EXIT

} Direction;

static int rowNum, colNum;

static StatusCode getLines(const string &fileName, vector<string> &lines)
{
	ifstream in(fileName);

	if (!in)
		return(FILE_NOT_FOUND);

	for (;;)
	{
		string line;

		getline(in, line);

		if (in.eof())
			break;

		lines.push_back(line);
	}

	
	rowNum = lines.size();

	int max = 0;
	for (int i = 0; i < rowNum; i++)
	{
		if (lines[i].length()>max)
			max = lines[i].length();
	}
	colNum = max;

	return(SUCCESS);
}

static vector<vector<char>> setupGrid(vector<string> &lines)
{
	vector<vector<char>>grid;

	for (int row = 0; row < rowNum; row++)
	{
		grid.push_back(vector<char>());
		for (int col = 0; col < colNum; col++)
		{
			if (col>=lines[row].length() && lines[row].length()<colNum)
			{
				lines[row] += " ";
				grid[row].push_back(' ');
			}
			else
			{
				grid[row].push_back(lines[row].at(col));
			}
			
		}
	}

	return grid;
}

static vector<string> findWords(vector<vector<char>> grid)
{

	vector<string> words;
	string newWord = "";
	int row = 0, col = 0;
	char cAtPos=' ';
	Direction currentD;
	bool done = false;
	
	cAtPos = grid[row][col+1];

	if (cAtPos != ' ')
		currentD = EAST;
	else
	{
		currentD = SOUTH;
		
	}
	
	while (!done)
	{
		
		cAtPos = grid[row][col];

		switch (currentD)
		{
			case NORTH:{
				while (cAtPos != ' ' && row >= 0)
				{
					
					newWord += cAtPos;
					if (row == 0)
					{
						row--;
						break;
					}
					else
					cAtPos = grid[--row][col];
					
				}
				row++;
				break;
			}
			case EAST:{
				while (cAtPos != ' ' && col <= colNum-1)
				{
					
					newWord += cAtPos;
					if (col == colNum - 1)
					{
						col++;
						break;
					}
					else
					cAtPos = grid[row][++col];
				}
				col--;
				break;
			}
			case SOUTH:{
				while (cAtPos != ' ' && row <= rowNum-1)
				{
					
					newWord += cAtPos;
					if (row == rowNum - 1)
					{
						row++;
						break;
					}
					else
						cAtPos = grid[++row][col];
					
				}
				row--;
				break;
			}
			case WEST:{
				while (cAtPos != ' ' && col >= 0)
				{
					
					newWord += cAtPos;
					if (col == 0)
					{
						col--;
						break;
					}
					else
					cAtPos = grid[row][--col];
				}
				col++;
				break;
			}
			default:{
				done = true;
			}

			
		}

		words.push_back(newWord);
		newWord = "";

		//find the direction to go in the grid
		if (col+1<colNum && grid[row][col + 1] != ' ' && currentD != WEST)
			currentD = EAST;
		else if (row+1<rowNum && grid[row + 1][col] != ' ' && currentD != NORTH)
			currentD = SOUTH;
		else if (col-1 > 0 && grid[row][col - 1] != ' ' && currentD != EAST)
			currentD = WEST;
		else if (row-1 > 0 && grid[row - 1][col] != ' ' && currentD != SOUTH)
			currentD = NORTH;
		else
			currentD = EXIT;
		//done = true;
	}
	
	return words;
}

int main()
{
	vector<string> lines;
	StatusCode sc = getLines("input-6.txt", lines);

	if (sc == FILE_NOT_FOUND)
	{
		cout << "File not found." << endl;
		getchar();
		return 0;
	}

	vector<vector<char>> theGrid = setupGrid(lines);

	vector<string> words = findWords(theGrid);

	cout << "==========\nThe Grid:\n==========\n" << endl;

	for (int i = 0; i < rowNum; i++)
	{
		cout << lines.at(i) << endl;
	}

	cout << "\n==========\nThe Words:\n==========\n" << endl;

	for (int i=0; i < words.size(); i++)
	{
		cout << words[i] << endl;
	}
	getchar();
	return(0);
}
