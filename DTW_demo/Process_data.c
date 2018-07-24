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
	long energy = 0;
	//	This is how to call the fft function
			
	rfft_fr16(fft_in,  fft_out, fft_twid, 1, VECTOR_SIZE, &blckExp, 1);	
		
	for(value_index = 0;value_index < 100; value_index++)
	{
	    /* complex absolute value */
	    fft_mag[value_index] = cabs_fr16 (fft_out[value_index]);
	    energy = energy + (short)fft_mag[value_index];
	    	    
	}
	energy = energy - 2*(short)fft_mag[0];
	if (energy > 35) 
	{
		for(i = 0;i < 100; i++)
		{
			//printf("%d\n",frame);
			Record[100*frame_Record+i] = (short)fft_mag[i];
		}
		frame_Record = __builtin_circindex(frame_Record, 1, 45);
	}
		
}


