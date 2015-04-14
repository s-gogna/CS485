#ifndef MASK_CPP
#define MASK_CPP
#include "Gauss.cpp"

class Mask
{
	public:
		Mask();
		~Mask();

		void buildGauss( double );
		void buildGaussFirstDeriv( double );

		int getDim() const;
		const double& at( int ) const;

	private:
		int dim;
		double* data;
};

Mask::Mask()
{
	// Initialize
	dim = 0;
	data = NULL;
}

void Mask::buildGauss( double sigma )
{
	// Initialize
	dim = int(5 * sigma);

	// If the mask dimension is even, add another dimension
	if( dim % 2 == 0 ) { dim++; }

	// Reallocate data
	if( data != NULL ) { delete[] data; }
	data = Gauss( sigma, dim );
}

void Mask::buildGaussFirstDeriv( double sigma )
{
	// Initialize
	dim = int(5 * sigma);

	// If the mask dimension is even, add another dimension
	if( dim % 2 == 0 ) { dim++; }

	// Reallocate data
	if( data != NULL ) { delete[] data; }
	data = Gauss_Deriv1( sigma, dim );
}

Mask::~Mask()
{
	// Reset
	dim = 0;
	delete[] data;
}

int Mask::getDim() const { return dim; }

const double& Mask::at(int index) const
{
	return data[index];
}

#endif
