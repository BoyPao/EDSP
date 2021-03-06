Project Name: LedButton

Project Type:
	ADSP-BF533 [ ]
	ADSP-BF537 [ ]
	ADSP-BF561 [X]	 
	
Hardware Used:
	EZ-Kit LEDs and Buttons 
	
System Services Components Used:
	DMA Manager                         [X]   Deferred Callback Manager   [ ]
	Interrupt Manager                   [X]   Timer Module                [ ]
	Power Management Module             [X]   Flag Module                 [X]
	External Bus Interface Unit Module  [X]   Port Module                 [ ]
	
Example Overview:
	This is a demo program that shows how to use the EZ-Kit utilities for the 
	LEDs and push buttons for the ADSP-BF561 EZ-KIT.
	
	This example is a single-core application in that Core A does all the work; 
	Core B is simply kept at IDLE. This is important as use of timers and  changes 
	to the Phased Locked Loop (PLL) for Power Management changes require Core B to 
	either remain inactive or to be idled. This is the case on board reset. However, 
	when the board is connected to an emulator session within the VisualDSP++ 4.0 IDDE, 
	Core B is awoken and must be returned to IDLE before the PLL can be 
	reprogrammed. This is achieved by loading a small piece of code which keeps
	Core B at IDLE.
	
	Once loaded, Debug->Multiprocessor->Run can be used to start the program. There is 
	no need to halt Core B after that, and must be kept running as long as Core A is 
	running. Should you wish to create a loader file, for downloading to Flash memory,
	only p0.dxe is required to be downloaded, as the conditions for Core B will be 
	satisfied on board reset.

	The LED's are turned off when you start the program.
	
	The program starts out in "polled mode", (polling the buttons, to see if one 
	is pressed). 
 
	The first three switches can be tested by pressing the first three buttons.
	When you press a button, all LED's except one will be turned off, in accordance 
	with the following chart:
	
	SW6 (first button) - LED5
	SW7 (second button) - LED6
	SW8 (third button) - LED7

	If you press the LAST button, (SW9) then you go into the next phase of the 
	program, which is Interrupt Driven mode.

	In this mode, callbacks are installed for each button.
	You can do the same thing you did in polled mode, press the three buttons and 
	see the three LED's go on.

	If you press the last button (SW9) it takes you, again, to the next phase, in which
	you can start the LED"s "cycling" and stop them again.  

	In this mode, each time the first button is pressed, the LED corresponding to the
	first button is toggled.  When the second button is pressed, all LED's are cycled 
	until either the first button is pressed again or until the last button is pressed,
	at which time the program is ended.

File Structure:
	The structure of the example follows the 5 project method that is advocated
	in the VisualDSP Help system. The LDF is automatically generated by VisualDSP.   

	"LEDButton" - main project directory
		"./LEDButton_projects.dpg" -  main project group file (groups all projects together)
		"./LEDButton.dpj" -  main project file 
		"./LEDButton.c" -  main program
		"./ezkitutilities.h" -  ezkit include file
		"./ezkitutilities.c" -  ezkit utilities

	"./coreA" -  code that runs on core A
        "./CoreA.dpj - project file for core A
        
	"./coreB" -  code that runs on core B
        "./CoreB.dpj - project file for core B
		"./idle.c" -  keeps core B in an idle loop

	"./sml2" -  code that gets located in L2 SRAM
        "./SM_L2.dpj - Project file for sml2
		"./stub.c" -  nothing

	"./sml3" -  code that gets located in SDRAM
        "./SM_L3.dpj - project file for sml3
		"./stub.c" -  nothing



System Configuration:
	Jumpers: SW4 (push button enable switch)
		 1:ON, 2:ON, 3:ON, 4:ON, 5:OFF, 6:OFF

	External connections: None

	
Getting Started:

	1)	Load Project group file "LEDButton_projects.dpg"

	2)  Select debug or release configuration.
	
	3)	Click "Rebuild All"
	
	4)  Wait for program to build

	5)	Click "OK" in the Load Processor Configuration dialog

	6)	Select "Multiprocessor Run" icon or "Debug->Multiprocessor->Run" or press (Ctrl-F5)

	7)	When finished, select "Multiprocessor Run" icon or "Debug->Multiprocessor->Halt" to 
	    stop the program.	
	
