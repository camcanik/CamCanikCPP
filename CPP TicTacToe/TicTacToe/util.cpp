#include <cctype>
#include "util.h"

string Util::ltrim(const string &s)
{
   string result = s;

   // So long as our string has non-zero length and
   // the first char is whitespace, we remove it

   while(result.size() > 0 && isspace(result[0]))
      result = result.substr(1);

   return(result);
}

string Util::rtrim(const string &s)
{
   string result = s;

   // So long as our string has non-zero length and
   // the last char is whitespace, we remove it

   for(;;)
   {
      int n = result.size();

      if(n == 0 || !isspace(result[n - 1]))
         break;

      result = result.substr(0, n - 1);
   }

   return(result);
}

string Util::trim(const string &s)
{
   return(ltrim(rtrim(s)));
}

bool Util::getInteger(
   const string &s, int *valuePtr, int minValue, int maxValue
)
{
   string theString = trim(s);

   // If 'theString' has leading zeros, remove all but one

   while(theString.size() > 1)
   {
      if(theString[0] == '0' && theString[1] == '0')
         theString = theString.substr(1);
      else
         break;
   }

   // How many digits in 'theString'?

   int nDigits = 0;

   for(auto i = theString.begin(); i != theString.end(); ++ i)
      if(isdigit(*i))
         nDigits++;

   // If no digits, or less digits than the length
   // of 'theString' -> invalid input

   if(nDigits < 1 || nDigits < (int)theString.size())
      return(false);

   // Ok, is it too big to fit in a 32 bit integer?
   // Compare 'nDigits' to the length of MAX_INT32
   // as a string, if nDigits is larger, 'theString'
   // can't be converted to a 32-bit integer.

   static string maxInt32String = toString(MAX_INT32);

   int delta = theString.size() - maxInt32String.size();

   if(delta > 0)
      return(false);

   // The same length as MAX_INT32 as a string, so compare
   // directly (dictionary order, using strcmp) to see if
   // 'theString' will fit or not

   if(delta == 0)
   {
      if(strcmp(maxInt32String.c_str(), theString.c_str()) < 0)
         return(false);
   }

   int value = atoi(theString.c_str());
   
   // Final check, within minValue/maxValue or not?

   if(value < minValue || value > maxValue)
      return(false);

   *valuePtr = value;

   return(true);
}
