Max function

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
float maxx (float x, float y)
{
	float w,b,c,z,d;
	if(x>y)
		z= x-y;
	else
		z= y-x;

	b = exp(-z);
	w = log(1+b);
	c = x+w;
	d = y+w;
	if(x>y)
		return(c);
	else
		return(d);
}
