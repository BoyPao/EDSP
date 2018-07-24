#include "DSP_Lab1_exp5.h"

#include <math.h>
#include <math_const.h>

void compute_fft(void);


//--------------------------------------------------------------------------//
// Function:	Process_Data()												//
//																			//
// Description: This function is called from inside the SPORT0 ISR every 	//
//				time a complete audio frame has been received. The new 		//
//				input samples can be found in the variables iCh0LeftIn and	//
//				iCh0RightIn, respectively. The processed data should be 	//
//				stored in iCh0LeftOut and iCh0RightOut, respectively.		//
//--------------------------------------------------------------------------//
void Process_Data(void)
{
	int i;
	// Perform a loopback (talkthrough)
	for (i=0; i<INPUT_SIZE; i++)
	{
		sCh0LeftOut[i] = sCh0LeftIn[i];
		sCh0RightOut[i] = sCh0RightIn[i];
	}
		
	//  Calculate the FFT

	compute_fft();	
	
}


void compute_fft(void)
{
	int blckExp=1;
	int value_index,i;
	//short energy = 0;
	short threshold=0;
	//short threshold1=0;
	//int frame_count=0;
	//	This is how to call the fft function
			
	rfft_fr16(fft_in,  fft_out, fft_twid, 1, VECTOR_SIZE, &blckExp, 1);	
	for(i=0;i<100;i++)
	{
		/* complex absolute value */
		fft_mag[i] = cabs_fr16 (fft_out[i]);
		energy = energy + (short)fft_mag[i]*(short)fft_mag[i];     	    
	}
	if(energy_count<19)
	{
		threshold+=energy;					//add energy for first 18 frams
	}
	if(energy_count>=19)
	{	
		threshold1=(short)threshold*5/19;	//set threshold
		if (energy > threshold1) 			//if over threshold, record the data
		{
			for(i = 0;i < 100; i++)
			{
				Record[100*frame_count+i] = (short)fft_mag[i];
			}
			frame_count = __builtin_circindex(frame_count, 1, 74);
		}
		if((frame_count>1)&&(energy < threshold1))//if less than threshold, abandon rest data
		{
			for(i = 0;i < 7400-100*frame_count; i++)
			Record[100*frame_count+i] = 0;
			energy_count=93;
		}
	}
	if(energy_count>92)		//if buffer is full, do DCT
	{
		compute_dct();
	}
	energy_count = __builtin_circindex(energy_count, 1, 94);		
}


