/*
An example of Armstrong - THE Arduino Music System

http://marquisdegeek.com/code_armstrong


IMPORTANT: Because MIDI data uses the Serial device you MUST unplug
the circuit whilst uploading the sketch.

Circuit: https://www.arduino.cc/en/uploads/Tutorial/MIDI_schem.png

pin 1 (TX) : Connect to a 220 ohm resistor, which in turn connects to MIDI plug, pin 5
5v : Connect to a 220 ohm resistor, which in turn connects to MIDI plug, pin 4
Gnd : Connect to MIDI plug, pin 2

*/

#include <armstrong.h>

// Armstrong setup
const int chlOutputMIDI = 0;

void setup() {

   // Prepare the Armstrong system. By default, all notes go to the
   // electronics plugged into the board. It can also be sent to the
   // serial port (OUTPUT_SERIAL) or MIDI (OUTPUT_MIDI);
   ancInitialize(OUTPUT_MIDI);

   // Only one MIDI device is supported, so we attach the channel abstraction
   // to a MIDI channel. In this case channel 0 is 1.
   ancAssignChannelToMIDI(chlOutputMIDI, 0);

   // Play some music
   int scale[] = {0,2,4,5,7,9,11,12};
   for(int n=0;n<8;++n) {
      anpPlayNote(chlOutputMIDI, 60 + scale[n], 100);
      delay(300);
      anpStopNote(chlOutputMIDI, 60 + scale[n]);
      delay(200);
   }

   // Play via the alternate 'amp' method
   ampPlayString(chlOutputMIDI, "T125L16O4cO3bagfedc");
}

void loop() {
}
