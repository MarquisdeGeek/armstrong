/*
An example of Armstrong - THE Arduino Music System

http://marquisdegeek.com/code_armstrong

*/

#include <armstrong.h>

// Armstrong setup
const int pinPiezo = 3;
const int chlOutputSpeaker = 0;

// Armstrong state
int currentPitch;


void setup(void) {

	// Prepare the Armstrong system. By default, all notes go to the
	// electronics plugged into the board. It can also be sent to the
	// serial port (OUTPUT_SERIAL) or MIDI (OUTPUT_MIDI);
	ancInitialize(OUTPUT_LOCAL);
	
	// Create an abstraction between the software (logical) channel and
	// the hardware pin.
	ancAssignChannelToPin(chlOutputSpeaker, pinPiezo);

	currentPitch = anfGetNotePitch(MIDI_NOTE_C1);
	ancNoteOn(chlOutputSpeaker, currentPitch, DEFAULT_VOLUME);
}

void loop() {
	ancUpdate();

	currentPitch += 2;
	if (currentPitch > 3500) {
		ancNoteOff(chlOutputSpeaker);
	} else {
		ancNoteChangePitch(chlOutputSpeaker, currentPitch);
	}
}
