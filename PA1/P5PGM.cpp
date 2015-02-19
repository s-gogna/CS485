// Includes
#include "P5PGM.hpp"

// Functions
Mask1D::Mask1D( float sig )
{
	// Assign and allocate
	sigma = sig;
	size = int(sig) * 5;
	vec = new float[ size ];

	// Get the gaussian
	Gauss( sigma, size, vec );
}

Mask1D::~Mask1D()
{
	// Deallocate
	sigma = 0.0;
	size = 0;
	delete[] vec;
	vec = NULL;
}

P5PGM::P5PGM( int h, int w )
{
	// Save and allocate
	height = h;
	width = w;
	data = new float*[ h ];
	for( int i = 0; i < height; ++i )
	{
		data[i] = new float[ width ];
	}
}

P5PGM::P5PGM( const char* filename )
{
	// Initialize variables
	char buf[1000];
	ifstream img( filename );

	// Read
	img.getline( buf, 100 );

	// If the line is a comment
	while( img.peek() == '#' )
	{
		// Read the next line
		img.getline( buf, 1000 );
	}

	// Read width and height
	img >> width >> height >> buf;
	img.get();

	// Allocate the data array and read
	data = new float*[ height ];
	for( int i = 0; i < height; ++i )
	{
		// Allocate the row
		data[i] = new float[ width ];

		// Read the row
		for( int j = 0; j < width; ++j )
		{
			data[i][j] = (float) img.get();
		}
	}
}

P5PGM::P5PGM( const P5PGM& src )
{
	// Deep copy the fields
	width = src.width;
	height = src.height;
	
	// Deep copy the image
	data = new float*[ height ];
	for( int i = 0; i < height; ++i )
	{
		// Allocate the row
		data[i] = new float[ width ];

		// Copy
		for( int j = 0; j < width; ++j )
		{
			data[i][j] = src.data[i][j];
		}
	}
}

bool P5PGM::write( const char* filename )
{
	// Initialize variables
	char result;
	ofstream output( filename );

	// Write out in P5 format
	output << "P5" << endl;
	output << width << ' ' << height << endl << 255 << endl;
	for( int i = 0; i < height; ++i )
	{
		for( int j = 0; j < width; ++j )
		{
			if( data[i][j] < 0.0 )
			{
				output << char(0);
			}
			else if( data[i][j] > 255.0 )
			{
				output << char(255);
			}
			else
			{
				output << char( int(data[i][j]) );
			}
		}
	}
}

P5PGM P5PGM::convolve1D( const Mask1D& mask )
{
	// Initialize variables
	int halfSize = mask.size / 2;
	P5PGM horizResult = *this;

	// Loop through the rows
	for( int i = 0; i < height; ++i )
	{
		// Loop through the columns
		for( int j = 0; j < width; ++j )
		{
			// Set the result data to 0 (only this time)
			horizResult.data[ i ][ j ] = 0.0;

			// Loop through the mask
			for( int k = -halfSize; k <= halfSize; ++k )
			{
				// Check position
				if( (j + k) >= 0 && (j + k) < width )
				{
					horizResult.data[ i ][ j ] += 
						data[ i ][ j + k ] * 
						mask.vec[ k + halfSize ];
				}
			}
		}
	}

	// Vertical result
	P5PGM vertResult = horizResult;

	// Loop through the rows
	for( int i = 0; i < height; ++i )
	{
		// Loop through the columns
		for( int j = 0; j < width; ++j )
		{
			// Set the result data to 0 (only this time)
			vertResult.data[ i ][ j ] = 0.0;

			// Loop through the mask
			for( int k = -halfSize; k <= halfSize; ++k )
			{
				// Check position
				if( (i + k) >= 0 && (i + k) < height )
				{
					vertResult.data[ i ][ j ] += 
						horizResult.data[ i + k ][ j ] * 
						mask.vec[ k + halfSize ];
				}
			}
		}
	}

	// Return
	return vertResult;
}

