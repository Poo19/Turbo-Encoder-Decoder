
Encoder function

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void enc(int *in, int *sys, int *par);
void encoder(int *output)
{
unsigned int input[43],inlen,h,f1,f2,inter[43],f,b;
unsigned int v0[43],v1[43],v00[43],v11[43],x[40],encodedoutput[129];
char str[10];
int final[80];
int no[10],n[10][8];
int i=0,j,k,m,a=0;

printf("Enter any string: ");
scanf("%[^\n]s",str);

j=strlen(str);
for (i=0;i<j;i++)
for (k=0;k<8;k++)
	n[i][k]=0;

	for(i=0;i<j;i++)

{
	no[i] = str[i];
	m=no[i];
	k=7;
	while (m>0)
	{
		n[i][k] = m%2;
		m = m/2;
		k=k-1;
	}
	n[i][0]=0;
}

k=0;
for(i=0;i<j;i++)
	for(a=0;a<8;a++)
		final[k++] = n[i][a];

if ((8*j)%40!=0)
{
	b = (8*j) + (40-((8*j)%40));
}
else
	b = 8*j;

for (k=8*j;k<b;k++)
{
	final[k]=0;
}

for(f=0;f<(b/40);f++)
{
	for(i=0;i<40;i++)
		input[i]=final[40*f+i];
	input[40]=input[41]=input[42]=0;
	inlen=40;

	h=43;
	f1=3;
	f2=10;
	for (i=0;i<inlen;i++)
		x[i] = (((f1*i)+(f2*i*i))%inlen);
	for( i = 0;i<inlen;i++)
		inter[i]= input[x[i]];
	inter[40]=inter[41]=inter[42]=0;

	enc(&input[0],&v0[0],&v1[0]);
	enc(inter,v00,v11);

	for( i=0;i<h;i++)
	{
		encodedoutput[3*i]=v0[i];
		encodedoutput[3*i+1]=v1[i];
		encodedoutput[3*i+2]=v11[i];
	}
	for(i=0;i<129;i++)
		output[f*129+i]=encodedoutput[i];
}
}




void enc(int *in, int *sys, int *par)
{
unsigned int c[44], d[44], e[44],n=0;
c[0]=0;
d[0]=0;
e[0]=0;
for(n=0;n<43;n++)
{
	par[n]= d[n]^in[n]^c[n];
	c[n+1]=d[n]^in[n]^e[n];
	d[n+1]=c[n];
	e[n+1]=d[n];

	if(n>39)
	{
		if(c[n]==0)
		{
			if(d[n]==0)
			{
				if(e[n]==1)
					in[n]=1;
			}
			else if(d[n]==1)
			{
				if (e[n]==0)
					in[n]=1;
				else if (e[n]==1)
					in[n]=0;
			}
		}
		else if (c[n]==1)
		{
			if (d[n]==0)
			{
				if (e[n]==0)
					in[n]=0;
				else if (e[n]==1)
					in[n]=1;
			}
			else if (d[n]==1)
			{
				if (e[n]==0)
					in[n]=1;
				else if (e[n]==1)
					in[n]=0;
			}
		}
	}
	sys[n]=in[n];
}

