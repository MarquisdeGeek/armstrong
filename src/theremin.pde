#include "notecontrol.h"
#include "noteplay.h"
#include "notepitch.h"
#include "musicplay.h"

#include "analoginput.h"

int piezoPin = 8;

#define CHANNEL_INPUT_VOLUME_LDR			1
#define CHANNEL_INPUT_PITCH_LDR				2

#define CHANNEL_OUTPUT_PIEZO_SPEAKER		0

void setup() {
	Serial.begin(57600);

	ancInitialize(OUTPUT_SERIAL);

	aaiInitialize();

	// Prepare two input channels, one for volume, and one for pitch.
	// These values match my circuit. Yours may vary.
	aaiInitializeScalar(CHANNEL_INPUT_PITCH_LDR, 0, anfGetNotePitch(MIDI_NOTE_C6),anfGetNotePitch(MIDI_NOTE_C2));
	aaiSetInputScale(CHANNEL_INPUT_PITCH_LDR, 1, 500);
	aaiInitializeScalar(CHANNEL_INPUT_VOLUME_LDR, 1, MAX_VOLUME, 0);
	aaiSetInputScale(CHANNEL_INPUT_VOLUME_LDR, 100, 200);

	// Output is currently set to piezo
	ancAssignChannelToPin(CHANNEL_OUTPUT_PIEZO_SPEAKER, piezoPin);

	ampPlayString(CHANNEL_OUTPUT_PIEZO_SPEAKER, "T125O3L16cdefgabO4c");
	
	// Update the analog input device(s)
	aaiUpdate();

	ancNoteOn(CHANNEL_OUTPUT_PIEZO_SPEAKER, aaiGetState(CHANNEL_INPUT_PITCH_LDR), DEFAULT_VOLUME);

}


void loop(){
	aaiUpdate();

	ancNoteChangePitch(CHANNEL_OUTPUT_PIEZO_SPEAKER, aaiGetState(CHANNEL_INPUT_PITCH_LDR));
	ancNoteChangeVolume(CHANNEL_OUTPUT_PIEZO_SPEAKER, aaiGetState(CHANNEL_INPUT_VOLUME_LDR));

	// Now update the note controller to oscillate the piezo/speaker
	// accordingly...
	ancUpdate();
}
