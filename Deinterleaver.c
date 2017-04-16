
Deinterleaver function 

#include<stdio.h>
#include<stdlib.h>
void deinterleaver(float *in,float *out)
{
	int x[40],i,f1=3,f2=10;
	for (i=0;i<40;i++)
	{
		x[i] = (((f1*i)+(f2*i*i))%40);
		out[x[i]]= in[i];
	}
	out[40]=in[40];
	out[41]=in[41];
	out[42]=in[42];
}