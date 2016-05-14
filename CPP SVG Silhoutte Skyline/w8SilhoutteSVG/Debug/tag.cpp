#include <iostream>
#include <sstream>
#include "util.h"
#include "tag.h"

using std::ostringstream;
using std::cout;
using std::endl;

// ----------------------------------------
// Tag - Implementation
// ----------------------------------------

Tag::Tag(const string &name) : mName(name)
{
}

void Tag::addAttribute(const string &key, const string &value)
{
   mAttributes.push_back(make_pair(key, value));
}

void Tag::addAttribute(const string &key, int value)
{
   mAttributes.push_back(make_pair(key, Util::toString(value)));
}

void Tag::setTarget(const string &target)
{
   mTarget = target;
}

string Tag::toString() const
{
   ostringstream out;

   out << "<" << mName;

   for(int i = 0, n = mAttributes.size(); i < n; i ++)
   {
      out << " " << mAttributes[i].first <<
             "=" << Util::quote(mAttributes[i].second);
   }

   if(mTarget == "")
   {
      out << "/>";
   }
   else
   {
      out << ">" << mTarget << "</" << mName << ">";
   }

   return(out.str());
}

#if 0

int main()
{
   Tag theTag("a");

   theTag.addAttribute("href", "http://www.fractorama.com");
   theTag.addAttribute("name", "some-link");

   cout << theTag.toString() << endl;

   return(0);
}

#endif
