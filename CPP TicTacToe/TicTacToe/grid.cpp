#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include "util.h"

using std::string;
using std::vector;
using std::endl;
using std::cout;
using std::cin;

class Grid
{
   public:

      enum { OFF = '*', ON = '.' };

      Grid(int size) : mSize(size)
      {
         reset();
      }

      Grid() : mSize(3)
      {
         reset();
      }

      bool setChar(int row, int column, char c)
      {
         // If out of bounds or if the specified location
         // is already in state: ON

         if(row <      0 || column <      0 ||
            row >= mSize || column >= mSize ||
            mRows[row][column]     != OFF)
               return(false);

         mRows[row][column] = c;

         return(true);
      }

      int size() const { return(mSize); }

      void show() const
      {
         static string line;

         if(!line.size())
         {
            line += "+";

            for(int i = 0; i < mSize; i ++)
               line += "---+";
         }

         cout << line << endl;

         for(int i = 0; i < mSize; i ++)
         {
            cout << "|";

            for(int j = 0; j < mSize; j ++)
               cout << " " << mRows[i][j] << " " << "|";

            cout << endl << line << endl;
         }
      }

      bool isFull() const
      {
         for(int i = 0; i < mSize; i ++)
            for(int j = 0; j < mSize; j ++)
               if(mRows[i][j] == OFF)
                  return(false);

         return(true);
      }

      static int getValue(int minValue, int maxValue)
      {
         int theValue = 0;

         for(;;)
         {
            cout << "Enter an integer >= " << minValue <<
               " and <= " << maxValue << ": ";

            string line;

            getline(cin, line);

            if(Util::getInteger(line, &theValue, minValue, maxValue))
               break;
         }

         return(theValue);
      }

   private:

      void reset()
      {
         mRows.clear();

         for(int i = 0; i < mSize; i ++)
            mRows.push_back(string(mSize, OFF));
      }

      int mSize;
      vector<string> mRows;
};

int main()
{
   const int SIZE = 4;

   Grid g(SIZE);

   int maxValue = g.size() * g.size();

   for(;;)
   {
      g.show();
      cout << endl;

      if(g.isFull())
         break;

      int value = Grid::getValue(1, maxValue);

      cout << endl;

      int row    = (value - 1) / g.size();
      int column = (value - 1) % g.size();

      if(!g.setChar(row, column, Grid::ON))
         cout << "(location already chosen)" << endl << endl;
   }

   return(0);
}
