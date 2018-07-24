****************************************************************************************************

ADSP-BF561 EZ-KIT 

Video TalkThrough Example

Analog Devices, Inc.
DSP Division
Three Technology Way
Norwood, MA 02062

Date Created:	11/11/03

Change History:
04/30/04		: eliminated silicon revision check
 				  example now works for rev 0.2 and higher
				  
____________________________________________________________________________________________________

This example streams input from a video source to a output screen.
A video source is aquired frame-by-frame into SDRAM from the VideoDecoder. 
The frames are then output with a one-frame delay to the Encoder.
In this example, no processing is done on the frames. They are passed unaltered.
This is a dual core project. Please see section III. to get familiar with the project structure
____________________________________________________________________________________________________

Requires ADSP-BF561 silicon revision of 0.2 or higher

Note:

1. For BF561 EZ-Kit rev 2.0 and 2.1, if a USB or LAN extender board is used then the System bus frequency can be set to a maximum of 100 MHz.  
The PLL_CTL, PLL_DIV, and VR_CTL registers should be setup and configured to run the system bus at 100 Mhz. Please consult the BF561 Hardware reference and programmers guide for further information regarding programming the PLL.

2. The BF561 EZ-Kit rev 2.3 and higher will support the BF561SKBCZ-6V parts. For these rev of EZ-Kits if you are running the example at the default speed of 600MHz then an external voltage regulator will be required so that the part will be powered with 1.35V. The external regulator is required because the max voltage the internal regulator can supply is 1.3V which is not enough for operating above 533MHz.

A jumper and modified external regulation circuit is being added to the EZ-Kit design to support this. The FREQ[1:0] bits in VR_CTL will need to be set to 00 to disable the internal regulation. 
Follow the data sheet with the new VR_CTL settings that support the new ADSP-BF561SKBCZ-6V device.
____________________________________________________________________________________________________


CONTENTS

I.	 FUNCTIONAL DESCRIPTION
II.	 OPERATION DESCRIPTION
III. PROJECT STRUCTURE


I.    FUNCTIONAL DESCRIPTION

	Core A sets up Clock frequencies, SDRAM controller, the Video Encoder and Decoder, and PPI0 to 
	perform the video acquisiton in ITU-656	mode. Frames are stroed in SDRAM (in a circular fashion,
	4 frames at a time).
	
	Core B sets up PPI1 to perform video output. It then waits for the first valid frame in memory
	from Core A, and starts the transfers. GP output mode is used.

	Both the Video Encoder and Decoder are kept in theit power on configuration, no other configuration
	is done. If you need to change settings, you may do so by adding code for I2C routines.
	
	

II.  OPERATION DESCRIPTION

	- Open the "video_inout.dpg" project group in the VisualDSP Integrated Development
	  Environment (IDDE). Follow instructions in section III.
	  
	- Under the "Project" tab, select "Build Project" (program is then loaded automatically into DSP).

	- Connect a video NTSC/PAL CVBS source to the bottom right connector of J6 (video in/out jack)
	
	- Connect a video display monitor to the video output jack (top middle connector of J6)
		
	- Dipswitch SW5: set #1 to "off", all others to on
	- Dipswitch SW2: set #2,3,5 to "off", #1,4,6 to on
	- Dipswitch SW3: set #1,4 to "off", #2,3 to on
	
	- Run the executables by pressing "multiprocessor run" (CTRL-F5) on the toolbar. DO NOT use the 
	  single core (F5) button. You should see a copy of the input video in the output screen.
	  
	- Halt the processor ("multiprocessor halt" button). If you open a memory window and go to the
	  addresses of _sFrame0,1,2,3, you see the video data of the four frames.
	  
	- The main header file "system.h" contains #define statements for most of the system settings 
	  (clock frequencies etc)

		
III. PROJECT STRUCTURE

	This is a dual core project. It consists of a main project - containing only system defines and linker settings-
			".\video_inout.dpj",
	four sub-projects 		- containing the source code -
			".\coreA\coreA.dpj"					( code exclusive to core A, in L1 memory )
			".\coreB\coreB.dpj"					( code exclusive to core B, in L1 memory )
			".\Shared Memory L2\L2_SRAM.dpj"	( code that is shared between the cores, in on-chip L2 memory )
			".\Shared Memory L3\L3_SDRAM.dpj"	( code that is shared between the cores, in off-chip L3 memory, SDRAM ),
	and the project group that ties everything together:
			".\video_in_out.dpg".
			
			
	Follow this procedure to open and compile the project:
	- open the project group (File -> Open -> Project Group)
	- right click on the main project and re-build the project.
	

