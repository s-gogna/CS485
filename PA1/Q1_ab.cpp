#include <cstdlib>
#include <iostream>
#include <fstream>
#include "Gauss_masks.c"
using namespace std;

// Function Definitions
void readFromDataFile( float* dest )
{
	// Open the file
	ifstream fin( "Rect_128.txt" );

	// Read
	for( int i = 0; i < 128; ++i )
	{
		// Read a float
		fin >> dest[ i ];
	}

	// Close the file
	fin.close();
}

float* convolve( const float* original, int maskSize, const float* mask )
{
	// Initialize variables
	int halfMaskSize = maskSize / 2;
	float* result = new float[ 128 ];

	// Loop
	for( int i = 0; i < 128; ++i )
	{
		// Loop
		result[ i ] = 0.0;
		for( int j = -halfMaskSize; j <= halfMaskSize; ++j )
		{
			// Check position
			if( i + j >= 0 && i+ j <= 127 )
			{
				result[ i ] += original[ i + j ] * mask[ j + halfMaskSize ];
			}
		}
	}

	// Return
	return result;
}

// Main
int main( int argc, char** argv )
{
	// Declare variables
	int sigma = 0;
	float sigmaFloat = 0.0;
	float original[ 128 ];
	float* mask;
	float* res;

	// Check the arguments
	if( argc < 2 )
	{
		// Output an error and return
		cout << "Invalid console arguments. Do ./program sigma" << endl;
		return -1;
	}

	// Convert the argument
	sigma = atoi( argv[1] );
	mask = new float[ sigma * 5 ];

	// Build the mask
	Gauss( sigma, sigma * 5, mask );

	// Read the data
	readFromDataFile( original );

	///////////////////////////////////////////////////////////////////////
	// Part A
	///////////////////////////////////////////////////////////////////////
	cout << "=== Convolve by Input Sigma ===" << endl;
	res = convolve( original, sigma * 5, mask );
	for( int i = 0; i < 128; ++i )
	{
		cout << "Num " << i << ": ";
		cout << res[i] * 255.0 << endl;
	}
	delete[] res;

	///////////////////////////////////////////////////////////////////////
	// Part B
	///////////////////////////////////////////////////////////////////////
	cout << "=== Convolve by Sigma=5 Twice ===" << endl;
	sigmaFloat = 5.0;
	sigma = int(sigmaFloat);

	delete[] mask;
	mask = new float[ sigma * 5 ];
	Gauss( sigmaFloat, int(sigmaFloat * 5), mask );

	res = convolve( original, sigma * 5, mask );
	float* resTwo = convolve( res, sigma * 5, mask );
	for( int i = 0; i < 128; ++i )
	{
		cout << "Num " << i << ": ";
		cout << resTwo[i] * 255.0 << endl;
	}
	delete[] resTwo;
	delete[] res;


	cout << "=== Convolve by Sigma=5*root(2) ===" << endl;
	sigmaFloat = 5 * sqrt(2.0);
	sigma = int(sigmaFloat);

	delete[] mask;
	mask = new float[ sigma * 5 ];
	Gauss( sigmaFloat, int(sigmaFloat * 5), mask );

	res = convolve( original, sigma * 5, mask );
	for( int i = 0; i < 128; ++i )
	{
		cout << "Num " << i << ": ";
		cout << res[i] * 255.0 << endl;
	}

	// Deallocate
	delete[] mask;
	delete[] res;
	return 0;
}
