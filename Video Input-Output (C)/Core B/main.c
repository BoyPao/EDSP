#include "main.h"

// set up DMA descriptors (one for each frame, then repeat)
// small descriptor model, only start address needs to be fetched

tDMA_descriptor DMA_PPI1_first = 	{&DMA_PPI1_second, sFrame0};
tDMA_descriptor DMA_PPI1_second = 	{&DMA_PPI1_third , sFrame1};
tDMA_descriptor DMA_PPI1_third = 	{&DMA_PPI1_fourth, sFrame2};
tDMA_descriptor DMA_PPI1_fourth = 	{&DMA_PPI1_first , sFrame3};


volatile int current_out_Frame = -1;		// 0, 1, 2 or 3  ... indicates the last frame that is output COMPLETELY

bool Set_PACK32 = false;



// User program
void main() {

	// set Clocks
	Set_PLL( (short)(CORECLK/CLKIN), (short)(CORECLK/SYSCLK));			// sets Core and System Clocks to the values defined in system.h 

	// initialise PPI0 and associated DMA channel for Video IN
	
	current_out_Frame  = -1;				// no frames transmitted yet
	Set_PACK32 = false;
#ifdef PACK_32
	Set_PACK32 = true;
#endif

	InitPPI1(Set_PACK32, &DMA_PPI1_first, PIXEL_PER_LINE, LINES_PER_FRAME);
	
	// initialise Interrupts
	InitInterrupts_coreB();
	

	// now ready to kick off PPI output stream. 
	// Just wait for core A to indicate that a valid frame has been received
	while (!semaphore_frames_received);				// loop until core A sets the semaphore
	semaphore_frames_received = false;
	
    // start transfers
	EnablePPI1();


	
	
	// main loop, just wait for interrupts
	while(1) {
		idle();				//   do nothing

		// check for core A to reset the video
		if (semaphoreResetVideo)
		{
			semaphoreResetVideo = false;
			
			DisablePPI1();
			
			
			InitPPI1(Set_PACK32, &DMA_PPI1_first, PIXEL_PER_LINE, LINES_PER_FRAME);
			
			while (!semaphore_frames_received);				// loop until core A sets the semaphore
			semaphore_frames_received = false;
			
			EnablePPI1();
			
		}

	}	// while(1)
		
	
}		// main
