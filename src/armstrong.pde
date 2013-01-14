#include "notecontrol.h"
#include "noteplay.h"
#include "notepitch.h"
#include "musicplay.h"

#include "digitalinput.h"
#include "analoginput.h"

#if 0

int piezoPin = 8;
int buttonPin = 3;

#define CHANNEL_INPUT_NOTETEST_SWITCH		0
#define CHANNEL_OUTPUT_PIEZO_SPEAKER		0

void sssetup() {
	Serial.begin(9600);

	ancInitialize(OUTPUT_LOCAL | OUTPUT_SERIAL);

	adiInitialize();
	aaiInitialize();

	ancAssignChannelToPin(CHANNEL_OUTPUT_PIEZO_SPEAKER, piezoPin);
	adiInitializePushButton(CHANNEL_INPUT_NOTETEST_SWITCH, buttonPin);

	ampPlayString(CHANNEL_OUTPUT_PIEZO_SPEAKER, "T125O3L16cdefgabO4c");
}


void sloop(){
	adiUpdateDevice(CHANNEL_INPUT_NOTETEST_SWITCH);
//	adiUpdate();
	//aaiUpdate();

	// Do all input handling here!
	// e.g.
	if (adiDidStateChange(CHANNEL_INPUT_NOTETEST_SWITCH)) {
		// state changed!
		int state = adiGetState(CHANNEL_INPUT_NOTETEST_SWITCH);
		if (state) {
			ancNoteOn(CHANNEL_OUTPUT_PIEZO_SPEAKER, MIDI_NOTE_C4+1, DEFAULT_VOLUME);
		} else {
			ancNoteOff(CHANNEL_OUTPUT_PIEZO_SPEAKER);
		}
	}

	// Now update the note controller to oscillate the piezo/speaker
	// accordingly...
	ancUpdateChannel(CHANNEL_OUTPUT_PIEZO_SPEAKER);
}
#endif
