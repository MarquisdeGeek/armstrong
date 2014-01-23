#include "armstrong.h"
#include "waveform.h"


#define CHANNEL_OUTPUT_PIEZO_SPEAKER	 0
#define EXAMPLE_LOGGING                  0

int piezoPin = 8;
int pinReset = 2;
int pinForce = 7;  // output set to high, to pull up pinReset

void (*pExampleList[])() = {ex0,ex1,ex2,ex3, ex4, ex5, ex6, ex7};
void (*pExampleListUpdate[])() = {nofn, nofn, nofn, ex3update, ex4update, ex5update, ex5update, ex5update};

int lastExample = -1;

#if EXAMPLE_LOGGING==1
#define exLog Serial.print
#define exLogLine Serial.println
#else
#define exLog (void *)
#define exLogLine (void *)
#endif


void runExamplesFrom(int example) {
  exLog("Example ");
  exLog(example);
  exLogLine(" now starting..");
  
  lastExample = example;
  pExampleList[example]();
}

void     updateExample() {
   if (lastExample < 0) {
       return;
   }
   //
   pExampleListUpdate[lastExample]();
}

void nofn() {
  exampleEnds();
}

void exampleEnds() {

  // Wait until the wire is removed from the pin (i.e. it goes high from pull-up)
  while (digitalRead(pinReset) == LOW) {
      delay(1); // nop
  }


  // Wait for the wire to touch it (goes low)
  exLogLine("Example over - let go of the wire");
  while (digitalRead(pinReset)) {
    delay(1);
  }
  // For wait for it to be removed
 while (digitalRead(pinReset) == LOW) {
      delay(1); // nop
  }

  exLogLine("Now TAP the wire to start next example");
  exLogLine("Thank you!");


  // Any more examples?
  ++lastExample;
  if (lastExample >= sizeof(pExampleList)/sizeof(pExampleList[0])) {
    exLogLine("End of example list!");
    lastExample = 0;
  }
  
  runExamplesFrom(lastExample);
  
}

//
// Example code
//

// 0 - standard tone
void ex0() {
   //tone(piezoPin, 262, 750);  // C3 - removed for demo because I want to demonstrate the waveform library, that requires the interutps
   ex1();
}

// 1 - Using a simple play string
void ex1() {
  ampPlayString(CHANNEL_OUTPUT_PIEZO_SPEAKER, "O3c");
}

// 2 - A more involved play string
void ex2() {
  ampPlayString(CHANNEL_OUTPUT_PIEZO_SPEAKER, "T130L16O3eaebegaeO4cO3eO4dO3ebO4cO3eaebegaeO4cO3eO4dO3ebO4c");
}

// 3 - Constant pitch, until wire pulled
void ex3() {
  ancNoteOn(CHANNEL_OUTPUT_PIEZO_SPEAKER, anfGetNotePitch(MIDI_NOTE_C3), DEFAULT_VOLUME);
}

void ex3update() {
  if (digitalRead(pinReset) == LOW) {
    ancNoteOff(CHANNEL_OUTPUT_PIEZO_SPEAKER);
    while(digitalRead(pinReset) == LOW) {
      delay(1);
    }
    exampleEnds();
  }
}

// 4 - Rising pitch
int ext4Pitch;
void ex4() {
   ext4Pitch = anfGetNotePitch(MIDI_NOTE_C1);
  ancNoteOn(CHANNEL_OUTPUT_PIEZO_SPEAKER, ext4Pitch, DEFAULT_VOLUME);
}

void ex4update() {
   ext4Pitch += 2;
   if (ext4Pitch > 3500) {
       ancNoteOff(CHANNEL_OUTPUT_PIEZO_SPEAKER);
      exampleEnds();
  } else {
    ancNoteChangePitch(CHANNEL_OUTPUT_PIEZO_SPEAKER, ext4Pitch);
  }
}

void loop() {
  updateExample();
  ancUpdate();  
}

tBYTE wave[256];
void ex5() {

  // First AWF method
  awfInitialize();
  
  //
  float harmonics[] = {1.0f};

  awfGenerate8(wave, sizeof(wave), harmonics, sizeof(harmonics)/sizeof(harmonics[0]));
  awfPlaySample8(wave, sizeof(wave));
}

void ex5update() {
  if (digitalRead(pinReset) == LOW) {
    while(digitalRead(pinReset) == LOW) {
      awfFadeSample8(wave, sizeof(wave), 1);
      delay(1);
    }
    awfStop();
    exampleEnds();
  }
}

void ex6() { 
  float harmonics[] = {0.7f, 0, 0.5f};

  awfGenerate8(wave, sizeof(wave), harmonics, sizeof(harmonics)/sizeof(harmonics[0]));
  awfPlaySample8(wave, sizeof(wave));
}


void ex7() { 
  float harmonics[] = {0.7f, 0, 0.5f, 0, 0, 0.3f};

  awfGenerate8(wave, sizeof(wave), harmonics, sizeof(harmonics)/sizeof(harmonics[0]));
  awfPlaySample8(wave, sizeof(wave));
}




void setup() {
  
  pinMode(pinReset, INPUT);
  pinMode(pinForce, OUTPUT);
  digitalWrite(pinForce, HIGH);
  
  ancInitialize(OUTPUT_LOCAL);

  ancAssignChannelToPin(CHANNEL_OUTPUT_PIEZO_SPEAKER, piezoPin);

  ampPlayString(CHANNEL_OUTPUT_PIEZO_SPEAKER, "O3cO4c");

  Serial.begin(38400);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  
  exLogLine("Setup");
  exLogLine("Tap the wire to start..");


  while (digitalRead(pinReset)) {
    delay(1);
  }
  while (!digitalRead(pinReset)) {
    delay(1);
  }
  
  exLogLine("Starting now...");


  runExamplesFrom(0);

}


