#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include "util.h"
#include "svg.h"

using std::ostringstream;
using std::ofstream;
using std::cout;
using std::endl;
using std::min;

// -------------------------------------
// Local Function Prototypes
// -------------------------------------

static string getText();

static void handleShapeTag(
   Tag          *tagPtr,
   const string &edgeColor,
   const string &fillColor,
   vector<Tag>  &tags
);

// -------------------------------------
// Color
// -------------------------------------

#define COLOR_ENTRY(name) \
   const string Color::name = #name

COLOR_ENTRY(white);
COLOR_ENTRY(darkgray);
COLOR_ENTRY(black);
COLOR_ENTRY(yellow);
COLOR_ENTRY(greenyellow);
COLOR_ENTRY(lime);
COLOR_ENTRY(darkgreen);
COLOR_ENTRY(lightgreen);
COLOR_ENTRY(seagreen);
COLOR_ENTRY(aquamarine);
COLOR_ENTRY(mediumaquamarine);
COLOR_ENTRY(lightseagreen);
COLOR_ENTRY(paleturquoise);
COLOR_ENTRY(cyan);
COLOR_ENTRY(darkturquoise);
COLOR_ENTRY(deepskyblue);
COLOR_ENTRY(lightskyblue);
COLOR_ENTRY(royalblue);
COLOR_ENTRY(blue);
COLOR_ENTRY(darkblue);
COLOR_ENTRY(lavender);
COLOR_ENTRY(mediumslateblue);
COLOR_ENTRY(blueviolet);
COLOR_ENTRY(purple);
COLOR_ENTRY(orchid);
COLOR_ENTRY(magenta);
COLOR_ENTRY(hotpink);
COLOR_ENTRY(pink);
COLOR_ENTRY(darkred);
COLOR_ENTRY(crimson);
COLOR_ENTRY(orangered);
COLOR_ENTRY(darkorange);
COLOR_ENTRY(gold);
COLOR_ENTRY(lemonchiffon);
COLOR_ENTRY(chocolate);
COLOR_ENTRY(burlywood);

vector<string> Color::all()
{
#define ADD(name) colors.push_back(name)

   static bool first = true;
   static vector<string> colors;

   if(first)
   {
      first = false;

      ADD(white);
      ADD(darkgray);
      ADD(black);
      ADD(yellow);
      ADD(greenyellow);
      ADD(lime);
      ADD(darkgreen);
      ADD(lightgreen);
      ADD(seagreen);
      ADD(aquamarine);
      ADD(mediumaquamarine);
      ADD(lightseagreen);
      ADD(paleturquoise);
      ADD(cyan);
      ADD(darkturquoise);
      ADD(deepskyblue);
      ADD(lightskyblue);
      ADD(royalblue);
      ADD(blue);
      ADD(darkblue);
      ADD(lavender);
      ADD(mediumslateblue);
      ADD(blueviolet);
      ADD(purple);
      ADD(orchid);
      ADD(magenta);
      ADD(hotpink);
      ADD(pink);
      ADD(darkred);
      ADD(crimson);
      ADD(orangered);
      ADD(darkorange);
      ADD(gold);
      ADD(lemonchiffon);
      ADD(chocolate);
      ADD(burlywood);
   }

   return(colors);
}

vector<string> Color::drawingColors()
{
   static bool first = true;
   static vector<string> colors;

   if(first)
   {
      first = false;

      vector<string> list = all();

      static const string toRemove[] =
      {
         Color::white,
         Color::darkgray,
         Color::black,
         Color::darkgreen,
         Color::darkblue,
         Color::darkorange,
         Color::purple,
         Color::darkred,
         Color::lavender
      };

      static const int nRemove = sizeof(toRemove) / sizeof(toRemove[0]);

      for(int i = 0, n = list.size(); i < n; i ++)
      {
         bool found = false;

         for(int j = 0; !found && j < nRemove; j ++)
            if(list[i] == toRemove[j])
               found = true;

         if(!found)
            colors.push_back(list[i]);
      }
   }

   return(colors);
}

// -------------------------------------
// Justify
// -------------------------------------

#define JUSTIFY_ENTRY(name) \
   const string Justify::name = #name

JUSTIFY_ENTRY(start);
JUSTIFY_ENTRY(middle);
JUSTIFY_ENTRY(end);

