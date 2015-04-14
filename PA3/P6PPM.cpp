#ifndef P6PPM_CPP
#define P6PPM_CPP
#include <iostream>
#include <fstream>
#include "Mask.cpp"
#include "P5PGM.cpp"
using namespace std;

class P6PPM
{
	public:
		P6PPM();
		P6PPM( int Width, int Height );
		P6PPM( const P5PGM& );
		P6PPM( const P6PPM& );
		~P6PPM();
		P6PPM& operator=( const P6PPM& );

		int getWidth() const { return width; }
		int getHeight() const { return height; }

		void read( const char* );
		void write( const char* );

		void drawCircleAt( int row, int col, int r, int g, int b, double radius );
		void drawCrossAt( int row, int col, int r, int g, int b, int size );

		double& at( int row, int col, int dim ) const { return data[row][col][dim]; }

	private:
		double*** data;
		int width;
		int height;
};

P6PPM::P6PPM()
{
	// Initialize
	width = height = 0;
	data = NULL;
}

P6PPM::P6PPM( int Width, int Height )
{
	// Initialize
	width = Width;
	height = Height;
	data = new double**[height];
	for(int i = 0; i < height; ++i )
	{
		data[i] = new double*[width];
		for(int j = 0; j < width; ++j )
		{
			data[i][j] = new double[3];
			data[i][j][0] = 0;
			data[i][j][1] = 0;
			data[i][j][2] = 0;
		}
	}
}

P6PPM::P6PPM( const P5PGM& src )
{
	// Copy fields
	width = src.getWidth();
	height = src.getHeight();

	// Copy data
	data = new double**[height];
	for(int i = 0; i < height; ++i )
	{
		data[i] = new double*[width];
		for(int j = 0; j < width; ++j )
		{
			data[i][j] = new double[3];
			data[i][j][0] = src.at(i,j);
			data[i][j][1] = src.at(i,j);
			data[i][j][2] = src.at(i,j);
		}
	}
}

P6PPM::P6PPM( const P6PPM& src )
{
	// Copy fields
	width = src.width;
	height = src.height;

	// Copy data
	data = new double**[height];
	for(int i = 0; i < height; ++i )
	{
		data[i] = new double*[width];
		for(int j = 0; j < width; ++j )
		{
			data[i][j] = new double[3];
			data[i][j][0] = src.data[i][j][0];
			data[i][j][1] = src.data[i][j][1];
			data[i][j][2] = src.data[i][j][2];
		}
	}
}

P6PPM::~P6PPM()
{
	// Reset
	width = height = 0;
	for( int i = 0; i < height; ++i )
	{
		delete[] data[i];
	}
	delete[] data;
	data = NULL;
}

P6PPM& P6PPM::operator=( const P6PPM& src )
{
	// Check if self
	if( &src == this ) { return *this; }

	// Check if not the same size
	if( width != src.width || height != src.height )
	{
		// Reset data
		for( int i = 0; i < height; ++i )
		{
			for( int j = 0; j < width; ++j )
			{
				delete[] data[i][j];
			}
			delete[] data[i];
		}
		delete[] data;

		// Copy fields
		width = src.width;
		height = src.height;
	}

	// Copy data
	data = new double**[height];
	for(int i = 0; i < height; ++i )
	{
		data[i] = new double*[width];
		for(int j = 0; j < width; ++j )
		{
			data[i][j] = new double[3];
			data[i][j][0] = src.data[i][j][0];
			data[i][j][1] = src.data[i][j][1];
			data[i][j][2] = src.data[i][j][2];
		}
	}

	// Return
	return *this;
}

void P6PPM::read( const char* filename )
{
	// Initialize variables
	char buf[255];
	ifstream infile(filename);

	// Read
	infile >> buf;
	infile.get();

	// While there are comments
	while( infile.peek() == '#' ) { infile.getline(buf, 255); }

	// Read
	infile >> width >> height >> buf;
	infile.get();

	// Loop and read
	data = new double**[height];
	for(int i = 0; i < height; ++i )
	{
		data[i] = new double*[width];
		for(int j = 0; j < width; ++j )
		{
			data[i][j] = new double[3];
			data[i][j][0] = infile.get();
			data[i][j][1] = infile.get();
			data[i][j][2] = infile.get();
		}
	}
}

void P6PPM::write( const char* filename )
{
	// Initialize variables
	ofstream outfile(filename);

	// Output
	outfile << "P6\n" << width << ' ' << height << '\n' << 255 << '\n';

	// Loop and write
	for( int i = 0; i < height; ++i )
	{
		for( int j = 0; j < width; ++j )
		{
			for( int k = 0; k < 3; ++k )
			{
				if(data[i][j][k] > 255)
				{
					outfile << (unsigned char) 255;
				}
				else if(data[i][j][k] < 0)
				{
					outfile << (unsigned char) 0;
				}
				else
				{
					outfile << (unsigned char) data[i][j][k];
				}
			}
		}
	}
}

void P6PPM::drawCircleAt( int row, int col, int r, int g, int b, double radius )
{
	// Initialize variables
	int intRadius = int(radius) + 1;

	// Loop in a square the size of the radius
	for( int i = -intRadius; i <= intRadius; ++i )
	{
		for( int j = -intRadius; j <= intRadius; ++j )
		{
			// Check if coordinate is inside the image
			if( row+i >= 0 && row+i < height && col+j >= 0 && col+j < width )
			{
				// Check if the value is on the circle
				if( ((intRadius*intRadius) - (i*i + j*j)) > 0.5 )
				{
					// Change the color of the pixel
					data[row + i][col + j][0] = r;
					data[row + i][col + j][1] = g;
					data[row + i][col + j][2] = b;
				}
			}
		}
	}
}

void P6PPM::drawCrossAt( int row, int col, int r, int g, int b, int size )
{
	// Loop in the Y
	for( int i = -size; i <= size; ++i )
	{
		// Check if coordinate is inside the image
		if( row+i >= 0 && row+i < height )
		{
			// Change the color of the pixel
			data[row + i][col][0] = r;
			data[row + i][col][1] = g;
			data[row + i][col][2] = b;
		}
	}

	// Loop in the X
	for( int i = -size; i <= size; ++i )
	{
		// Check if coordinate is inside the image
		if( col+i >= 0 && col+i < width )
		{
			// Change the color of the pixel
			data[row][col + i][0] = r;
			data[row][col + i][1] = g;
			data[row][col + i][2] = b;
		}
	}
}

#endif
