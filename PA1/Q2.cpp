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
	float thresh = atof( argv[1] );

	// Read the image
	P5PGM lenna( "lenna.pgm" );
	P5PGM sf( "sf.pgm" );

	// Define the sobel operators
	const float horizSobel[3][3] = { { -1, 0, 1 }, {-2, 0, 2}, {-1, 0, 1} };
	const float vertSobel[3][3] = { { -1, -2, -1 }, {0, 0, 0}, {1, 2, 1} };

	///////////////////////////////////////////////////////////////////////
	// Part A, B, C, D
	///////////////////////////////////////////////////////////////////////
	P5PGM hLenna = lenna.convolve2D( 3, horizSobel );
	hLenna.write( "LennaHorizSobel.pgm" );
	P5PGM vLenna = lenna.convolve2D( 3, vertSobel );
	vLenna.write( "LennaVertSobel.pgm" );
	
	P5PGM hSF = sf.convolve2D( 3, horizSobel );
	hSF.write( "SFHorizSobel.pgm" );
	P5PGM vSF = sf.convolve2D( 3, vertSobel );
	vSF.write( "SFVertSobel.pgm" );

	P5PGM::magnitudeOf( hLenna, vLenna ).write( "LennaMagnitude.pgm" );
	P5PGM::directionOf( vLenna, hLenna ).write( "LennaDirection.pgm" );

	P5PGM::magnitudeOf( hSF, vSF ).write( "SFMagnitude.pgm" );
	P5PGM::directionOf( vSF, hSF ).write( "SFDirection.pgm" );

	///////////////////////////////////////////////////////////////////////
	// Part E
	///////////////////////////////////////////////////////////////////////
	P5PGM lennaThresh = P5PGM::magnitudeOf( hLenna, vLenna ).threshold( thresh );
	P5PGM sfThresh = P5PGM::magnitudeOf( hSF, vSF ).threshold( thresh );

	lennaThresh.write( "LennaThresholded.pgm" );
	sfThresh.write( "SFThresholded.pgm" );

	// Return
	return 0;
}
