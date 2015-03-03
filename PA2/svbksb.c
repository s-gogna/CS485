#include <stdio.h>
#include <math.h>

extern float *vector(long, long);
extern void free_vector(float*, long, long);

#define NRANSI

void svbksb(float** u, float* w, float** v, int m, int n, float* b, float* x)
//float **u, *w, **v, *b, *x;
//int m,n;
{
	int jj,j,i;
	float s,*tmp;

	tmp=vector(1,n);
	for (j=1;j<=n;j++) {
		s=0.0;
		if (w[j]) {
			for (i=1;i<=m;i++) s += u[i][j]*b[i];
			s /= w[j];
		}
		tmp[j]=s;
	}
	for (j=1;j<=n;j++) {
		s=0.0;
		for (jj=1;jj<=n;jj++) s += v[j][jj]*tmp[jj];
		x[j]=s;
	}
	free_vector(tmp,1,n);
}
#undef NRANSI
/* (C) Copr. 1986-92 Numerical Recipes Software 0#Y". */
