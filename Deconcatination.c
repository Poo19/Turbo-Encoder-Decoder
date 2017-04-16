
Deconcat function

#include<stdio.h>
#include<stdlib.h>
void deconcat (float *in,float *out1,float *out2,float *out3)
{
	int i;
	for (i=0;i<43;i++)
	{
		out1[i]= in[3*i];
		out2[i]=in[3*i+1];
		out3[i]=in[3*i+2];
	}
}