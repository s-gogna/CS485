#include <iostream>
#include <cstdlib>
#include <cmath>
#include <string>
#include "P5PGM.cpp"
#include "P6PGM.cpp"
using namespace std;

// Structs
struct ScaleImage
{
	double sigmaI;
	double sigmaD;
	P5PGM image;
};

// Functions
P5PGM computeRImage( double, const P5PGM&, const P5PGM&, const P5PGM& );

P5PGM computeLocalMaximaImage( const P5PGM& );

P6PGM computeCornerOverlayImage( const P5PGM&, const P5PGM& );

P5PGM computeHarrisCornerImage( double, double, P5PGM& );

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
	char buf[16];
	string filename(argv[1]);
	P5PGM image;
	ScaleImage* gss = new ScaleImage[18];

	// Read the image
	image.read( (filename + ".pgm").c_str() );

	// Loop and fill the Gaussian Scale Space
	for( int k = 0; k < 18; ++k )
	{
		// Initialize some values
		gss[k].sigmaI = 1.5 * pow( 1.2, k );
		gss[k].sigmaD = 0.7 * gss[k].sigmaI;

		// Compute the Harris Corner Image
		gss[k].image = computeHarrisCornerImage( gss[k].sigmaD, gss[k].sigmaI, image );
		sprintf(buf, "%d", k);
		gss[k].image.write( (filename + "_N" + string(buf) + ".pgm").c_str() );
	}

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
	P5PGM result( width, height );

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
	P5PGM result( width, height );

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

P6PGM computeCornerOverlayImage( const P5PGM& src, const P5PGM& corners )
{
	// Initialize
	int width = src.getWidth();
	int height = src.getHeight();
	P6PGM result( src );

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

P5PGM computeHarrisCornerImage( double sigmaD, double sigmaI, P5PGM& src )
{
	// Initialize variables
	Mask maskD;
	Mask maskI;

	// Compute Ix and Iy
	maskD.buildGaussFirstDeriv( sigmaD );
	P5PGM Ix = src.convolveX( maskD );
	P5PGM Iy = src.convolveY( maskD );

	// Compute Ix2, Iy2, IxIy
	maskI.buildGauss( sigmaI );
	P5PGM Ix2 = Ix.multiply( Ix ).convolve( maskI );
	P5PGM Iy2 = Iy.multiply( Iy ).convolve( maskI );
	P5PGM IxIy = Ix.multiply( Iy ).convolve( maskI );

	// Compute the Local Maxima RImage
	return computeLocalMaximaImage( computeRImage( 0.06, Ix2, Iy2, IxIy ) );
}




