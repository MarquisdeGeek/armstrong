armstrong
=========

Amstrong is a collection of routines to make the building and prototyping of musical instruments on the Arduino very much easier. It abstracts the hardware pins from their function and supports serial communications, allowing sounds to be played on remote hardware (PC or synthesizer) without additional coding.

It comprises of Arduino source code to:

* Process analog input
* Process digital input
* Play individual notes
* Play music (using the Armstrong Music Format)
* Transmit the notes played to a PC (via USB/serial)
* Send MIDI out messages

It provides separate, loosely-coupled, components for each part of the signal chain (input-processing-output) and abstracts the hardware ideas of 'pins' with 'channels'. In this way, the code of each component does not need to change if your circuit design does. Furthermore, you need not specify a hardware pin at all, since the abstraction allows the note data to be sent by serial (either to the PC, or a MIDI keyboard) to trigger remote sounds.

As an example, you could create a channel with a push button,

   adiInitializePushButton(CHANNEL_INPUT_NOTETEST_SWITCH, buttonPin);
Read its state inside 'loop'
   if (adiDidStateChange(CHANNEL_INPUT_NOTETEST_SWITCH)) {
      // state changed!
      int state = adiGetState(CHANNEL_INPUT_NOTETEST_SWITCH);
Then place notes onto the various output channels. Such as,
   ancNoteOn(CHANNEL_OUTPUT_PIEZO_SPEAKER, MIDI_NOTE_C4, DEFAULT_VOLUME);
The ancUpdate() function is also called each time around loop(), and it is this which controls the pizeo/speaker. The pitch and volume of each channel can be changed at run-time.
There are also examples, and PC sample code for handling the serial protocol.


The Arduino components

They are:
aai - Armstrong Analog Input

Current supports scaled inputs and one-shot triggers using piezo transducers.

1. Scaled inputs take the hardware input levels (0-1023 for Arduino) and re-adjust them to fit between a specified range. Floor and ceiling limits are applied. You can also limit the input range.
2. Piezo triggers fluctuate, so this watches for when the input rises above the 'high' threshold. This indicates a 'hit'. It must then wait for the hardware to settle down and fall below the 'low' threshold before it can be re-triggered.

adi - Armstrong Digital Input

This handles digital input switches. It currently supports toggle and push buttons.

1. Toggle buttons change state on every 'down' press on the button. This means it takes two presses to return to its original state.
2. Push buttons reflect the simple 'push-to-make' switches.

The biggest benefit of using this abstraction is to simplify the state logic, as you can easily watch for the 'up' or 'down' motion without extra code.

anc - Armstrong Note Control

This contains the main output code for handling play individual notes. The basic interface comprises of NoteOn-ChangePitch-ChangeVolume-NoteOff, which the specific parameters being sent to either the local hardware speaker, the serial output to PC, or a MIDI port. This may be changed with ' ancSetOutputMode'.

anp - Armstrong Note Play

This handles basic note playback, using the note control library. The oscillation of the piezo/speaker for each note can be handled by either:

1. The ancUpdate methods
2. This code, internally (with anpPlayNoteFor)

In the first case, you start a note with anpPlayNote (and stop it with the equivalent anpStopNote) and ensure ancUpdate is called within loop.

In the second, ancUpdate is called automatically.

amp - Armstrong Music Player

Utility code to play complete pieces of music, as described by the Armstrong music format. This format is state system using a list of notes (C, D, E, etc) which are played with the current duration, volume, and octave. It uses the play functions anp.
The commands are:

* Vn - Change volume of subsequent notes (0<=n<=255)
* On - Change octave of subsequent notes (unlimited, but 0<=n<=9)
* Ln - Change octave of subsequent notes, where n is 32 for crochets, 16 for quavers, and so on.

The format has been created to provide small footprints of music, consequently the 'length' parameter above is 1/12 the ppqn size given for MIDI_NOTE_CROCHET et al in noteplay.h

Sharps and flats may be created by suffixing the note name with + and - respectively.

anf - Armstrong Note Frequencies

These are simple helper functions that understand frequencies. You can convert between MIDI note ID and frequency, and quantize to the nearest note.


Notes

MIDI out is untested
FMOD is required for the Wintel32 binary

