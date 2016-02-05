/*
An example of Armstrong - THE Arduino Music System

http://marquisdegeek.com/code_armstrong


The circuit consists of two potential dividers - one for each of volume and pitch.

Connect an LDR between 5V and A0, an resister between A0 and GND (this controls volume)
Connect an LDR between 5V and A1, an resister between A1 and GND (this controls pitch)

Add a piezo or speaker to pin 3.

*/
#include <armstrong.h>

const int pinPiezo = 3;
const int pinVolumeLDR = A0;
const int pinPitchLDR = A1;

const int chlOutputSpeaker = 0;
const int chlInputVolumeLDR = 1;
const int chlInputPitchLDR = 2;

void setup() {

	// Prepare the Armstrong system. By default, all notes go to the
	// electronics plugged into the board. It can also be sent to the
	// serial port (OUTPUT_SERIAL) or MIDI (OUTPUT_MIDI);
	ancInitialize(OUTPUT_LOCAL);

	// aai = Armstrong Analogue Input
	aaiInitialize();

	// Prepare two input channels, one for volume, and one for pitch.
	aaiInitializeScalar(chlInputPitchLDR, pinPitchLDR, anfGetNotePitch(MIDI_NOTE_C6),anfGetNotePitch(MIDI_NOTE_C2));
	aaiSetInputScale(chlInputPitchLDR, 1, 500);

	aaiInitializeScalar(chlInputVolumeLDR, pinVolumeLDR, MAX_VOLUME, 0);
	aaiSetInputScale(chlInputVolumeLDR, 100, 200);

	// Output is currently set to piezo
	ancAssignChannelToPin(chlOutputSpeaker, pinPiezo);

	// Let the user know we're ready
	ampPlayString(chlOutputSpeaker, "T125O4L16cO5c");

	ancNoteOn(chlOutputSpeaker, aaiGetState(chlInputPitchLDR), DEFAULT_VOLUME);
}


void loop(){
	aaiUpdate();

	ancNoteChangePitch(chlOutputSpeaker, aaiGetState(chlInputPitchLDR));
	ancNoteChangeVolume(chlOutputSpeaker, aaiGetState(chlInputVolumeLDR));

	// Now update the note controller to oscillate the piezo/speaker
	// accordingly...
	ancUpdate();
}
