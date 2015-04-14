#include <iostream>
#include <cstdlib>
#include <cmath>
#include <string>
#include "P5PGM.cpp"
#include "P6PPM.cpp"
using namespace std;

// Functions
P5PGM computeRImage( double, const P5PGM&, const P5PGM&, const P5PGM& );

P5PGM computeLocalMaximaImage( const P5PGM& );

P5PGM computeLocalScaleMaximaImage( P5PGM&, const P5PGM&, const P5PGM&, const P5PGM& );

P6PPM computeCornerOverlayImage( const P5PGM&, const P5PGM&, double );

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
	P5PGM* gauss = new P5PGM[18];
	P5PGM* diffGauss = new P5PGM[17];
	P5PGM* harrisCorners = new P5PGM[18];
	P5PGM* cornerMaxima = new P5PGM[18];
	P5PGM* cornerScaleMaxima = new P5PGM[17];

	// Read the image
	image.read( (filename + ".pgm").c_str() );
	P5PGM empty( image.getHeight(), image.getWidth() );

	// Loop and fill the Gaussian Scale Space
	cout << "Computing Gaussian Scale Space . . ." << endl;
	for( int m = 0; m < 18; ++m )
	{
		// Convolve and save
		Mask gaussMask;
		gaussMask.buildGauss( 1.5 * pow( 1.2, m ) );
		gauss[m] = image.convolve( gaussMask );

		// Write the image
		// sprintf(buf, "%d", m);
		// gauss[m].normalize().write( (filename + "_Gauss_N" + string(buf) + ".pgm").c_str() );
	}

	// Loop and fill the Difference of Gaussian Scale Space
	cout << "Computing Difference of Gaussian Scale Space . . ." << endl;
	for( int m = 0; m < 17; ++m )
	{
		// Subtract and save
		diffGauss[m] = gauss[m+1].subtract( gauss[m] );

		// Write the image
		// sprintf(buf, "%d", m);
		// diffGauss[m].normalize().write( (filename + "_DoGauss_N" + string(buf) + ".pgm").c_str() );
	}

	// Loop and fill the Harris Corners for each sigma in the Gauss Scale Space
	cout << "Computing Harris Corners . . ." << endl;
	for( int m = 0; m < 18; ++m )
	{
		// Compute the Harris Corner Image
		double sigmaI = 1.5 * pow( 1.2, m );
		cout << "(Sigma, InterestPoints) = " << "( " << 1.5 * pow( 1.2, m ) << " , ";
		harrisCorners[m] = computeHarrisCornerImage( 0.7 * sigmaI, sigmaI, image );

		// Write the image
		sprintf(buf, "%d", m);
		computeCornerOverlayImage( image, harrisCorners[m].normalize(), 1.5 * pow( 1.2, m ) ).
			write( (filename + "_Corners_M" + string(buf) + ".ppm").c_str() );
	}

	// Loop and perform 3x3 Maxima Thresholding for each Harris Corner Image
	cout << "Computing Harris Corner with Threshold . . ." << endl;
	for( int m = 0; m < 18; ++m )
	{
		// Compute the Harris Corner Maxima Image
		cout << "(Sigma, InterestPoints) = " << "( " << 1.5 * pow( 1.2, m ) << " , ";
		cornerMaxima[m] = computeLocalMaximaImage( harrisCorners[m] );

		// Write the image
		sprintf(buf, "%d", m);
		computeCornerOverlayImage( image, cornerMaxima[m].normalize(), 1.5 * pow( 1.2, m ) ).
			write( (filename + "_CornersThresh_M" + string(buf) + ".ppm").c_str() );
	}

	// Loop and perform 3x3x3 Maxima Thresholding for successive levels of the DoG
	cout << "Computing Harris Corners with Difference of Gaussian Threshold . . ." << endl;
	for( int m = 0; m < 17; ++m )
	{
		// Compute the Harris Corner Maxima Image
		cout << "(Sigma, InterestPoints) = " << "( " << 1.5 * pow( 1.2, m ) << " , ";
		if( m == 0 )
		{
			cornerScaleMaxima[m] = computeLocalScaleMaximaImage( harrisCorners[0], empty, diffGauss[0], diffGauss[1] );
		}
		else if( m == 16 )
		{
			cornerScaleMaxima[m] = computeLocalScaleMaximaImage( harrisCorners[16], diffGauss[15], diffGauss[16], empty );
		}
		else
		{
			cornerScaleMaxima[m] = computeLocalScaleMaximaImage( harrisCorners[m], diffGauss[m-1], diffGauss[m], diffGauss[m+1] );
		}

		// Write the image
		sprintf(buf, "%d", m);
		computeCornerOverlayImage( image, cornerScaleMaxima[m].normalize(), 1.5 * pow( 1.2, m ) ).
			write( (filename + "_CornersScaleThresh_M" + string(buf) + ".ppm").c_str() );
	}
	
	// Deallocate
	delete[] gauss;
	delete[] diffGauss;
	delete[] harrisCorners;
	delete[] cornerMaxima;
	delete[] cornerScaleMaxima;

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
	int count = 0;
	max = max * 0.01;
	for( int i = 0; i < height; ++i )
	{
		for( int j = 0; j < width; ++j )
		{
			// If below thresh
			if( result.at(i,j) <= max ) 
			{ 
				result.at(i,j) = 0.0; 
			}
			else
			{
				count++;
			}
		}
	}
	cout << count << " ) R-Image" << endl;

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
	int count = 0;
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
			if( src.at(i,j) != max || src.at(i,j) < 1500 )
			{
				result.at(i,j) = 0;
			}
			else
			{
				result.at(i,j) = max;
				count++;
			}
		}
	}
	cout << count << " ) Non-Maxima Supression" << endl;

	// Return
	return result;
}

P5PGM computeLocalScaleMaximaImage( P5PGM& corners, const P5PGM& below, const P5PGM& src, const P5PGM& above )
{
	// Initialize variables
	int width = src.getWidth();
	int height = src.getHeight();
	P5PGM result( height, width );

	// Loop through image
	int count = 0;
	for( int i = 0; i < height; ++i )
	{
		for( int j = 0; j < width; ++j )
		{
			// If there is a corner in the image
			if( corners.at(i,j) > 0 )
			{
				// Initialize some more variables
				double max = -1000000;

				// Find the max in a 3x3x3 neighborhood
				for( int k = -1; k <= 1; ++k )
				{
					for( int l = -1; l <= 1; ++l )
					{
						if( (i+k) >= 0 && (i+k) < height && (j+l) >= 0 && (j+l) < width )
						{
							if( above.at(i+k, j+l) > max ) { max = above.at(i+k, j+l); }
							if( src.at(i+k, j+l) > max ) { max = src.at(i+k, j+l); }
							if( below.at(i+k, j+l) > max ) { max = below.at(i+k, j+l); }
						}
					}
				}

				// If it was not the max, reject corner
				if( src.at(i,j) != max || src.at(i,j) < 10 )
				{
					result.at(i,j) = 0;
				}
				else
				{
					result.at(i,j) = max;
					count++;
				}
			}
		}
	}
	cout << count << " ) Scale Non-Maxima Supression" << endl;

	// Return
	return result;
}

P6PPM computeCornerOverlayImage( const P5PGM& src, const P5PGM& corners, double sigma )
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
				result.drawCircleAt( i, j, 0, 255, 0, 2 * sigma );
				result.drawCrossAt( i, j, 0, 255, 0, 3 );
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
	return computeRImage( 0.06, Ix2, Iy2, IxIy );
}




