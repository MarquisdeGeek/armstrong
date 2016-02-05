/*
An example of Armstrong - THE Arduino Music System

http://marquisdegeek.com/code_armstrong

*/

#include <armstrong.h>

// Armstrong setup
const int pinPiezo = 3;
const int chlOutputSpeaker = 0;

void setup() {

	// Prepare the Armstrong system. By default, all notes go to the
	// electronics plugged into the board. It can also be sent to the
	// serial port (OUTPUT_SERIAL) or MIDI (OUTPUT_MIDI);
	ancInitialize();

	// Create an abstraction between the software (logical) channel and
	// the hardware pin.
	ancAssignChannelToPin(chlOutputSpeaker, pinPiezo);

	// Play some music
	ampPlayString(chlOutputSpeaker, "T125O3L16cdefgabO4c");
}


void loop(){
}