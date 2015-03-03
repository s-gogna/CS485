#include <stdlib.h>

extern void svdcmp(float**,int,int,float*,float**);
extern void svbksb(float**,float*,float**,int,int,float*,float*);

/* this routine solves a m x n linear system of equations using SVD */ 

void solve_system(int m,int n,float** a,float* x,float* b)
//int m,n;
//float **a, *x, *b;
{
 int i,j;
 float w_min, w_max, **a_temp, *w, **v;

 w=(float *)malloc((n+1)*sizeof(float));

 v=(float **)malloc((n+1)*sizeof(float *));
 for(i=0; i<n+1; i++)
   v[i]=(float *)malloc((n+1)*sizeof(float));

 a_temp=(float **)malloc((m+1)*sizeof(float *));
 for(i=0; i<m+1; i++)
   a_temp[i]=(float *)malloc((n+1)*sizeof(float));

 /* copy a to a_temp because svdcmp changes a */

 for(i=0; i<m; i++)
   for(j=0; j<n; j++)
     a_temp[i+1][j+1]=a[i+1][j+1]; 

 svdcmp(a_temp,m,n,w,v);

 w_min=w_max=w[1];
 for(j=2; j<=n; j++) {
   if(w_min > w[j]) w_min=w[j];
   if(w_max < w[j]) w_max=w[j];
 }

 w_min=w_max*1.0e-6;
 for(j=1; j<=n; j++)
   if(w[j] < w_min) w[j]=0.0;

 svbksb(a_temp,w,v,m,n,b,x);

 for(i=0; i<m+1; i++)
   free(a_temp[i]);
 free(a_temp); 

 free(w);

 for(i=0; i<n+1; i++)
   free(v[i]);
 free(v); 
} 