// -------------------------------------
// SVG - Implementation
// -------------------------------------

SVG::SVG(int width, int height, bool bottomLeftOrigin) :
   mWidth(width), mHeight(height), mBottomLeftOrigin(bottomLeftOrigin)
{
}

void SVG::rect(
   int x, int y,
   int width, int height,
   const string &edgeColor,
   const string &fillColor
)
{
   Tag t("rect");
   
   // For 'rect', (x, y) will be the corner with the
   // smallest coordinate values so use 'min' to ensure
   // this is the case (whether 'mBottomLeftOrigin' is
   // set or not)

   t.addAttribute("x",      x);
   t.addAttribute("y",      min(yValue(y), yValue(y + height)));
   t.addAttribute("width",  width);
   t.addAttribute("height", height);

   handleShapeTag(&t, edgeColor, fillColor, mTags);
}

void SVG::circle(
   int x, int y,
   int radius,
   const string &edgeColor,
   const string &fillColor
)
{
   Tag t("circle");

   t.addAttribute("cx", x);
   t.addAttribute("cy", yValue(y));
   t.addAttribute("r",  radius);

   handleShapeTag(&t, edgeColor, fillColor, mTags);
}

void SVG::line(int x1, int y1, int x2, int y2, const string &color)
{
   Tag t("line");

   t.addAttribute("x1",     x1);
   t.addAttribute("y1",     yValue(y1));
   t.addAttribute("x2",     x2);
   t.addAttribute("y2",     yValue(y2));
   t.addAttribute("stroke", color);

   mTags.push_back(t);
}

void SVG::polyline(const vector<int> &points, const string &color)
{
   Tag t("polyline");

   ostringstream out;

   for(int i = 0, n = points.size(); i < n; i += 2)
      out << (i == 0 ? "" : ",") << points[i] << "," << points[i + 1];

   t.addAttribute("points", out.str());
   t.addAttribute("stroke", color);

   mTags.push_back(t);
}

void SVG::text(
   int x, int y, const string &text, const string &color, string justify
)
{
   Tag t("text");

   t.addAttribute("x", x);
   t.addAttribute("y", yValue(y));
   t.addAttribute("fill", color);

   if(justify != Justify::start)
      t.addAttribute("text-anchor", justify);

   t.setTarget(text);

   mTags.push_back(t);
}

string SVG::toString() const
{
   ostringstream out;

   for(int i = 0, n = mTags.size(); i < n; i ++)
      out << "   " << mTags[i].toString() << (i + 1 == n ? "" : "\n");

   vector<string> values;

   values.push_back("SVG Output");
   values.push_back(Util::toString(mWidth));
   values.push_back(Util::toString(mHeight));
   values.push_back(out.str());

   return(
      Util::doReplace(getText(), values)
   );
}

bool SVG::writeToFile(const string &fileName, string &errMsg)
{
   ofstream out(fileName);

   if(!out)
   {
      errMsg = "Could not open file: [" + fileName + "]";
      return(false);
   }

   out << toString();

   return(true);
}

int SVG::yValue(int value) const
{
   int result = value;

   if(mBottomLeftOrigin)
      result = mHeight - value;

   return(result);
}

// -------------------------------------
// Local Functions
// -------------------------------------

static string getText()
{
   static string text =

      "<html>"                                          "\n"
      " <head><title>{0}</title></head>"                "\n"
      " <body>"                                         "\n" 
      "  <svg"                                          "\n"
      "   xmlns=\"http://www.w3.org/2000/svg\""         "\n"
      "   xmlns:xlink=\"http://www.w3.org/1999/xlink\"" "\n"
      "   width=\"{1}\""                                "\n"
      "   height=\"{2}\">"                              "\n"
      ""                                                "\n"
      "{3}"                                             "\n"
      ""                                                "\n"
      "  </svg>"                                        "\n"
      " </body>"                                        "\n" 
      "</html>"                                         "\n";

   return(text);
}

static void handleShapeTag(
   Tag *tagPtr,
   const string &edgeColor,
   const string &fillColor,
   vector<Tag> &tags
)
{
   tagPtr->addAttribute("fill", fillColor == "" ? "none" : fillColor);

   if(edgeColor != "")
      tagPtr->addAttribute("stroke", edgeColor);

   tags.push_back(*tagPtr);
}
