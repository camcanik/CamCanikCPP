#ifndef _UTIL_H_
#  define _UTIL_H_

#  include <sstream>
#  include <string>

   using std::ostringstream;
   using std::string;

   struct Util
   {
      static string ltrim(const string &s);
      static string rtrim(const string &s);
      static string trim(const string &s);

      template<class T>
      static string toString(const T& t)
      {
         ostringstream out; out << t; return(out.str());
      }

      enum { MAX_INT32 = 0x7fffffff };

      static bool getInteger(
         const string &s,
         int          *valuePtr,
         int          minValue = 0,
         int          maxValue = MAX_INT32
      );

      private:

         Util();
   };

#endif
