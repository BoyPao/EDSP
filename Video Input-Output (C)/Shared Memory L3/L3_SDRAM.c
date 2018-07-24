
#include "L3_SDRAM.h"


// Define the DMA buffers for each frame.
// Because of SDRAM performance, each frame must be in a different bank.
// The placement in different banks is obtained by creating seperate memory bank placements in the ldf file! 
//The NO_INIT pragma, avoids the memory initialization on a program load, reducing the executable loading time.

#pragma section("frame_buffer0",NO_INIT) 
volatile short sFrame0[Frame_Size];
#pragma section("frame_buffer1",NO_INIT) 
volatile short sFrame1[Frame_Size];
#pragma section("frame_buffer2",NO_INIT) 
volatile short sFrame2[Frame_Size];
#pragma section("frame_buffer3",NO_INIT) 
volatile short sFrame3[Frame_Size];

volatile bool semaphore_frames_received;		// signals to core B that output stream can be started
volatile bool semaphoreResetVideo;
