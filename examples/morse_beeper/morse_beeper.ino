/*
An example of Armstrong - THE Arduino Music System

http://marquisdegeek.com/code_armstrong


The circuit consists of a switch, taking pin 12 to either 5V or GND

Connect a resistor between GND and pin 12.
Connect a normally-open switch between pin 12 and 5v.

Add a piezo or speaker to pin 3.

*/

#include <armstrong.h>

// Armstrong setup
const int pinPiezo = 3;
const int pinButton = 12;

const int chlOutputSpeaker = 0;
const int chlInputButton = 0;    // input channels are different to output channels

void setup() {

	// Prepare the Armstrong system. By default, all notes go to the
	// electronics plugged into the board. It can also be sent to the
	// serial port (OUTPUT_SERIAL) or MIDI (OUTPUT_MIDI);
	ancInitialize();

	// Create an abstraction between the software (logical) channel and
	// the hardware pin.
	ancAssignChannelToPin(chlOutputSpeaker, pinPiezo);

	// Create an abstraction between the software (logical) channel and
	// the hardware pin. This changes the state when pressed and when
	// released.
	adiInitializePushButton(chlInputButton, pinButton);
}


void loop(){
	adiUpdate();

	if (adiIsPressed(chlInputButton)) {
		ancNoteOn(chlOutputSpeaker, anfGetNotePitch(MIDI_NOTE_C3), DEFAULT_VOLUME);
	} else if (adiIsPressed(chlInputButton)) {
   		ancNoteOff(chlOutputSpeaker);
	}

	ancUpdate();
}
