// Includes
#include "P5PGM.hpp"

// Functions
P5PGM::P5PGM( const char* filename )
{
	// Initialize variables
	char buf[100];
	ifstream img( filename );

	// Read
	img.getline( buf, 100 );

	// If the line is a comment
	while( img.peek() == '#' )
	{
		// Read the next line
		img.getline( buf, 100 );
	}

	// Read width and height
	img >> width >> height;

	// Allocate the data array and read
	data = new byte*[ height ];
	for( int i = 0; i < height; ++i )
	{
		// Allocate the row
		data[i] = new byte[ width ];

		// Read the row
		for( int j = 0; j < width; ++j )
		{
			img >> data[i][j];
		}
	}
}

P5PGM::P5PGM( const P5PGM& src )
{
	// Deep copy the fields
	width = src.width;
	height = src.height;
	
	// Deep copy the image
	data = new byte*[ height ];
	for( int i = 0; i < height; ++i )
	{
		// Allocate the row
		data[i] = new byte[ width ];

		// Copy
		for( int j = 0; j < width; ++j )
		{
			data[i][j] = src.data[i][j];
		}
	}
}

bool P5PGM::write( const char* filename )
{
	// Initialize variables
	ofstream output( filename );

	// Write out in P5 format
	output << "P5" << endl;
	output << width << ' ' << height << endl;
	for( int i = 0; i < height; ++i )
	{
		for( int j = 0; j < width; ++j )
		{
			output << data[i][j];
		}
	}
}

bool P5PGM::convolve1DVert( double* vec )
{

}

bool P5PGM::convolve2DVert( double* vec )
{

}

bool P5PGM::convolve2D( double** mask )
{

}

		int width;
		int height;
		byte image**;
