#include <cstdlib>
#include <iostream>
#include <fstream>
#include "P5PGM.cpp"
using namespace std;

// Main
int main( int argc, char** argv )
{
	// Create the mask
	Mask1D mask( 1.0 );

	// Read the image
	P5PGM lenna( "lenna.pgm" );

	///////////////////////////////////////////////////////////////////////
	// Loop
	///////////////////////////////////////////////////////////////////////
	char buf[100];
	for( int i = 256; i >= 4; i /= 2 )
	{
		// Print lenna
		sprintf( buf, "LennaPyr_%i.pgm", i );
		lenna.write( buf );
		
		// Downscale
		lenna = lenna.convolve1D( mask ).downscaleBy2();
	}

	// Return
	return 0;
}
