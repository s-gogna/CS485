#ifndef P6PGM_CPP
#define P6PGM_CPP
#include <iostream>
#include <fstream>
#include "Mask.cpp"
#include "P5PGM.cpp"
using namespace std;

class P6PGM
{
	public:
		P6PGM();
		P6PGM( int Width, int Height );
		P6PGM( const P5PGM& );
		P6PGM( const P6PGM& );
		~P6PGM();
		P6PGM& operator=( const P6PGM& );

		int getWidth() const { return width; }
		int getHeight() const { return height; }

		void read( const char* );
		void write( const char* );

		double& at( int row, int col, int dim ) const { return data[row][col][dim]; }

	private:
		double*** data;
		int width;
		int height;
};

P6PGM::P6PGM()
{
	// Initialize
	width = height = 0;
	data = NULL;
}

P6PGM::P6PGM( int Width, int Height )
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

P6PGM::P6PGM( const P5PGM& src )
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

P6PGM::P6PGM( const P6PGM& src )
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

P6PGM::~P6PGM()
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

P6PGM& P6PGM::operator=( const P6PGM& src )
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

void P6PGM::read( const char* filename )
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

void P6PGM::write( const char* filename )
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


#endif
