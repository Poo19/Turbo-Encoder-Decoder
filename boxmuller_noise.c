Noise function
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

void boxmuller(int *modout, float *modout11,int snr)
{
	int j;
	float gaussian_noise[258],rn=0,x1=0,x2=0;
	double noise_amp, randnum,noise=0,RATE =0.73;

	noise_amp = sqrt(2.0*RATE*pow(10.0,(snr/10.0)));
	 	//AWGN -> BOX-MULLER METHOD
	for(j=0;j<258;j++)
	{
		do
		{
			randnum = (double) rand()/(RAND_MAX*.125);
	 		x1 = 2*randnum - 1;
	 		randnum = (double) rand()/(RAND_MAX*.125);
	 		x2 = 2*randnum - 1;
	 		rn = x1*x1 + x2*x2;
	 	}
		while(rn >= 1);
		noise = x2 * sqrt( (-2.0*log(rn))/(rn) );
	 	noise = noise/noise_amp;
	 	gaussian_noise[j]=noise;
	 	if(gaussian_noise[j]>.75)
	 		gaussian_noise[j]=.25;
	 		if(gaussian_noise[j]<-0.75)
	 			gaussian_noise[j]=.25;
	 	modout11[j]= modout[j]+gaussian_noise[j];
	 }
}

