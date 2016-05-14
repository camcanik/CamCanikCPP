#include <iostream>
#include "svg.h"
#include "tag.h"
#include "util.h"
#include <cmath>

using std::cout;
using std::endl;
using std::abs;
using std::atan2;

struct Point
{
	int x;
	int y;
};
struct Rect
{
	int x, y, width, height;
	Point topLeft, topRight;

	void SetPoints()
	{
		topLeft.x = x;
		topLeft.y = y;

		topRight.x = x+width;
		topRight.y = y;
	}	
};

static vector<Point> getBorderCorners(vector<Rect> rects, int width)
{
	int currentY = 0;
	vector<Point> borderPoints;
	Point p;
	p.x = 0, p.y = 0;
	for (int x = 0; x < width; x++) // for each pixel on the grid, from left to right
	{
		for (int i = 0; i < rects.size(); i++)
		{
			if (rects[i].topLeft.x == x && rects[i].topLeft.y > currentY) //if we encounter new higher top left, ascending y
			{
				p.x = x;
				p.y = currentY;				//set point at intersection of boxes
				borderPoints.push_back(p);

				currentY = rects[i].topLeft.y;
				p.y = currentY;				//update height to corner
				borderPoints.push_back(p);
			}
			else if (rects[i].topRight.x == x && rects[i].topRight.y == currentY) // can't ascend, must descend to next highest block or ground
			{
				int newY = 0; // if nothing else, we drop to ground
				for (int j = 0; j < rects.size(); j++) // again, for all rects
				{
					if (i == j) //skip itself
						continue;

					if (rects[j].topLeft.x < x && x < rects[j].topRight.x) //if different rect contains x in boundaries
					{
						if (rects[j].topLeft.y > newY) //across all rects, if higher y, set y to highest box
							newY = rects[j].topLeft.y;
					}
				}

				p.x = x;					//this is rects[i].topright, the point of descending
				p.y = currentY;
				borderPoints.push_back(p);

				currentY = newY;			//this is intersection with new highest box, continue right till next point
				p.y = currentY;
				borderPoints.push_back(p);
			}
		}
	}
	return borderPoints;
}

static void drawBorder(SVG *ptr1, SVG *ptr2, vector<Point> borderPoints)
{
	for (int i = 0; i < borderPoints.size()-1; i++)
	{
		ptr1->line(borderPoints[i].x, borderPoints[i].y, borderPoints[i + 1].x, borderPoints[i + 1].y, Color::orangered);
		ptr2->line(borderPoints[i].x, borderPoints[i].y, borderPoints[i + 1].x, borderPoints[i + 1].y, Color::black);
	}
}
static string getRandomColor()
{
	static vector<string> colors = Color::drawingColors();

	return(colors[Util::getRandomValue() % colors.size()]);
}

static vector<Rect> placeRect(SVG *ptr, int width, int height, int count, int margin, int minWidth, int minHeight)
{
	vector<Rect> allRects;
	for (int i = 0; i < count; i++)
	{
		bool okay = false;

		int x, y, w, h;
		Rect r;
		
		while (!okay)
		{
			bool inBounds = false;

			w = Util::getRandomValue() % (width-margin);
			h = Util::getRandomValue() % (height-margin);
			x = Util::getRandomValue() % (width- w) + margin; // x-coord
			y = 0; // y-coord

			int xBound = x + w;
			int yBound = y + h;

			if (xBound < width - margin &&  w> minWidth && h > minHeight) //check if in boundaries
			{
				inBounds = true;
			}
			else
				continue;

			if (inBounds) // if its in bounds, continue
			{
				okay = true;
				
			}				
		}
		
		r.height = h;
		r.width = w;
		r.x = x;
		r.y = y+h;
		r.SetPoints();

		ptr->rect(x, y, w, h, Color::black);

		allRects.push_back(r);

	}
	return allRects;
}

int main()
{
	const int WIDTH = 640;
	const int HEIGHT = 480;
	const int COUNT = 5;
	const int MIN_WIDTH = 10;
	const int MIN_HEIGHT = 10;
	const int MARGIN = 5;
	const int DESIRED_ANGLE = 45;

	SVG svg1(WIDTH, HEIGHT, true);
	SVG svg2(WIDTH, HEIGHT, true);
	svg1.rect(0, 0, WIDTH, HEIGHT, Color::black); //draw border
	svg2.rect(0, 0, WIDTH, HEIGHT, Color::black); //draw border

	vector<Rect> allRects;

	allRects = placeRect(&svg1, WIDTH, HEIGHT, COUNT, MARGIN, MIN_WIDTH, MIN_HEIGHT);

	vector<Point> borderPoints;

	borderPoints = getBorderCorners(allRects, WIDTH);

	drawBorder(&svg1, &svg2, borderPoints);

	string errMsg, fileName1 = "redline.html", fileName2 = "skyline.html";

	if (!svg1.writeToFile(fileName1, errMsg))
		cout << errMsg << endl;
	else
		cout << "Wrote File: [" << fileName1 << "]" << endl; // write to file

	if (!svg2.writeToFile(fileName2, errMsg))
		cout << errMsg << endl;
	else
		cout << "Wrote File: [" << fileName2 << "]" << endl; // write to file

	getchar();

	return(0);
}

/*
-Flow-
Place Rects 
Find bordering points
Draw Line
*/


