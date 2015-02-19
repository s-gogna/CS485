#include <cstdlib>
#include <iostream>
#include <fstream>
#include "P5PGM.cpp"
using namespace std;

// Main
int main( int argc, char** argv )
{
	// Check the arguments
	if( argc < 2 )
	{
		// Output an error and return
		cout << "Invalid console arguments. Do ./program sigma" << endl;
		return -1;
	}

	// Create the mask
	Mask1D m1( atof( argv[1] ) );

	// Read the image
	P5PGM lenna( "lenna.pgm" );

	///////////////////////////////////////////////////////////////////////
	// Part C
	///////////////////////////////////////////////////////////////////////
	lenna.convolve2D( m1 ).write( "Lenna2D.pgm" );

	///////////////////////////////////////////////////////////////////////
	// Part D
	///////////////////////////////////////////////////////////////////////
	lenna.convolve1D( m1 ).write( "Lenna1D.pgm" );

	// Return
	return 0;
}
