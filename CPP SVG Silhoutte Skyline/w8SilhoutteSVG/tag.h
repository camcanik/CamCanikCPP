#ifndef _TAG_H_
#  define _TAG_H_

   #include <utility>
   #include <vector>
   #include <string>

   using std::vector;
   using std::string;

   typedef std::pair<string, string> KeyValue;

   class Tag
   {
      public:

         Tag(const string &name);

         void   addAttribute(const string &key, const string &value);
         void   addAttribute(const string &key, int value);
         void   setTarget(const string &target);
         string toString() const;

      private:

         string           mName;
         string           mTarget;
         vector<KeyValue> mAttributes;
   };

#endif
