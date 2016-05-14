#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include "util.h"

using std::ostringstream;
using std::cout;
using std::endl;

string Util::quote(const string &s)
{
   string result = "\"";

   for(int i = 0, n = s.size(); i < n; i ++)
   {
      if(s[i] == '"')
         result += "&quot;";

      else
         result += s[i];
   }

   return(result + "\"");
}

string Util::toString(int value)
{
   ostringstream out;

   out << value;

   return(out.str());
}

string Util::doReplace(
   const string &s,
   const string &from,
   const string &to,
   bool  replaceAll
)
{
   string result = s;
   size_t index  = 0;

   for(;;)
   {
      size_t location = result.find(from, index);

      if(location == string::npos)
         break;

      result = result.replace(location, from.size(), to);

      if(!replaceAll)
         break;

      index = location + to.size();
   }

   return(result);
}

string Util::doReplace(
   const string &s, const vector<string> &values
)
{
   string result = s;

   for(int i = 0, n = values.size(); i < n; i ++)
      result = doReplace(result, "{" + toString(i) + "}", values[i], true);

   return(result);
}

int Util::getRandomValue()
{
   static bool first = true;

   if(first)
   {
      first = false;
      srand((unsigned)time(0));
   }

   return(rand());
}

#if 0

int main()
{
   string s = "x xx xxx";

   cout << Util::doReplace(s, "x", "xx", true) << endl;

   return(0);
}

#endif
