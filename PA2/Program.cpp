#include <iostream>
#include <string.h>
#include "PGMFile.cpp"
#include "solve_system.c"
#include "svbksb.c"
#include "svdcmp.c"
using namespace std;

int main( int argc, char** argv )
{
	// Initialize variables
	// Note: solver requires indexing of 1, therefore extra space is
	// allocated and unused
	PGMFile source;
	PGMFile dest(48, 40); // (height, width)
	float** A;
	float coef_x[4];
	float B_x[4];
	float coef_y[4];
	float B_y[4];

	// Set the B vectors
	B_x[1] = 8;
	B_x[2] = 31;
	B_x[3] = 20;
	B_x[4] = 20;

	B_y[1] = 10;
	B_y[2] = 10;
	B_y[3] = 27;
	B_y[4] = 38;

	// Allocate A and set values
	A = new float*[5];
	for( int i = 0; i < 5; ++i )
	{
		A[i] = new float[4];
	}

	// Check arguments
	if( argc < 10 )
	{
		// Return error
		cout << "Invalid usage: ./a.out filename eleft_x eleft_y eright_x eright_y nose_x nose_y mouth_x mouth_y" << endl;
		return -1;
	}

	// Read the image and set the coordinates
	source.read( argv[1] );

	A[1][1] = atof( argv[2] );
	A[1][2] = atof( argv[3] );
	A[1][3] = 1.0;

	A[2][1] = atof( argv[4] );
	A[2][2] = atof( argv[5] );
	A[2][3] = 1.0;

	A[3][1] = atof( argv[6] );
	A[3][2] = atof( argv[7] );
	A[3][3] = 1.0;
	
	A[4][1] = atof( argv[8] );
	A[4][2] = atof( argv[9] );
	A[4][3] = 1.0;

//cout << "Matrix A" << endl;
//for( int i = 1; i<= 4; ++i ) { for( int j = 1; j <= 3; ++j ) cout << A[i][j] << ' '; cout << endl; }

	// Solve
	solve_system( 4, 3, A, coef_x, B_x );
	solve_system( 4, 3, A, coef_y, B_y );

cout << "Vector X, Vector Y" << endl;
for( int i = 1; i < 4; ++i ) cout << coef_x[i] << ',' << coef_y[i] << endl;

	// Start writing to the destination image
	for( int i = 0; i < 112; ++i )
	{
		for( int j = 0; j < 92; ++j )
		{
			// Get the remapped value
			int final_x = (j * coef_x[1]) + (i * coef_x[2]) + coef_x[3];
			int final_y = (j * coef_y[1]) + (i * coef_y[2]) + coef_y[3];

			// If the point is within bounds
			if( final_x >= 0 && final_x < 40 && final_y >= 0 && final_y < 48 )
			{
				// Set the pixel value at that point
				dest.at( final_y, final_x ) = source.at( i, j );
			}
		}
	}

	char buf[128];
	int len = strlen( argv[1] );
	strcpy( buf, argv[1] );
	strcpy( buf + len - 4, "_Result.pgm" );
	dest.write( buf );

	// Return success
	return 0;
}




