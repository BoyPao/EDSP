

#include "main.h"


// set up DMA descriptors (one for each frame, then repeat)
// small descriptor model, only start address needs to be fetched

tDMA_descriptor DMA_PPI0_first = 	{&DMA_PPI0_second, sFrame0};
tDMA_descriptor DMA_PPI0_second = 	{&DMA_PPI0_third , sFrame1};
tDMA_descriptor DMA_PPI0_third = 	{&DMA_PPI0_fourth, sFrame2};
tDMA_descriptor DMA_PPI0_fourth = 	{&DMA_PPI0_first , sFrame3};


volatile int current_in_Frame  = -1;		// 0, 1, 2 or 3  ... indicates the last frame that was received COMPLETELY

bool Set_PACK32 = false;
bool Set_Entire_Field = false;


// User program
void main() {

	// unblock Core B if dual core operation is desired	
#ifndef RUN_ON_SINGLE_CORE
	*pSICA_SYSCR &= 0xFFDF; // clear bit 5 to unlock  
#endif

	// set Clocks
	Set_PLL( (short)(CORECLK/CLKIN), (short)(CORECLK/SYSCLK));			// sets Core and System Clocks to the values defined in system.h 

	// initialise SDRAM
	InitSDRAM();
	*pTC_PER = 0x0770;			// set DMA traffic control register to favour unidirectional transfers to SDRAM

	// initialise Video Encoder ADV7179
	Reset_ADV7179();

	// initialise Video Decoder ADV7183
	Reset_ADV7183();
	
	
	// initialise PPI0 and associated DMA channel for Video IN
	current_in_Frame  = -1;				// no frames received yet
	semaphore_frames_received = false;	// do not start output stream yet

	Set_Entire_Field = false;
#ifdef ENTIRE_FIELD_MODE
	Set_Entire_Field = true;
#endif

	Set_PACK32 = false;
#ifdef PACK_32
	Set_PACK32 = true;
#endif
	
	InitPPI0(Set_Entire_Field, Set_PACK32, &DMA_PPI0_first, PIXEL_PER_LINE, LINES_PER_FRAME);
	

		
	// initialise Interrupts
	InitInterrupts_coreA();
	
	
	// enable transfers
	EnablePPI0();


	
	// main loop, just wait for interrupts
	while(1) {
		idle();		//   do nothing
		// check for PPI framing error
		if (*pPPI0_STATUS & FT_ERR)
		{
			// error occurred -- clear error and restart video transfer
			*pPPI0_STATUS &= ~FT_ERR;
			semaphoreResetVideo = true;
			
			while(semaphoreResetVideo);     // wait for core B to reset video
			
			DisablePPI0();
			current_in_Frame  = -1;         // no frames received yet
			semaphore_frames_received = false;
			
			InitPPI0(Set_Entire_Field, Set_PACK32, &DMA_PPI0_first, PIXEL_PER_LINE, LINES_PER_FRAME);
			
			EnablePPI0();
		}

	}	// while(1)
		
	
}		// main

