/*
An example of Armstrong - THE Arduino Music System

http://marquisdegeek.com/code_armstrong

*/
#include <armstrong.h>

// Armstrong setup
const int pinPiezo = 3;
const int chlOutputSpeaker = 0;
const int noteLowest = MIDI_NOTE_C1;
const int noteHighest = MIDI_NOTE_C3;
const boolean quantizeToScale = true;


// Armstrong state
const int gTouchScreenSamplePeriod = 8; 
int gTouchScreenSampleCount = 0;
float gPitchLowest;
float gPitchHighest;

/*
ts.getPoint() is so slow that there is a small, but noticeable, gap
in the sound as the touch-screen data is collected. Therefore we only
poll is N frames. This N is gTouchScreenSamplePeriod

Consequently gTouchScreenSamplePeriod controls the touch latency and the
gap between sounds, caused by getPoint()
*/



// *** SPFD5408 change -- Begin
#include <SPFD5408_Adafruit_GFX.h>    // Core graphics library
#include <SPFD5408_Adafruit_TFTLCD.h> // Hardware-specific library
#include <SPFD5408_TouchScreen.h>
// *** SPFD5408 change -- End

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

// Circuit
#define YP A3 
#define XM A2 
#define YM 9  
#define XP 8 

// Calibrate values
#define TS_MINX  100
#define TS_MINY  100
#define TS_MAXX  832
#define TS_MAXY  832

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
// optional
#define LCD_RESET A4

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);


void setup(void) {

  // Prepare Armstrong, as usual
  ancInitialize(OUTPUT_LOCAL);
  ancAssignChannelToPin(chlOutputSpeaker, pinPiezo);
  ancNoteOn(chlOutputSpeaker, 0, DEFAULT_VOLUME);

  // Define limits
  gPitchLowest = anfGetNotePitch(noteLowest);
  gPitchHighest = anfGetNotePitch(noteHighest);

  // Let the user know we're ready
  ampPlayString(chlOutputSpeaker, "T125O4L16cO5c");

  // I can't remember why this is here!?!?! :(
  pinMode(13, OUTPUT);
}

#define MINPRESSURE 10
#define MAXPRESSURE 1000

void loop()
{
  ancUpdate();

  if (++gTouchScreenSampleCount < gTouchScreenSamplePeriod) {
    return;
  }
  gTouchScreenSampleCount = 0;

  TSPoint pt = ts.getPoint(); 
  if (pt.z >= MINPRESSURE && pt.z < MAXPRESSURE) {
    // Volume has no effect on piezo-based local sounds, but does on MIDI etc
    pt.x = map(pt.x, TS_MINY, TS_MAXY, 0, MAX_VOLUME);
    pt.y = map(pt.y, TS_MINX, TS_MAXX, gPitchLowest, gPitchHighest);

    if (quantizeToScale) {
      pt.y = anfQuantizePitch(pt.y);
    }

    if (ancIsNoteOn(chlOutputSpeaker)) {
      ancNoteChangePitch(chlOutputSpeaker, pt.y);
      ancNoteChangeVolume(chlOutputSpeaker, pt.x);
    } else {
      ancNoteOn(chlOutputSpeaker, pt.y, pt.x);
    }

  } else {
    ancNoteOff(chlOutputSpeaker);
  }
}
