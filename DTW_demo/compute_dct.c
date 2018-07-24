#include "DSP_Lab1_exp5.h"
#include <math.h>
#include <math_const.h>

void compute_dct(void)
{
	int i,j,k;
	float dct,table;
	//printf("\n start");
	for (i = 0;i < frame_Record;i++)
	{
		for (k = 0;k < 30;k++)
		{
			for (j = 0;j < 100;j++)
			{
				table = 0.1414*cos(3.14*(2*j+1)*(k+1)/200);
				dct = table*Record[j + i*100];
				test[k + i*30] = test[k + i*30]+dct;
			}
			//printf("%f, ",test[k + i*30]);
		}
		//tt = 0.1414*cos_fr16(3.14/200);
		//printf("%f   ",tt);
	}
	//printf("\n end \n");
}
