// Includes
#include <iostream>
#include <fstream>
#include "Gauss_masks.c"
using namespace std;

// Mask
struct Mask1D
{
	public:
		Mask1D( float );
		~Mask1D();

		int size;
		float sigma;
		float* vec;
};

// PGM Reader and Writer
class P5PGM
{
	public:
		P5PGM( const char* );
		P5PGM( const P5PGM& );
		~P5PGM();

		bool write( const char* );

		P5PGM convolve1D( const Mask1D& );

		P5PGM convolve2D( const Mask1D& );

	private:
		int width;
		int height;
		float** data;
};
