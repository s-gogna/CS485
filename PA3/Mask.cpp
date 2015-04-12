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

	// Reallocate data
	if( data != NULL ) { delete[] data; }
	data = Gauss( sigma, int(5 * sigma) );
}

void Mask::buildGaussFirstDeriv( double sigma )
{
	// Initialize
	dim = int(5 * sigma);

	// Reallocate data
	if( data != NULL ) { delete[] data; }
	data = Gauss_Deriv1( sigma, int(5 * sigma) );
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
