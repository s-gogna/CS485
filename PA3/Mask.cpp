#ifndef MASK_CPP
#define MASK_CPP
#include "Gauss.cpp"

class Mask
{
	public:
		Mask( double, int );
		~Mask();

		int getDim() const;
		const double& at( int ) const;

	private:
		int dim;
		double* data;
};

Mask::Mask( double sigma, int width )
{
	// Initialize
	dim = width;
	data = Gauss_Deriv1( sigma, width );
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
