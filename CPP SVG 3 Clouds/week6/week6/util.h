#ifndef _UTIL_H_
#  define _UTIL_H_

#  include <vector>
#  include <string>

   using std::vector;
   using std::string;

   class Util
   {
      private:

         Util();

      public:

         static string quote(const string &s);

         static string toString(int value);

         static string doReplace(
            const string &text,
            const string &from,
            const string &to,
            bool  replaceAll = false
         );

         static string doReplace(
            const string &text, const vector<string> &values
         );

         static int getRandomValue();
   };

#endif
