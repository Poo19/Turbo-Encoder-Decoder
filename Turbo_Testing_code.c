Turbo test function

#include<stdio.h>
#include<stdlib.h>
float maxx (float a , float b);
void turbotest(float *leu,float *ruu,float *rp,float *lint,float SNR)
{
	float p1,p2,q1,q2,r1,r2,s1,s2,t1,t2,u1,u2,v1,v2,w1,w2;
	float alp[43][8], bet[43][8],lc;
	int k,i;

	alp[0][0]=0;
	alp[1][0]=-0.5*lint[0]+0.5*(-ruu[0]-rp[0]);
	alp[1][4]=0.5*lint[0]+0.5*(ruu[0]+rp[0]);

	alp[2][0]=-0.5*lint[1]+0.5*(-ruu[1]-rp[1])+alp[1][0];
	alp[2][2]=-0.5*lint[1]+0.5*(-ruu[1]+rp[1])+alp[1][4];
	alp[2][4]=0.5*lint[1]+0.5*(ruu[1]+rp[1])+alp[1][0];
	alp[2][6]=0.5*lint[1]+0.5*(ruu[1]-rp[1])+alp[1][4];

	alp[3][0]=-0.5*lint[2]+0.5*(-ruu[2]-rp[2])+alp[2][0];
	alp[3][1]=0.5*lint[2]+0.5*(ruu[2]-rp[2])+alp[2][2];
	alp[3][2]=-0.5*lint[2]+0.5*(-ruu[2]+rp[2])+alp[2][4];
	alp[3][3]=0.5*lint[2]+0.5*(ruu[2]+rp[2])+alp[2][6];
	alp[3][4]=0.5*lint[2]+0.5*(ruu[2]+rp[2])+alp[2][0];
	alp[3][5]=-0.5*lint[2]+0.5*(-ruu[2]+rp[2])+alp[2][2];
	alp[3][6]=0.5*lint[2]+0.5*(ruu[2]-rp[2])+alp[2][4];
	alp[3][7]=-0.5*lint[2]+0.5*(-ruu[2]-rp[2])+alp[2][6];

	k=40;

	for(i=4;i<k;i++)
	{
		alp[i][0]=maxx(-0.5*lint[i-1]+0.5*(-ruu[i-1]-rp[i-1])+alp[i-1][0],0.5*lint[i-1]+0.5*(ruu[i-1]+rp[i-1])+alp[i-1][1]);
		alp[i][1]=maxx(0.5*lint[i-1]+0.5*(ruu[i-1]-rp[i-1])+alp[i-1][2],-0.5*lint[i-1]+0.5*(-ruu[i-1]+rp[i-1])+alp[i-1][3]);
		alp[i][2]=maxx(-0.5*lint[i-1]+0.5*(-ruu[i-1]+rp[i-1])+alp[i-1][4],0.5*lint[i-1]+0.5*(ruu[i-1]-rp[i-1])+alp[i-1][5]);
		alp[i][3]=maxx(0.5*lint[i-1]+0.5*(ruu[i-1]+rp[i-1])+alp[i-1][6],-0.5*lint[i-1]+0.5*(-ruu[i-1]-rp[i-1])+alp[i-1][7]);
		alp[i][4]=maxx(0.5*lint[i-1]+0.5*(ruu[i-1]+rp[i-1])+alp[i-1][0],-0.5*lint[i-1]+0.5*(-ruu[i-1]-rp[i-1])+alp[i-1][1]);
		alp[i][5]=maxx(-0.5*lint[i-1]+0.5*(-ruu[i-1]+rp[i-1])+alp[i-1][2],0.5*lint[i-1]+0.5*(ruu[i-1]-rp[i-1])+alp[i-1][3]);
		alp[i][6]=maxx(0.5*lint[i-1]+0.5*(ruu[i-1]-rp[i-1])+alp[i-1][4],-0.5*lint[i-1]+0.5*(-ruu[i-1]+rp[i-1])+alp[i-1][5]);
		alp[i][7]=maxx(-0.5*lint[i-1]+0.5*(-ruu[i-1]-rp[i-1])+alp[i-1][6],0.5*lint[i-1]+0.5*(ruu[i-1]+rp[i-1])+alp[i-1][7]);
	}

	alp[k][0]=maxx(-0.5*lint[k-1]+0.5*(-ruu[k-1]-rp[k-1])+alp[k-1][0],0.5*lint[k-1]+0.5*(ruu[k-1]+rp[k-1])+alp[k-1][1]);
	alp[k][1]=maxx(0.5*lint[k-1]+0.5*(ruu[k-1]-rp[k-1])+alp[k-1][2],-0.5*lint[k-1]+0.5*(-ruu[k-1]+rp[k-1])+alp[k-1][3]);
	alp[k][2]=maxx(-0.5*lint[k-1]+0.5*(-ruu[k-1]+rp[k-1])+alp[k-1][4],0.5*lint[k-1]+0.5*(ruu[k-1]-rp[k-1])+alp[k-1][5]);
	alp[k][3]=maxx(0.5*lint[k-1]+0.5*(ruu[k-1]+rp[k-1])+alp[k-1][6],-0.5*lint[k-1]+0.5*(-ruu[k-1]-rp[k-1])+alp[k-1][7]);

	alp[k+1][0]=maxx(-0.5*lint[k]+0.5*(-ruu[k]-rp[k])+alp[k][0],0.5*lint[k]+0.5*(ruu[k]+rp[k])+alp[k][1]);
	alp[k+1][1]=maxx(0.5*lint[k]+0.5*(ruu[k]-rp[k])+alp[k][2],-0.5*lint[k]+0.5*(-ruu[k]+rp[k])+alp[k][3]);

	alp[k+2][0]=maxx(-0.5*lint[k+1]+0.5*(-ruu[k+1]-rp[k+1])+alp[k+1][0],0.5*lint[k+1]+0.5*(ruu[k+1]+rp[k+1])+alp[k+1][1]);

	bet[k+2][0]=0;

	bet[k+1][0]=-0.5*lint[k+1]+0.5*(-ruu[k+1]-rp[k+1]);
	bet[k+1][1]=0.5*lint[k+1]+0.5*(ruu[k+1]+rp[k+1]);

	bet[k][0]=-0.5*lint[k]+0.5*(-ruu[k]-rp[k])+bet[k+1][0];
	bet[k][1]=0.5*lint[k]+0.5*(ruu[k]+rp[k])+bet[k+1][0];
	bet[k][2]=0.5*lint[k]+0.5*(ruu[k]-rp[k])+bet[k+1][1];
	bet[k][3]=-0.5*lint[k]+0.5*(-ruu[k]+rp[k])+bet[k+1][1];

	bet[k-1][0]=-0.5*lint[k-1]+0.5*(-ruu[k-1]-rp[k-1])+bet[k][0];
	bet[k-1][1]=0.5*lint[k-1]+0.5*(ruu[k-1]+rp[k-1])+bet[k][0];
	bet[k-1][2]=0.5*lint[k-1]+0.5*(ruu[k-1]-rp[k-1])+bet[k][1];
	bet[k-1][3]=-0.5*lint[k-1]+0.5*(-ruu[k-1]+rp[k-1])+bet[k][1];
	bet[k-1][4]=-0.5*lint[k-1]+0.5*(-ruu[k-1]+rp[k-1])+bet[k][2];
	bet[k-1][5]=0.5*lint[k-1]+0.5*(ruu[k-1]-rp[k-1])+bet[k][2];
	bet[k-1][6]=0.5*lint[k-1]+0.5*(ruu[k-1]+rp[k-1])+bet[k][3];
	bet[k-1][7]=-0.5*lint[k-1]+0.5*(-ruu[k-1]-rp[k-1])+bet[k][3];

	for(i=k-2;i>=3;i--)
	{
		bet[i][0]=maxx(-0.5*lint[i]+0.5*(-ruu[i]-rp[i])+bet[i+1][0],0.5*lint[i]+0.5*(ruu[i]+rp[i])+bet[i+1][4]);
		bet[i][1]=maxx(-0.5*lint[i]+0.5*(-ruu[i]-rp[i])+bet[i+1][4],0.5*lint[i]+0.5*(ruu[i]+rp[i])+bet[i+1][0]);
		bet[i][2]=maxx(-0.5*lint[i]+0.5*(-ruu[i]+rp[i])+bet[i+1][5],0.5*lint[i]+0.5*(ruu[i]-rp[i])+bet[i+1][1]);
		bet[i][3]=maxx(-0.5*lint[i]+0.5*(-ruu[i]+rp[i])+bet[i+1][1],0.5*lint[i]+0.5*(ruu[i]-rp[i])+bet[i+1][5]);
		bet[i][4]=maxx(-0.5*lint[i]+0.5*(-ruu[i]+rp[i])+bet[i+1][2],0.5*lint[i]+0.5*(ruu[i]-rp[i])+bet[i+1][6]);
		bet[i][5]=maxx(-0.5*lint[i]+0.5*(-ruu[i]+rp[i])+bet[i+1][6],0.5*lint[i]+0.5*(ruu[i]-rp[i])+bet[i+1][2]);
		bet[i][6]=maxx(-0.5*lint[i]+0.5*(-ruu[i]-rp[i])+bet[i+1][7],0.5*lint[i]+0.5*(ruu[i]+rp[i])+bet[i+1][3]);
		bet[i][7]=maxx(-0.5*lint[i]+0.5*(-ruu[i]-rp[i])+bet[i+1][3],0.5*lint[i]+0.5*(ruu[i]+rp[i])+bet[i+1][7]);
	}

	bet[2][0]=maxx(-0.5*lint[2]+0.5*(-ruu[2]-rp[2])+bet[3][0],0.5*lint[2]+0.5*(ruu[2]+rp[2])+bet[3][4]);
	bet[2][2]=maxx(-0.5*lint[2]+0.5*(-ruu[2]+rp[2])+bet[3][5],0.5*lint[2]+0.5*(ruu[2]-rp[2])+bet[3][1]);
	bet[2][4]=maxx(-0.5*lint[2]+0.5*(-ruu[2]+rp[2])+bet[3][2],0.5*lint[2]+0.5*(ruu[2]-rp[2])+bet[3][6]);
	bet[2][6]=maxx(-0.5*lint[2]+0.5*(-ruu[2]-rp[2])+bet[3][7],0.5*lint[2]+0.5*(ruu[2]+rp[2])+bet[3][3]);

	bet[1][0]=maxx(-0.5*lint[1]+0.5*(-ruu[1]-rp[1])+bet[2][0],0.5*lint[1]+0.5*(ruu[1]+rp[1])+bet[2][4]);
	bet[1][4]=maxx(-0.5*lint[1]+0.5*(-ruu[1]+rp[1])+bet[2][2],0.5*lint[1]+0.5*(ruu[1]-rp[1])+bet[2][6]);

	lc=4*(SNR);

	leu[0]=rp[0]+bet[1][4]-bet[1][0];
	leu[1]=maxx(alp[1][0]+bet[2][4]+lc*0.5*rp[1],alp[1][4]+bet[2][6]-lc*0.5*rp[1])-maxx(alp[1][0]+bet[2][1]-lc*0.5*rp[1],alp[1][4]+bet[2][2]+lc*0.5*rp[1]);
	leu[2]=maxx(maxx(alp[2][0]+bet[3][4]+lc*0.5*rp[2],alp[2][2]+bet[3][1]-lc*0.5*rp[2]),maxx(alp[2][4]+bet[3][6]-lc*0.5*rp[2],alp[2][6]+bet[3][3]+lc*0.5*rp[2]))-maxx(maxx(alp[2][1]+bet[3][1]-lc*0.5*rp[2],alp[2][2]+bet[3][5]+lc*0.5*rp[2]),maxx(alp[2][4]+bet[3][2]+lc*0.5*rp[2],alp[2][6]+bet[3][7]-lc*0.5*rp[2]));
	for (i=3;i<k;i++)
	{
		p1=alp[i][0]+bet[i+1][4]+lc*0.5*rp[i];
		p2=alp[i][1]+bet[i+1][0]+lc*0.5*rp[i];
		q1=alp[i][2]+bet[i+1][1]-lc*0.5*rp[i];
		q2=alp[i][3]+bet[i+1][5]-lc*0.5*rp[i];
		r1=alp[i][4]+bet[i+1][6]-lc*0.5*rp[i];
		r2=alp[i][5]+bet[i+1][2]-lc*0.5*rp[i];
		s1=alp[i][6]+bet[i+1][3]+lc*0.5*rp[i];
		s2=alp[i][7]+bet[i+1][7]+lc*0.5*rp[i];
		t1=alp[i][0]+bet[i+1][0]-lc*0.5*rp[i];
		t2=alp[i][1]+bet[i+1][4]-lc*0.5*rp[i];
		u1=alp[i][2]+bet[i+1][5]+lc*0.5*rp[i];
		u2=alp[i][3]+bet[i+1][1]+lc*0.5*rp[i];
		v1=alp[i][4]+bet[i+1][2]+lc*0.5*rp[i];
		v2=alp[i][5]+bet[i+1][6]+lc*0.5*rp[i];
		w1=alp[i][6]+bet[i+1][7]-lc*0.5*rp[i];
		w2=alp[i][7]+bet[i+1][3]-lc*0.5*rp[i];
		leu[i]=maxx((maxx(maxx(p1,p2),maxx(q1,q2))),(maxx(maxx(r1,r2),maxx(s1,s2))))-maxx((maxx(maxx(t1,t2),maxx(u1,u2))),(maxx(maxx(v1,v2),maxx(w1,w2))));
	}
	leu[k]=maxx(maxx(alp[k][1]+bet[k+1][0]+lc*0.5*rp[k],alp[k][2]+bet[k+1][1]-lc*0.5*rp[k]),maxx(alp[k][5]+bet[k+1][2]-lc*0.5*rp[k],alp[k][6]+bet[k+1][3]+lc*0.5*rp[k]))-maxx(maxx(alp[k][0]+bet[k+1][1]-lc*0.5*rp[k],alp[k][3]+bet[k+1][1]+lc*0.5*rp[k+1]),maxx(alp[k][4]+bet[k+1][2]+lc*0.5*rp[k],alp[k][7]+bet[k+1][3]-lc*0.5*rp[k]));
	leu[k+1]=maxx(alp[k+1][2]+bet[k+2][0]+lc*0.5*rp[k+1],alp[k+1][2]+bet[k+2][1]-lc*0.5*rp[k+1])-maxx(alp[k+1][0]+bet[k+2][0]-lc*0.5*rp[k+1],alp[k+1][3]+bet[k+2][1]+lc*0.5*rp[k+1]);
	leu[k+2]=rp[k+2]+alp[k+2][1]-alp[k+2][0];
}
