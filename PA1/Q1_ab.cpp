#include <cstdlib>
#include <iostream>
#include <fstream>
#include "P5PGM.cpp"
using namespace std;

// Function Definitions
void printDataNormalized( const float* src )
{
	for( int i = 0; i < 128; ++i )
	{
		cout << '<' << i << ">\t" << src[i] * 255 << endl;
	}
}

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

float* convolve( const float* original, const Mask1D& mask )
{
	// Initialize variables
	int halfSize = mask.size / 2;
	float* result = new float[ 128 ];

	// Loop
	for( int i = 0; i < 128; ++i )
	{
		// Loop
		result[ i ] = 0.0;
		for( int j = -halfSize; j <= halfSize; ++j )
		{
			// Check position
			if( (i + j) >= 0 && (i + j) <= 127 )
			{
				result[ i ] += original[ i + j ] * mask.vec[ j + halfSize ];
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
	float original[ 128 ];

	// Check the arguments
	if( argc < 2 )
	{
		// Output an error and return
		cout << "Invalid console arguments. Do ./program sigma" << endl;
		return -1;
	}

	// Create the mask
	Mask1D m1( atof( argv[1] ) );

	// Read the data
	readFromDataFile( original );

	///////////////////////////////////////////////////////////////////////
	// Part A
	///////////////////////////////////////////////////////////////////////
	cout << "=== Convolve by Input Sigma ===" << endl;
	printDataNormalized( convolve( original, m1 ) );

	///////////////////////////////////////////////////////////////////////
	// Part B
	///////////////////////////////////////////////////////////////////////
	cout << "=== Convolve by Sigma=5 Twice ===" << endl;
	Mask1D m2( 5.0 );
	printDataNormalized( convolve( convolve( original, m2 ), m2 ) );

	cout << "=== Convolve by Sigma=5*root(2) ===" << endl;
	Mask1D m3( 5.0 * 1.41421356237 );
	printDataNormalized( convolve( original, m3 ) );

	// Return
	return 0;
}
