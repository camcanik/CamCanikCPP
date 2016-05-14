#ifndef _SVG_H_
#  define _SVG_H_

#include <string>
#include <vector>
#include "tag.h"

using std::string;
using std::vector;

class Color
{
   private:

      Color();

   public:

      static const string white;
      static const string darkgray;
      static const string black;
      static const string yellow;
      static const string greenyellow;
      static const string lime;
      static const string darkgreen;
      static const string lightgreen;
      static const string seagreen;
      static const string aquamarine;
      static const string mediumaquamarine;
      static const string lightseagreen;
      static const string paleturquoise;
      static const string cyan;
      static const string darkturquoise;
      static const string deepskyblue;
      static const string lightskyblue;
      static const string royalblue;
      static const string blue;
      static const string darkblue;
      static const string lavender;
      static const string mediumslateblue;
      static const string blueviolet;
      static const string purple;
      static const string orchid;
      static const string magenta;
      static const string hotpink;
      static const string pink;
      static const string darkred;
      static const string crimson;
      static const string orangered;
      static const string darkorange;
      static const string gold;
      static const string lemonchiffon;
      static const string chocolate;
      static const string burlywood;

      static vector<string> all();
      static vector<string> drawingColors();
};

class Justify
{
   private:

      Justify();

   public:

      static const string start;
      static const string middle;
      static const string end;
};

class SVG
{
   public:

      SVG(int width, int height, bool bottomLeftOrigin = false);

      void rect(
         int x, int y,
         int width, int height,
         const string &edgeColor,
         const string &fillColor = ""
      );

      void circle(
         int x, int y,
         int radius,
         const string &edgeColor,
         const string &fillColor = ""
      );

      void line(int x1, int y1, int x2, int y2, const string &color);

      void polyline(
         const vector<int> &points, const string &color
      );

      void text(
         int x, int y, const string &text,
         const string &color, string justify = Justify::start
      );

      string toString() const;

      int getWidth()  const { return(mWidth);  }
      int getHeight() const { return(mHeight); }

      bool writeToFile(const string &fileName, string &errMsg);

   private:

      int yValue(int value) const;

      int         mWidth;
      int         mHeight;
      bool        mBottomLeftOrigin;
      vector<Tag> mTags;
};

#endif
