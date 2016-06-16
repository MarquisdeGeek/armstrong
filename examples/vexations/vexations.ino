
#include <armstrong.h>
#include "data.h"

/*
Connect MIDI out the TX (pin 1) as usual, and just let it run.

If you're using 'performance mode', connect pin 8 to ground. Disconnecting
this connection lets it float high, and the 'music' will start 2 seconds
afterwards.
*/
#define PERFORMANCE_MODE   0

const int chlOutput = 0;


void playPattern(const byte *pData, const int span, const int variance/*between 0 and 100%*/) {

   do {
     // We slow down as time goes on, to prolong the agony
     long duration = (long)*pData++ * (500+variance);
     byte volume;

     // We vary each note/chord by 5% over time, to simulate the players apathy
     duration += random((duration * 5) / 100);
     
     for(int i=0;i<span;++i) {
       volume = 70;
       // We always vary the volume, because no human is exact
       volume += random(0, 15);
       volume -= random(0, 10);
       
       // we vary it a bit more, because no human can be bothered after the first 4 bars
       volume += random(0, variance/10);
       volume -= random(0, variance/7);
       
       anpPlayNote(chlOutput, pData[i], volume);
     }
     delay((duration * 15) / 16);
     
     for(int i=0;i<span;++i) {
       anpStopNote(chlOutput, pData[i]);
     }
     delay((duration * 1) / 16);
     
     if (pData[0] == 0) {
       return;
     }
     
     pData += span;
    
   } while(true);
}


void setup() {

   ancInitialize(OUTPUT_MIDI);
   ancAssignChannelToMIDI(chlOutput, 0);

   int repeats = 1;// or 840 if you're crazy ;)

#if PERFORMANCE_MODE
   pinMode(8, INPUT);
   while(digitalRead(8) == LOW) {
     // nop
   }
   delay(2000);
   repeats = 840;
#endif   

  for(int i=0;i<repeats;++i) {
    int variance = (i*100) / repeats;
    
    playPattern(theme,   1, variance);
    playPattern(melody1, 3, variance);
    playPattern(theme,   1, variance);
    playPattern(melody2, 3, variance);
  }
}

void loop() {
}
