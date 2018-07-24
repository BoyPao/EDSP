#include "DSP_Lab1_exp5.h"
#include <math.h>
#include <math_const.h>

void compute_dct(void)
{
	int i,j,k;
	float dctcos,u;
	//printf("\n start");
	for (i = 0;i < 74;i++)					//calculate 74 frames
	{
		for (k = 0;k < 30;k++)				//ecach frame take only 30 result
		{
			for (j = 0;j < 100;j++)			//ecach do 100 point DCT
			{
				if(k==0)
				{
					u=0.1414;				
				}
				else
				{
					u=1;
				}
				dctcos = u*cos(3.14*(2*j+1)*(k+1)/200)*Record[j + i*100];
				test[k + i*30] = test[k + i*30]+dctcos;
			}
		}
	}
}
