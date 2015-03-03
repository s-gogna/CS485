#include <iostream>
#include <fstream>
using namespace std;

typedef unsigned char byte;

class PGMFile
{
	public:
		PGMFile();
		PGMFile( int, int );
		~PGMFile();
		PGMFile( const PGMFile& );

		void read( const char* filename );
		void write( const char* filename );

		byte& at( int, int );

	private:
		byte** data;
		int width;
		int height;
};

PGMFile::PGMFile()
{
	data = NULL;
	width = height = 0;
}

PGMFile::PGMFile( int h, int w )
{
	height = h;
	width = w;
	data = new byte*[height];
	for( int i = 0; i < height; ++i )
	{
		data[i] = new byte[width];
		for( int j = 0; j < width; ++j )
		{
			data[i][j] = 0;
		}
	}
}

PGMFile::~PGMFile()
{
	// Deallocate memory
	for( int i = 0; i < height; ++i )
	{
		delete[] data[i];
	}
	delete[] data;
	data = NULL;

	// Reset values to 0
	width = height = 0;
}

PGMFile::PGMFile( const PGMFile& src )
{
	// Copy data
	width = src.width;
	height = src.height;

	// Allocate and copy
	data = new byte*[ height ];
	for( int i = 0; i < height; ++i )
	{
		// Allocate
		data[i] = new byte[width];

		// Copy
		for( int j = 0; j < width; ++j )
		{
			data[i][j] = src.data[i][j];
		}
	}
}

void PGMFile::read( const char* filename )
{
	// Open the file
	char buf[50];
	ifstream infile( filename );

	// Read the first word
	infile >> buf;

	// Read the width and then height and then the P5 max value
	infile >> width >> height >> buf;

	// Read the newline
	infile.get();

	// Allocate memory
	unsigned char temp;
	data = new byte*[ height ];
	for( int i = 0; i < height; ++i )
	{
		// Allocate
		data[i] = new byte[width];

		// Read
		for( int j = 0; j < width; ++j )
		{
			data[i][j] = infile.get();
		}
	}

	// Close the file
	infile.close();
}

void PGMFile::write( const char* filename )
{
	// Open the file
	ofstream outfile( filename );

	// Output the basic information
	outfile << "P5" << endl << width << ' ' << height << endl << "255" << endl;

	// Loop through the data and output
	for( int i = 0; i < height; ++i )
	{
		for( int j = 0; j < width; ++j )
		{
			outfile << data[i][j];
		}
	}

	// Close the file
	outfile.close();
}

byte& PGMFile::at( int i, int j )
{
	// Check bounds
	if( i >= 0 && i < height && j >= 0 && j < width )
	{
		return data[i][j];
	}	
	else
	{
		throw "Invalid index arguments for at(i,j)";
	}
}






