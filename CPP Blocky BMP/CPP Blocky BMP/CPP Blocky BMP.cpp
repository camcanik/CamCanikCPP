#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>

#include "bmp.h"

using std::string;
using std::cout;
using std::endl;
using std::exception;
using std::to_string;
using std::ifstream;
using std::ios;

typedef vector<ColorRow> ColorGrid;

struct Pixel //coordinates and color values
{
	int x;
	int y;
	 
	unsigned char r, g, b;
};
struct Block // set of pixels with the average color
{
	vector<Pixel> pixels;
	unsigned int AverageR = 0, AverageG = 0, AverageB = 0;

	void Block::setAverageColor()
	{
		for (int i = 0; i < pixels.size(); i++)
		{
			AverageR += pixels[i].r;
			AverageG += pixels[i].g;
			AverageB += pixels[i].b;
		}
		AverageR = AverageR / pixels.size() +0.5;
		AverageG = AverageG / pixels.size() + 0.5;
		AverageB = AverageB / pixels.size() + 0.5;
	}
};

static bool readFile( // from reader.cpp, reads bmp from file, returns pixel array
	const string &name,
	ColorGrid    &grid,
	string       &errMsg
	)
{
	ifstream in(name, ios::binary);

	if (!in)
	{
		errMsg = "Could not open file: [" + name + "]";
		return(false);
	}

	const int SIZE = (Bmp::FILE_HEADER_SIZE + Bmp::INFO_HEADER_SIZE);
	unsigned char buffer[SIZE];

	if (!in.read((char *)buffer, SIZE))
	{
		errMsg = "Error reading bmp header, file: [" + name + "]";
		return(false);
	}

	if (buffer[0] != 'B' || buffer[1] != 'M')
	{
		errMsg = "Not a bmp file: [" + name + "]";
		return(false);
	}

	int width = 0;
	int height = 0;

	for (int i = 18, j = 0; i <= 21; i++, j += 8)
		width += (buffer[i] << j);

	for (int i = 22, j = 0; i <= 25; i++, j += 8)
		height += (buffer[i] << j);

	unsigned char color[3];

	for (int i = 0; i < height; i++)
	{
		ColorRow row;

		for (int j = 0; j < width; j++)
		{
			if (!in.read((char *)color, sizeof(color)))
			{
				errMsg = "Error reading color data, file: [" + name + "]";
				return(false);
			}

			// Colors are in the order: { blue, green, red }

			row.push_back(Color(color[2], color[1], color[0]));
		}

		grid.push_back(row);
	}

	 //cout << "w: " << width << ", h: " << height << endl;

	return(true);
}

static vector<Block> setBlocks(int width, int height, int blockSize, ColorGrid grid) //mathematically sets each pixel to a block vector (psuedo 2D vector)
{
	int totalBlocks = ((width*height) / (blockSize*blockSize)) + 0.5;
	vector<Block> Blocks(totalBlocks);
	int blockNum = 0;

	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < width; w++)
		{
			//x + y , x=w/blocksize, y=(width/blocksize)*(h/blocksize)
			blockNum = (w / blockSize) + ((width / blockSize)*(h / blockSize));
			if (blockNum >= Blocks.size()) //in case we need more blocks (redundant, I know, but I got an error because I was off by 1 every so often)
			{
				Block b;
				Blocks.push_back(b);
			}

			Pixel px; //set pixel data

			px.x = w;
			px.y = h;

			px.r = grid[h][w].mRed;
			px.g = grid[h][w].mGreen;
			px.b = grid[h][w].mBlue;

			Blocks[blockNum].pixels.push_back(px); //throw pixel into the appropriate block
		}
	}

	return Blocks;
}


static bool createImage(
	const string &fileName,
	int           blockSize,
	string       &outFileName,
	string       &errMsg
	)
{
	bool okay = false;
	ColorGrid grid;

	try{

		if (!readFile(fileName, grid, errMsg)) // check if file can be read
		{
			cout << errMsg << endl;
			return false;
		}

		int height = grid.size();
		int width = grid[0].size();

		Bmp bmp(width, height);	// get file's dimensions, set our picture to same

		for (int h = 0; h < height; h++)	
		{
			for (int w = 0; w < width; w++)
			{
				bmp.setPixel(w, h, grid[h][w].mRed, grid[h][w].mGreen, grid[h][w].mBlue); //set each pixel to recreate file
			}
		}

		vector<Block> blocks = setBlocks(width, height, blockSize, grid); // now that we have the image, set up grid of blocks for averaging

		for (int i = 0; i < blocks.size(); i++)
		{
			blocks[i].setAverageColor();
			int xDif, yDif;

			xDif = (blocks[i].pixels[blocks[i].pixels.size()-1].x) - (blocks[i].pixels[0].x-1); // should equal blocksize, but if block is fringe (goes offscreen), stick within range
			yDif = (blocks[i].pixels[blocks[i].pixels.size() - 1].y) - (blocks[i].pixels[0].y);
			
			bmp.fillRectangle(blocks[i].pixels[0].x, blocks[i].pixels[0].y, xDif, yDif, blocks[i].AverageR, blocks[i].AverageG, blocks[i].AverageB);
		}

		bmp.drawRectangle(0, 0, blockSize-1, blockSize-1, 0, 0, 0); //test, shows size of blocks

		if (!bmp.write(outFileName, errMsg)) // write the output file
		{
			okay = false;
			cout << errMsg << endl;
		}
		else
		{
			okay = true; // if nothing fails, return success
		}
	}
	catch (exception& e)
	{
		errMsg = e.what(); // catch all errors
	}

	return okay;
}


int main()
{

	string errMsg;
	string outFileName, fileName = "file.bmp";
	int blockSize;

	for (int i = 1; i <= 6; i++) // iterations
	{
		blockSize = pow(2, i); // increase in powers of 2 i.e. 2^1=2, 2^2=4, 2^3=8, etc.

		outFileName = "blocky";
		outFileName += to_string(blockSize);
		outFileName += ".bmp";

		if (!createImage(fileName, blockSize, outFileName, errMsg))
			cout << errMsg << endl;

		else
			cout << "Write file: [" << outFileName << "]" << endl;
	}

	getchar();

	return(0);
}