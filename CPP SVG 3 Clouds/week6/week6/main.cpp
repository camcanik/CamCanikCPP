#include <iostream>
#include "svg.h"
#include "tag.h"
#include "util.h"

using std::cout;
using std::endl;

static string getRandomColor()
{
	static vector<string> colors = Color::drawingColors();

	return(
		colors[Util::getRandomValue() % colors.size()]
		);
}

void testRect(SVG *ptr, int width, int height, int count)
{
	for (int i = 0; i < count; i++)
	{
		bool okay = false;

		int x, y, w, h;

		while (!okay)
		{
			w = Util::getRandomValue() % 51 + 25;
			h = Util::getRandomValue() % 51 + 25;

			x = Util::getRandomValue() % (width - w) + 2;
			y = Util::getRandomValue() % (height - h) + 2;

			int xBound = x + w;
			int yBound = y + h;

			if (xBound < width-2 && yBound < height-2 && x>=2 && y>=2)
				okay = true;
		}

		const string cBorder = getRandomColor(), cFill = getRandomColor();

		ptr->rect(x, y, w, h, cBorder, cFill);
	}
	
}
void testCircle(SVG *ptr, int width, int height, int count)
{
	for (int i = 0; i < count; i++)
	{
		bool okay = false;

		int x, y, r;

		while (!okay)
		{
			r = Util::getRandomValue() % 51 + 25;

			x = Util::getRandomValue() % (width - r) + 2;
			y = Util::getRandomValue() % (height - r) + 2;

			int xBound = x + r;
			int yBound = y + r;

			if (xBound < width-2 && yBound < height-2 && x-r>2 && y-r>2)
				okay = true;
		}

		const string cBorder = getRandomColor(), cFill = getRandomColor();

		ptr->circle(x, y, r, cBorder, cFill);
	}
	
}
void testLine(SVG *ptr, int width, int height, int count)
{
	for (int i = 0; i < count; i++)
	{
		bool okay = false;

		int x1, y1, x2, y2;

		while (!okay)
		{
			x1 = Util::getRandomValue() % (width - 4) + 2;
			y1 = Util::getRandomValue() % (height - 4) + 2;

			x2 = Util::getRandomValue() % (width - 4) + 2;
			y2 = Util::getRandomValue() % (height - 4) + 2;

			if (x1 >= 2 && x1 <= width - 2 && x2 >= 2 && x2 <= width - 2 && y1 >= 2 && y1 <= height - 2 && y2 >= 2 && y2 <= height - 2)
				okay = true;
		}

		const string cLine = getRandomColor();

		ptr->line(x1, y1, x2, y2, cLine);
	}
}


int main()
{
	struct Entry
	{
		string mFileName;
		void(*mTestFunction)(SVG *ptr, int width, int height, int count);
	};

	static Entry entries[] =
	{
		{ "svg-rect.html", testRect },
		{ "svg-circle.html", testCircle },
		{ "svg-line.html", testLine }
	};

	static const int n = sizeof(entries) / sizeof(entries[0]);

	const int WIDTH = 500;
	const int HEIGHT = 500;
	const int COUNT = 100;

	for (int i = 0; i < n; i++)
	{
		SVG svg(WIDTH, HEIGHT, true);

		svg.rect(0, 0, WIDTH, HEIGHT, Color::black);

		entries[i].mTestFunction(&svg, WIDTH, HEIGHT, COUNT);

		string errMsg;

		if (!svg.writeToFile(entries[i].mFileName, errMsg))
			cout << errMsg << endl;

		else
			cout << "Wrote File: [" << entries[i].mFileName << "]" << endl;
	}
	//getchar();

	return(0);
}

