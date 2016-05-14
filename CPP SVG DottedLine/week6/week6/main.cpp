#include <iostream>
#include "svg.h"
#include "tag.h"
#include "util.h"
#include <cmath>

#define PI 3.14159265
using std::cout;
using std::endl;
using std::abs;
using std::atan2;

struct Point
{
	int x;
	int y;
};

static double findDistance(Point A, Point B)
{
	//pythagorean theorem a^2 + b^2 = c^2
	double a,b,c;

	//length of triangle leg a
	a = abs(A.x - B.x);

	//length of triangle leg b
	b = abs(A.y - B.y);

	//hypotenuse^2 = a^2 + b^2
	c = pow(a, 2) + pow(b, 2);

	c = sqrt(c);

	return c;
}

static double findAngle(Point start, Point finish)
{
	double deltaX = finish.x - start.x;
	double deltaY = finish.y - start.y;

	double angle = atan2(deltaY,deltaX) * 180 / PI;
	//credit to stack overflow ^
	return angle;
}

static vector<Point> findBestPoints(vector<Point> allPoints,int desiredAngle)
{
	vector<Point> bestPoints;

	Point bestStart = allPoints[0], bestEnd=allPoints[1];
	double angle, bestAngle=findAngle(bestStart,bestEnd);

	for (int start = 0; start < allPoints.size(); start++) //walk through all points
	{
		for (int end = 0; end < allPoints.size(); end++) //from start point, check all other points
		{
			if (start == end) //point can't check angle with itself
				continue;
			
			angle = findAngle(allPoints[start], allPoints[end]);

			if (angle == desiredAngle) //if its exactly the angle, return
			{
				bestAngle = angle;
				bestStart = allPoints[start];
				bestEnd = allPoints[end];

				bestPoints.push_back(bestStart);
				bestPoints.push_back(bestEnd);

				return bestPoints;
			}
			else if (abs(angle-desiredAngle)< abs(bestAngle-desiredAngle)) //find the absolute value of the difference in angles. Essentially a min routine
			{
				bestAngle = angle;
				bestStart = allPoints[start];
				bestEnd = allPoints[end];

				//for debugging
				//cout << "Best angle now: " << angle << endl;
				//cout << "Best start: " << bestStart.x << "," << bestStart.y << endl;
				//cout << "Best end: " << bestEnd.x << "," << bestEnd.y << endl;
			}
		}
	}

	//finally add the best 2 points to the vector
	bestPoints.push_back(bestStart);
	bestPoints.push_back(bestEnd);

	return bestPoints;
}

static string getRandomColor()
{
	static vector<string> colors = Color::drawingColors();

	return(colors[Util::getRandomValue() % colors.size()]);
}

static vector<Point> drawCircles(SVG *ptr, vector<Point> allPoints, int width, int height, int count, int margin, int minDist)
{	
	for (int i = 0; i < count; i++)
	{
		bool okay = false;

		int x, y, r;
		Point p;

		while (!okay)
		{
			bool inBounds = false, tooClose = false;

			r = 3; //radius
			x = Util::getRandomValue() % (width - r) + margin; // x-coord
			y = Util::getRandomValue() % (height - r) + margin; // y-coord

			p.x = x; //set point value
			p.y = y;

			int xBound = x + r;
			int yBound = y + r;

			if (allPoints.size() == 0) //first point can't compare to other points
				tooClose = false;
			else
			{
				for (int i = 0; i < allPoints.size(); i++)
				{
					if (findDistance(p, allPoints[i]) < minDist) //check distance. break if too close
					{
						tooClose = true;
						break;
					}						
				}
			}	
			
			if (xBound < width - margin && yBound < height - margin && x - r> margin && y - r > margin) //check if in boundaries
			{
				inBounds = true;
			}
			
			if (inBounds && !tooClose) // if its in bounds and not too close to another point, continue
				okay = true;
		}

		const string cBorder = Color::black, cFill = Color::black;

		ptr->circle(x, y, r, cBorder, cFill);
		
		p.x = x;
		p.y = y;

		allPoints.push_back(p);
	}
	return allPoints;
}

static void drawDottedLine(Point s, Point f, SVG *ptr) //pick 20+ points between the start and finish, then draw lines from the points to create a dotted line
{
	vector<Point> dots;
	double dist = findDistance(s, f); //distance from start to finish
	double angle = findAngle(s, f);  // angle of the points
	double x, y;

	//---this part gets weird as it was revised several times---\\
	//also, formula for this was courtesy of my math-friend

	//set a point to the origin
	Point p;
	p.x = s.x;
	p.y = s.y;

	for (int i = 0; i < 22; i++)
	{		
		if (i == 0) //if its the first point, its the origin, add it to vector
		{
			dots.push_back(p);
			continue;
		}
		double divisor = (double)1 / 20; //divisor is distance modifier. 20 points means they are i/20ths the way there
		
		x = cos(angle * PI / 180); //need to multiply/divide again so angle works. Cos value
		x *= dist;	//total distance
		x *= divisor; //20th of the distance
		
		y = sin(angle * PI / 180); //^ Sin Value
		y *= dist;
		y *= divisor;
		
		//cout << i <<") x: " << p.x+x << " y: " << p.y+y << endl; the coords of dotted line segments
		
		p.x += x; //take previous point and add x/y values to move to new position
		p.y += y;

		dots.push_back(p); // add to vector. Rinse, lather, repeat
	}

	for (int i = 1; i < 20; i++) //starting at 1 makes the line look correct. 0 is too short
	{
		ptr->line(dots[i].x, dots[i].y, dots[i+1].x, dots[i+1].y, Color::orangered);
		i++; //skip a point for the spaces
	}
	//ptr->line(s.x, s.y, f.x, f.y, Color::orangered); straight line
}

int main()
{
	const int WIDTH = 640;
	const int HEIGHT = 480;
	const int COUNT = 50;
	const int MIN_DIST = 50;
	const int MARGIN = 5;
	const int DESIRED_ANGLE = 45;

	SVG svg(WIDTH, HEIGHT, true);
	svg.rect(0, 0, WIDTH, HEIGHT, Color::black); //draw border

	vector<Point> allPoints;

	allPoints = drawCircles(&svg, allPoints, WIDTH, HEIGHT, COUNT, MARGIN, MIN_DIST); //draw all points and store them

	vector<Point> bestPoints = findBestPoints(allPoints, DESIRED_ANGLE); //find best set of points

	//cout	<< "The best points are (" << bestPoints[0].x << "," << bestPoints[0].y 
	//		<< ") and (" << bestPoints[1].x << "," << bestPoints[1].y 
	//		<< ") \nwith an angle of " << findAngle(bestPoints[0], bestPoints[1]) << endl << endl;

	drawDottedLine(bestPoints[0], bestPoints[1], &svg); // draw segmented line

	string errMsg, fileName = "index.html";

	if (!svg.writeToFile(fileName, errMsg))
		cout << errMsg << endl;
	else
		cout << "Wrote File: [" << fileName << "]" << endl; // write to file

	//getchar();

	return(0);
}

/*
-Flow-
Draw circles
Find best Angle
Draw Line
*/


