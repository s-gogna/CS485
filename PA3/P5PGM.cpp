#ifndef P5PGM_CPP
#define P5PGM_CPP
#include <iostream>
#include <fstream>
#include "Mask.cpp"
using namespace std;

class P5PGM
{
	public:
		P5PGM();
		P5PGM( int Width, int Height );
		P5PGM( const P5PGM& );
		~P5PGM();
		P5PGM& operator=( const P5PGM& );

		void read( const char* );
		void write( const char* );

		P5PGM convolve( const Mask& );
		P5PGM convolveX( const Mask& );
		P5PGM convolveY( const Mask& );

		P5PGM multiply( const P5PGM& );

		double& at( int row, int col ) const;

	private:
		double** data;
		int width;
		int height;
};

P5PGM::P5PGM()
{
	// Initialize
	width = height = 0;
	data = NULL;
}

P5PGM::P5PGM( int Width, int Height )
{
	// Initialize
	width = Width;
	height = Height;
	data = new double*[height];
	for(int i = 0; i < height; ++i )
	{
		data[i] = new double[width];
		for(int j = 0; j < width; ++j )
		{
			data[i][j] = 0;
		}
	}
}

P5PGM::P5PGM( const P5PGM& src )
{
	// Copy fields
	width = src.width;
	height = src.height;

	// Copy data
	data = new double*[height];
	for(int i = 0; i < height; ++i )
	{
		data[i] = new double[width];
		for(int j = 0; j < width; ++j )
		{
			data[i][j] = src.data[i][j];
		}
	}
}

P5PGM::~P5PGM()
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

P5PGM& P5PGM::operator=( const P5PGM& src )
{
	// Check if self
	if( &src == this ) { return *this; }

	// Check if not the same size
	if( width != src.width || height != src.height )
	{
		// Reset data
		for( int i = 0; i < height; ++i )
		{
			delete[] data[i];
		}
		delete[] data;

		// Copy fields
		width = src.width;
		height = src.height;
	}

	// Copy data
	data = new double*[height];
	for(int i = 0; i < height; ++i )
	{
		data[i] = new double[width];
		for(int j = 0; j < width; ++j )
		{
			data[i][j] = src.data[i][j];
		}
	}

	// Return
	return *this;
}

void P5PGM::read( const char* filename )
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
	data = new double*[height];
	for( int i = 0; i < height; ++i )
	{
		data[i] = new double[width];
		for( int j = 0; j < width; ++j )
		{
			data[i][j] = infile.get();
		}
	}
}

void P5PGM::write( const char* filename )
{
	// Initialize variables
	ofstream outfile(filename);

	// Output
	outfile << "P5\n" << width << ' ' << height << '\n' << 255 << '\n';

	// Loop and write
	for( int i = 0; i < height; ++i )
	{
		for( int j = 0; j < width; ++j )
		{
			if(data[i][j] > 255)
			{
				outfile << (unsigned char) 255;
			}
			else if(data[i][j] < 0)
			{
				outfile << (unsigned char) 0;
			}
			else
			{
				outfile << (unsigned char) data[i][j];
			}
		}
	}
}

P5PGM P5PGM::convolve( const Mask& mask )
{
	// Initialize variables
	int halfDim = mask.getDim() / 2;
	P5PGM resultInterm(width, height);
	P5PGM result(width, height);

	// Apply mask to the image horizontally
	for( int i = 0; i < height; ++i )
	{
		for( int j = 0; j < width; ++j )
		{
			for( int k = -halfDim; k <= halfDim; ++k )
			{
				// Check if within image
				if( j+k >= 0 && j+k < width )
				{
					resultInterm.data[i][j] += data[i][j + k] * mask.at(k + halfDim);
				}
			}
		}
	}

	// Apply mask to the image vertically
	for( int i = 0; i < height; ++i )
	{
		for( int j = 0; j < width; ++j )
		{
			for( int k = -halfDim; k <= halfDim; ++k )
			{
				// Check if within image
				if( i+k >= 0 && i+k < height )
				{
					result.data[i][j] += resultInterm.data[i + k][j] * mask.at(k + halfDim);
				}
			}
		}
	}

	// Return
	return result;
}

P5PGM P5PGM::convolveX( const Mask& mask )
{
	// Initialize variables
	int halfDim = mask.getDim() / 2;
	P5PGM result(width, height);

	// Apply mask to the image horizontally
	for( int i = 0; i < height; ++i )
	{
		for( int j = 0; j < width; ++j )
		{
			for( int k = -halfDim; k <= halfDim; ++k )
			{
				// Check if within image
				if( j+k >= 0 && j+k < width )
				{
					result.data[i][j] += data[i][j + k] * mask.at(k + halfDim);
				}
			}
		}
	}

	// Return
	return result;
}

P5PGM P5PGM::convolveY( const Mask& mask )
{
	// Initialize variables
	int halfDim = mask.getDim() / 2;
	P5PGM result(width, height);

	// Apply mask to the image horizontally
	for( int i = 0; i < height; ++i )
	{
		for( int j = 0; j < width; ++j )
		{
			for( int k = -halfDim; k <= halfDim; ++k )
			{
				// Check if within image
				if( i+k >= 0 && i+k < height )
				{
					result.data[i][j] += data[i + k][j] * mask.at(k + halfDim);
				}
			}
		}
	}

	// Return
	return result;
}

P5PGM P5PGM::multiply( const P5PGM& src )
{
	// Initialize variables
	P5PGM result(width, height);

	// Make sure dimensions are the same
	if( src.width != width || src.height != height ) { return result; }

	// Loop through and multiply
	for( int i = 0; i < height; ++i )
	{
		for( int j = 0; j < width; ++j )
		{
			result.data[i][j] = data[i][j] * src.data[i][j];
		}
	}

	// Return
	return result;
}

double& P5PGM::at( int row, int col ) const
{
	// Return double
	return data[col][row];
}


#endif
