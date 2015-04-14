#include <iostream>
#include <cstdlib>
#include <string>
#include "P5PGM.cpp"
#include "P6PPM.cpp"
using namespace std;

// Functions
P5PGM computeRImage( double, const P5PGM&, const P5PGM&, const P5PGM& );

P5PGM computeLocalMaximaImage( const P5PGM& );

P6PPM computeCornerOverlayImage( const P5PGM&, const P5PGM& );

// Main
int main(int argc, char** argv)
{
	// Check the arguments
	if( argc < 2 )
	{
		// Return
		cout << "./a.out [image filename]" << endl;
		return -1;
	}

	// Initialize variables
	string filename(argv[1]);
	P5PGM image;
	Mask sigmaI;
	Mask sigmaD;

	// Read the image
	image.read( (filename + ".pgm").c_str() );

	// Compute the Ix and Iy images
	sigmaD.buildGaussFirstDeriv( 0.7 * 1.5 );
	P5PGM Ix = image.convolveX(sigmaD);
	P5PGM Iy = image.convolveY(sigmaD);

	// Compute Ix^2 , Iy^2 , and IxIy
	P5PGM Ix2 = Ix.multiply(Ix);
	P5PGM Iy2 = Iy.multiply(Iy);
	P5PGM IxIy = Ix.multiply(Iy);

	// Print the images
	Ix2.write( (filename + "_Ix2.pgm").c_str() );
	Iy2.write( (filename + "_Iy2.pgm").c_str() );
	IxIy.write( (filename + "_IxIy.pgm").c_str() );

	// Convolve Ix^2 , Iy^2 , and IxIy with a larger gaussian
	sigmaI.buildGauss( 1.5 );
	P5PGM Ix2_Aw = Ix2.convolve(sigmaI);
	P5PGM Iy2_Aw = Iy2.convolve(sigmaI);
	P5PGM IxIy_Aw = IxIy.convolve(sigmaI);

	// Compute the R(Aw) image
	P5PGM rImage = computeRImage( 0.06, Ix2_Aw, Iy2_Aw, IxIy_Aw );
	rImage.write( (filename + "_R(Aw).pgm").c_str() );

	// Compute the R(Aw) image after a 3x3 local maxima
	P5PGM rImageMaxima = computeLocalMaximaImage( rImage );
	rImageMaxima.write( (filename + "_R(Aw)LocalMaxima.pgm").c_str() );

	// Get the corner overlay image
	P6PGM overlayImage = computeCornerOverlayImage( image, rImageMaxima );
	overlayImage.write( (filename + "_CornerOverlay.ppm").c_str() );

	// Return
	return 0;
}

// Implemented Functions
P5PGM computeRImage( double alpha, const P5PGM& Ix2, const P5PGM& Iy2, const P5PGM& IxIy )
{
	// Initialize variables
	int width = Ix2.getWidth();
	int height = Ix2.getHeight();
	double max = -1000000.0;
	P5PGM result( height, width );

	// All operations are performed per pixel
	for( int i = 0; i < height; ++i )
	{
		for( int j = 0; j < width; ++j )
		{
			// Equation for R(Aw) = det(Aw) - alpha * trace^2(Aw)
			double det = ( Ix2.at(i,j) * Iy2.at(i,j) ) - ( IxIy.at(i,j) * IxIy.at(i,j) );
			double trace = Ix2.at(i,j) + Iy2.at(i,j);
			result.at( i, j ) = det - (alpha * trace * trace);

			// Check if new max
			if( result.at(i,j) > max ) { max = result.at(i,j); }
		}
	}

	// Loop again and threshold
	max = max * 0.01;
	for( int i = 0; i < height; ++i )
	{
		for( int j = 0; j < width; ++j )
		{
			// If above thresh
			if( result.at(i,j) <= max ) { result.at(i,j) = 0.0; }
		}
	} 

	// Return
	return result;
}

P5PGM computeLocalMaximaImage( const P5PGM& src )
{
	// Initialize variables
	int width = src.getWidth();
	int height = src.getHeight();
	P5PGM result( height, width );

	// Loop through image
	for( int i = 0; i < height; ++i )
	{
		for( int j = 0; j < width; ++j )
		{
			// Initialize some more variables
			double max = 0.0;

			// Find the max in a 3x3 neighborhood
			for( int k = -1; k <= 1; ++k )
			{
				for( int l = -1; l <= 1; ++l )
				{
					if( (i+k) >= 0 && (i+k) < height && (j+l) >= 0 && (j+l) < width )
					{
						if( src.at(i+k, j+l) > max ) { max = src.at(i+k, j+l); }
					}
				}
			}

			// If it was not the max, reject corner
			if( src.at(i,j) != max )
			{
				result.at(i,j) = 0;
			}
			else
			{
				result.at(i,j) = max;
			}
		}
	}

	// Return
	return result;
}

P6PPM computeCornerOverlayImage( const P5PGM& src, const P5PGM& corners )
{
	// Initialize
	int width = src.getWidth();
	int height = src.getHeight();
	P6PPM result( src );

	// Loop through image
	for( int i = 0; i < height; ++i )
	{
		for( int j = 0; j < width; ++j )
		{
			// Check if greater than 0
			if( corners.at(i,j) > 0 )
			{
				result.at(i,j,0) = 0;
				result.at(i,j,1) = 255;
				result.at(i,j,2) = 0;
			}
		}
	}

	// Return
	return result;
}




