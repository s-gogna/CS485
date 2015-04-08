#include <iostream>
#include <cstdlib>
#include <string>
#include "P5PGM.cpp"
using namespace std;

// Main
int main(int argc, char** argv)
{
	// Check the arguments
	if( argc < 2 )
	{
		// Return
		cout << "./a.out [threshold] [image filename]" << endl;
		return -1;
	}

	// Initialize variables
	double threshold = atof(argv[1]);
	string filename(argv[2]);
	P5PGM image;
	Mask sigmaI( 1.5, 5 );
	Mask sigmaD( 0.7 * 1.5, 5 );

	// Change the filenames

	// Read the image
	image.read( (filename + ".pgm").c_str() );

	// Convolve with the gaussian in the x and y
	P5PGM Ix = image.convolveX(sigmaD);
	P5PGM Iy = image.convolveY(sigmaD);
	P5PGM Ix2 = Ix.multiply(Ix);
	P5PGM Iy2 = Iy.multiply(Iy);
	P5PGM IxIy = Ix.multiply(Iy);

	// Print the images
	Ix2.write( (filename + "_Ix2.pgm").c_str() );
	Iy2.write( (filename + "_Iy2.pgm").c_str() );
	IxIy.write( (filename + "_IxIy.pgm").c_str() );

	// Return
	return 0;
}
