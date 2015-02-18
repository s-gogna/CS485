#include <math.h>

#define pi 3.141592653589793



/* Create a one-dimensional Gaussian mask
 "H" of scale "s" (sigma) on "Hsize" pixels.

   The values of the mask are scaled so that the
 sum of mask values = 1.0 
*/
 


void Gauss (float s, int Hsize, float * H)

{
  
	int     i;
  
	float  cst,tssq,x,sum;
 
  
	
	cst=1./(s*sqrt(2.0*pi));
  
	tssq=1./(2*s*s);
 
  
	for (i=0; i<Hsize; i++) 
	{
    
		x=(float)(i-Hsize/2);
    
		H[i]=(cst*exp(-(x*x*tssq)));
  
	}
 
  	
	sum=0.0;
  
	for (i=0;i<Hsize;i++)
    
		sum += H[i];
  
	for(i=0;i<Hsize;i++)
    
	H[i] /= sum;

}
