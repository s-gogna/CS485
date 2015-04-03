#include "P5PGM.cpp"

int main()
{
	P5PGM lena;
	lena.read("lena.pgm");
	Mask mask(10.0,9);

	lena.convolve(mask).write("lenaconv.pgm");

	return 0;
}
