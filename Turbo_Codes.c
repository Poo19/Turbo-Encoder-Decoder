
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
float maxx (float a , float b);
void interleaver(float *in,float *out);
void deinterleaver(float *in,float *out);
void deconcat (float *in,float *out1,float *out2,float *out3);
void modulation(int *in,int *out);
void turbotest(float *leu,float *ruu,float *rp,float *lint,float SNR);
void encoder(int *output);
void boxmuller(int *modout,float *modout11,int snr);
void main(void)
{
	char y[10];
	int i,j,encinput[258],modout[258],dec[5][8],k=0;
	int out=0;
	int finalout[5],d,f;
	float lc,snr,lee1[43],lee2[43],l[43],L[43],ru[43],ru2[43],rp1[43],rp2[43],noiseoutput[258],noiseoutput1[129],la[43];

	encoder(encinput);

	modulation(encinput,modout);
	snr = 6;
	lc = 4*snr;
	boxmuller(modout,noiseoutput,snr);
for (d=0;d<2;d++)
{
	for(f=0;f<129;f++)
	noiseoutput1[f]=noiseoutput[129*d+f];//modout[129*d+f];//noiseoutput[129*d+f];
	deconcat (noiseoutput1,ru,rp1,rp2);



	for (i=0;i<43;i++)
		la[i] = 0;
	interleaver (ru,ru2);

	for (j=0;j<4;j++)
	{
		turbotest (&lee1[0],&ru[0],&rp1[0],&la[0],snr);
		interleaver (lee1,la);
		turbotest (lee2,ru2,rp2,la,snr);
		for (i=0;i<43;i++)
			l[i] = (lc*ru2[i])+la[i]+lee2[i];
		deinterleaver (l,L);
		deinterleaver (lee2,la);
		for (i=0;i<40;i++)
			{
			if (L[i]<=0)
				L[i]=0;
			else
				L[i]=1;
			}
	}
	k = 0;
	for(i=0;i<5;i++)
		for(j=0;j<8;j++)
			dec[i][j] = L[k++];
	for(i=0;i<5;i++)
	{
		k = 0;
		out = 0;
		for (j=7;j>=0;j--)
			out= out + (dec[i][j] * pow(2,k++));
		finalout[i] = out;
		y[5*d+i] = finalout[i];
	}
}
	for (i=0;i<10;i++)
		printf("%c",y[i]);
}








