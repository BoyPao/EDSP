#include "DSP_Lab1_exp5.h"

//--------------------------------------------------------------------------//
// Function:	Sport0_RX_ISR												//
//																			//
// Description: This ISR is executed after a complete frame of input data 	//
//				has been received. The new samples are stored in 			//
//				iCh0LeftIn and iCh0RightIn, respectively. Then the function //
//				Process_Data() is called in which user code can be executed.//
//				After that the processed values are copied from the 		//
//				variables iCh0LeftOut and iCh0RightOut into the dma 		//
//				transmit buffer.											//
//--------------------------------------------------------------------------//
EX_INTERRUPT_HANDLER(Sport0_RX_ISR)
{
	int i;
	static short j=0,k=0,m=0,del=0;
	
	// confirm interrupt handling
	*pDMA2_0_IRQ_STATUS = 0x0001;

	// Move data from receive buffer to local buffer
	//m = !m;
	#pragma no_alias
	for (i = 0; i < INPUT_SIZE; i++)
	{
		// Retrieve all the samples from receive buffer to process buffer
		sCh0LeftIn[i] = sRxBuffer1[INTERNAL_ADC_L0+j];
		sCh0RightIn[i] = sRxBuffer1[INTERNAL_ADC_R0+j];
		fft_in[i] = fft_in[512+i];
		fft_in[512+i] = sRxBuffer1[INTERNAL_ADC_R0+j];
		// use the builtin circular buffer to update the index
		j = __builtin_circindex(j, 4, 4*INPUT_SIZE*TOTAL_FRAME);
	}

			// call function that contains user code
		Process_Data();

	
	#pragma no_alias
	for (i=0; i<INPUT_SIZE; i++)
	{
		sTxBuffer1[INTERNAL_DAC_L0+k] = sCh0LeftOut[i];
		sTxBuffer1[INTERNAL_DAC_R0+k] = sCh0RightOut[i];
		
		// use the builtin circular buffer to update the index
		k = __builtin_circindex(k, 4, 4*INPUT_SIZE*TOTAL_FRAME);
		
	}

}


