#include <math.h>

#define pi 3.141592653589793

/* Create a one-dimensional Gaussian mask
"H" of scale "s" (sigma) on "Hsize" pixels.

The values of the mask are scaled so that the
sum of mask values = 1.0 
*/
double* Gauss( double sigma, int width )
{
	// Initialize variables
	double sum;
	double constant = 1.0000 / (sigma * sqrt(2.0000 * pi));
	double powerConstant = 1.0000 / (2 * sigma * sigma);

	// Create the array
	double* array = new double[width];

	// Loop
	for( int i = 0; i < width; i++ )
	{
		double x = i - (width / 2);
		array[i] = constant * exp( -(x * x * powerConstant) );
		sum += array[i];
	}

	// Divide by sum
	for( int i = 0; i < width; i++ )
	{
		array[i] /= sum;
	}

	// Return
	return array;
}

/* **************************** */
/* First derivative of Gaussian */
/* **************************** */
double* Gauss_Deriv1(double sigma, int width)
{
	double cst = 1.0 / (sigma * sigma * sigma * sqrt(2.0 * pi));
	double tssq = -1.0 / (2.0 * sigma * sigma);

	// Create the array
	double* array = new double[width];

	for( int i = 0; i < width; i++ ) 
	{
		double x = i - width/2;
		array[i] = -x * cst * exp(x * x * tssq);
	}

	// Return
	return array;
}
