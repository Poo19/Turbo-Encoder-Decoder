Modulation function 

#include<stdio.h>
#include<stdlib.h>
void modulation(int *in,int *out)
{
	int i;
	for (i=0;i<258;i++)
	{
		if (in[i]==0)
			out[i]=-1;
		else
			out[i]=1;
	}

}