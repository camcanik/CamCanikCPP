#include <iostream>
#include <cstdlib>
#include <ctime>
#include "pdf.h"

using std::cout;
using std::endl;
using std::string;

struct Point
{
	int x, y;
};
struct Rect
{
	Point tl, tr, bl, br;
	int h, w;

	void SetPoints(int topleftX, int topleftY, int width, int height)
	{
		h = height;
		w = width;

		tl.x = topleftX;
		tl.y = topleftY;

		tr.x = tl.x + width;
		tr.y = tl.y;

		bl.x = tl.x;
		bl.y = tl.y + height;

		br.x = tr.x;
		br.y = bl.y;
	}

	bool isWiderThanTall()
	{
		if (w > h)
			return true;
		else
			return false;
	}
};

static int getRandomValue()
{
	static bool first = true;

	if (first)
	{
		first = false;
		srand((int)time(0));
	}

	return(rand());
}

static void splitRect(vector<Rect> &rects)
{
	int startingSize = rects.size();

	Rect r1, r2;
	int splitpoint;

	vector<Rect> newRects;

	for (int i = 0; i < startingSize; i++)
	{
		if (rects[i].isWiderThanTall()) // split vertically
		{
			splitpoint = (getRandomValue() % 50) + 25; // will be randomly placed 25-75% in rect
			double percentage = (double)splitpoint / 100.00; //ex. splitpoint = 50, percentage = .50
			splitpoint = rects[i].w * percentage;

			r1.SetPoints(rects[i].tl.x, rects[i].tl.y,splitpoint, rects[i].h);
			r2.SetPoints(r1.tl.x + splitpoint, r1.tl.y, rects[i].w - splitpoint, rects[i].h);

			//cout << "R1 (tl.x, tl.y, w, h) (" << r1.tl.x << ", " << r1.tl.y << ", " << r1.w << ", " << r1.h <<")" <<endl;
			//cout << "R2 (tl.x, tl.y, w, h) (" << r2.tl.x << ", " << r2.tl.y << ", " << r2.w << ", " << r2.h << ")" << endl;

			newRects.push_back(r1);
			newRects.push_back(r2);
		}
		else	// split horizontally
		{
			splitpoint = (getRandomValue() % 50) + 25; // will be randomly placed 25-75% in rect
			double percentage = (double)splitpoint / 100.00; //ex. splitpoint = 50, percentage = .50
			splitpoint = rects[i].h * percentage;

			r1.SetPoints(rects[i].tl.x, rects[i].tl.y, rects[i].w,  splitpoint);
			r2.SetPoints(r1.tl.x, r1.tl.y + splitpoint, rects[i].w, (rects[i].h - splitpoint));

			//cout << "R1 (tl.x, tl.y, w, h) (" << r1.tl.x << ", " << r1.tl.y << ", " << r1.w << ", " << r1.h << ")" << endl;
			//cout << "R2 (tl.x, tl.y, w, h) (" << r2.tl.x << ", " << r2.tl.y << ", " << r2.w << ", " << r2.h << ")" << endl;

			newRects.push_back(r1);
			newRects.push_back(r2);

		}
	}



	rects = newRects;
	
}

static void randomSplit(vector<Rect> &rects)
{
	int r;
	bool split = false;
	int count = 0;

	Rect r1, r2;
	int splitpoint;

	while (!split || count != 5)
	{
		r = getRandomValue() % rects.size();

		if (rects[r].w > 50 && rects[r].h > 50)
		{
			splitpoint = (getRandomValue() % 50) + 25; // will be randomly placed 25% -75% in rect
			double percentage = (double)splitpoint / 100.00; //ex. splitpoint = 50, percentage = .50
			
			if (getRandomValue() % 2 == 0)
			{
				splitpoint = rects[r].w * percentage;

				r1.SetPoints(rects[r].tl.x, rects[r].tl.y, splitpoint, rects[r].h);
				r2.SetPoints(r1.tl.x + splitpoint, r1.tl.y, rects[r].w - splitpoint, rects[r].h);

				rects.erase(rects.begin() + r);

				rects.push_back(r1);
				rects.push_back(r2);

				split = true;
			}
				
			else
			{
				splitpoint = rects[r].h * percentage;

				r1.SetPoints(rects[r].tl.x, rects[r].tl.y, rects[r].w, splitpoint);
				r2.SetPoints(r1.tl.x, r1.tl.y + splitpoint, rects[r].w, (rects[r].h - splitpoint));

				rects.erase(rects.begin() + r);

				rects.push_back(r1);
				rects.push_back(r2);

				split = true;
			}
				


		}

		count++;

	}

}

static void drawAndColorRects(PDF &p, vector<Rect> rects)
{
	int r;
	for (int i = 0; i < rects.size(); i++)
	{
		r = (getRandomValue() % 100);

		if (r >= 1 && r <= 15) // 20% chance to draw red
		{
			p.setFillColor(255, 0, 0);
			p.fillRect(rects[i].tl.x, rects[i].tl.y, rects[i].w, rects[i].h);
		}
		else if (r >= 16 && r <= 30) // 20% chance to draw yellow
		{
			p.setFillColor(255, 255, 0);
			p.fillRect(rects[i].tl.x, rects[i].tl.y, rects[i].w, rects[i].h);
		}
		else if (r >= 31 && r <= 45) // 20% chance to draw blue
		{
			p.setFillColor(0, 0, 255);
			p.fillRect(rects[i].tl.x, rects[i].tl.y, rects[i].w, rects[i].h);
		}

		p.drawRect(rects[i].tl.x, rects[i].tl.y, rects[i].w, rects[i].h);
	}

}

int main()
{
	PDF p;

	vector<Rect> rects;

	int xOffset = 20 + (getRandomValue() % 80);
	int yOffset = 20 + (getRandomValue() % 60);

	int x0 = xOffset;
	int x1 = p.getWidth() - xOffset;
	int y0 = yOffset;
	int y1 = p.getHeight() - yOffset;

	cout << "Rect(" << x0 << ", " << y0 << ", " <<
		x1 << ", " << y1 << ")" << endl;

	int w = (x1 - x0);
	int h = (y1 - y0);

	Rect r;
	r.SetPoints(x0, y0, w, h);
	rects.push_back(r);

	

	while (rects.size() < 14)
	{
		splitRect(rects); //more structured method, doesnt look random enough
		randomSplit(rects); //crazy method, looks too random
		// so I combined 'em
		//cout << rects.size() << endl;
	}
	
	drawAndColorRects(p,rects);

	p.drawRect(x0, y0, w, h);

	string fileName = "mondrian.pdf";
	string errMsg;

	if (!p.writeToFile(fileName, errMsg))
	{
		cout << errMsg << endl;
	}
	else
	{
		cout << "Successfully wrote file: [" << fileName << "]" << endl;
	}

	//getchar();
	return(0);
}


/*

---Flow---

Plot rect
split it
store the new rects
repeat a few times

once split enough, randomly choose which to color
color randomly
output to file

steps:
Split rect in half a few times, make sure its storing
split randomly
determine who to split

*/