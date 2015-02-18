// Includes
#include <iostream>
#include <fstream>
using namespace std;

// Typedefs
typedef unsigned char byte;

// PGM Reader and Writer
class P5PGM
{
	public:
		P5PGM( const char* );
		P5PGM( const P5PGM& );

		bool write( const char* );

		bool convolveVert( double* );

		bool convolveHoriz( double* );

		bool convolve2D( double** );

	private:
		int width;
		int height;
		byte data**;
};