P5PGM P5PGM::convolve2D( const Mask1D& mask )
{
	// Initialize variables
	int halfSize = mask.size / 2;
	P5PGM result = *this;

	// Allocate the space for a 2D Mask
	float** maskMatrix = new float*[ mask.size ];
	for( int i = 0; i < mask.size; ++i )
	{
		maskMatrix[i] = new float[ mask.size ];
	}

	// Multiply the 1D mask by itself to get the 2D Mask
	for( int i = 0; i < mask.size; ++i )
	{
		// Loop
		for( int j = 0; j < mask.size; ++j )
		{
			// Initialize value
			maskMatrix[ i ][ j ] = mask.vec[i] * mask.vec[j];
		}
	}

	// Convolve
	for( int i = 0; i < height; ++i )
	{
		for( int j = 0; j < width; ++j )
		{
			// Initialize value to 0
			result.data[ i ][ j ] = 0.0;

			// Loop through the mask matrix
			for( int k = -halfSize; k <= halfSize; ++k )
			{
				for( int l = -halfSize; l <= halfSize; ++l )
				{
					// Check if within bounds
					if( (i + k) >= 0 && (i + k) < height && (j + l) >= 0 && (j + l) < width )
					{
						result.data[ i ][ j ] += 
							data[ i + k ][ j + l ] * 
							maskMatrix[ k + halfSize ][ l + halfSize ];
					}
				}
			}
		}
	}

	// Return
	return result;
}

P5PGM P5PGM::convolve2D( int maskN, const float mask[3][3] )
{
	// Initialize variables
	int halfSize = maskN / 2;
	P5PGM result = *this;

	// Convolve
	for( int i = 0; i < height; ++i )
	{
		for( int j = 0; j < width; ++j )
		{
			// Initialize value to 0
			result.data[ i ][ j ] = 0.0;

			// Loop through the mask matrix
			for( int k = -halfSize; k <= halfSize; ++k )
			{
				for( int l = -halfSize; l <= halfSize; ++l )
				{
					// Check if within bounds
					if( (i + k) >= 0 && (i + k) < height && (j + l) >= 0 && (j + l) < width )
					{
						result.data[ i ][ j ] += 
							data[ i + k ][ j + l ] * 
							mask[ k + halfSize ][ l + halfSize ];
					}
				}
			}
		}
	}

	// Return
	return result;
}

P5PGM P5PGM::threshold( int tVal )
{
	// Initialize variables
	P5PGM result( height, width );

	// Loop through the image
	for( int i = 0; i < height; ++i )
	{
		for( int j = 0; j < width; ++j )
		{
			if( data[i][j] <= tVal )
			{
				result.data[i][j] = 0.0;
			}
			else
			{
				result.data[i][j] = data[i][j];
			}
		}
	}

	// Return
	return result;
}

P5PGM P5PGM::magnitudeOf( const P5PGM& one, const P5PGM& two )
{
	// Initialize a new P5PGM
	P5PGM result( one.height, one.width );

	// Loop through every point in the two images
	for( int i = 0; i < one.height; ++i )
	{
		for( int j = 0; j < one.width; ++j )
		{
			result.data[i][j] = sqrt( (one.data[i][j] * one.data[i][j]) + (two.data[i][j] * two.data[i][j] ) );
		}
	}

	// Return
	return result;
}

P5PGM P5PGM::directionOf( const P5PGM& one, const P5PGM& two )
{
	// Initialize a new P5PGM
	P5PGM result( one.height, one.width );

	// Loop through every point in the two images
	for( int i = 0; i < one.height; ++i )
	{
		for( int j = 0; j < one.width; ++j )
		{
			result.data[i][j] = atan2( one.data[i][j], two.data[i][j] );
		}
	}

	// Return
	return result;
}

P5PGM::~P5PGM()
{
	delete[] data;
	data = NULL;
	width = height = 0;
}
