/*
* notecontrol.pde - Handling code to play individual notes.
*                    Part of the 'Armstrong' Music System.
*
* Version 1.0
*
*  AUTHOR: Steven Goodwin (StevenGoodwin@gmail.com)
*          Copyright 2008, Steven Goodwin
*
*  This program is free software; you can redistribute it and/or
*  modify it under the terms of the GNU General Public License as
*  published by the Free Software Foundation; version 2.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software
*  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#include "armstrong.h"

// Do we have structs?
char hex[] ={'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};

int g_iOutputMode;
int lastChannelSent;

void sendValue(int channel, char precommand, int n);
void sendMessage(int channel, char *command);
void midiOut(int midi_channel, int message, int pitch, int velocity);

struct {
	int frequency;
	int period;
	int volume;
	int mute;
	int mode;
	int pinAssignment;
	//
	int midi_channel;
	//
	int lastVolumeSent;
	int lastPeriodSent;
} noteInfo[MAX_CHANNELS];

// MIDI messages
#define MIDI_NOTE_ON    0x90
#define MIDI_NOTE_OFF   0x80

void ancInitialize(int initialMode) {
	int i;
	for(i=0;i<MAX_CHANNELS;++i) {
		noteInfo[i].frequency = -1;
		noteInfo[i].period = -1;
		noteInfo[i].lastVolumeSent = noteInfo[i].lastPeriodSent = -1;
		noteInfo[i].mute = 0;
		noteInfo[i].volume = HIGH;
		noteInfo[i].mode = NOTE_PINMODE_DIGITAL;
		noteInfo[i].pinAssignment = -1;
		// MIDI only
		noteInfo[i].midi_channel = i & 0xf;
	}
	lastChannelSent = -1;
	g_iOutputMode |= initialMode;  // by or'ing them, we can re-init, without problems
	//
	if (g_iOutputMode & OUTPUT_MIDI) {
		Serial.begin(31250);
	} else if (g_iOutputMode & OUTPUT_SERIAL) {
		Serial.begin(57600);
	}
}

void ancRelease() {
	int i;
	for(i=0;i<MAX_CHANNELS;++i) {
		ancNoteOff(i, noteInfo[i].frequency);
	}
}

void ancSetOutputMode(int m) {
	g_iOutputMode = m;
}

int ancGetOutputMode() {
	return g_iOutputMode;
}

void ancAssignChannelToPin(int channel, int pin) {
	noteInfo[channel].pinAssignment = pin;
	pinMode(pin, OUTPUT); 
}

void ancAssignChannelToMIDI(int channel, int midi_channel) {
	noteInfo[channel].mode = NOTE_PINMODE_MIDI;
	noteInfo[channel].midi_channel = midi_channel & 0xf;
}

int ancIsNoteOn(int channel) {
	if (noteInfo[channel].frequency == -1) {
		return 0;
	} else {
		return 1;
	}
}

void ancNoteOn(int channel, int pitch, int volume) {
	if (noteInfo[channel].mute) {
		return;
	}

	int period = ahwGetTimingPeriod(pitch);

	// Don't bother process the 'off' and 'on' if it's the same note
	if (period == noteInfo[channel].period && volume == noteInfo[channel].lastVolumeSent) {
		return;
	}

	if (noteInfo[channel].frequency != -1 && noteInfo[channel].mode != NOTE_PINMODE_MIDI) {
		ancNoteOff(channel, noteInfo[channel].frequency);
	}

	noteInfo[channel].frequency = pitch;
	noteInfo[channel].period = period;
	noteInfo[channel].lastPeriodSent = period;
	noteInfo[channel].lastVolumeSent = volume;

	if (noteInfo[channel].mode == NOTE_PINMODE_MIDI) {
		// Volume is 0-MAX_VOLUME, i.e. 255, whereas MIDI velocity is 0-127, so we divide by 2
		midiOut(noteInfo[channel].midi_channel, MIDI_NOTE_ON, pitch, volume/2);
	}

	sendValue(channel, '#', pitch);
}

void ancNoteChangePitch(int channel, int newPitch) {
	if (noteInfo[channel].period != -1) {
		int newPeriod = ahwGetTimingPeriod(newPitch);

		if (noteInfo[channel].period != newPeriod) {  
			noteInfo[channel].period = newPeriod;
			noteInfo[channel].lastPeriodSent = newPeriod;
			sendValue(channel, '$', newPitch);
		}
	}
} 

void ancNoteChangeVolume(int channel, int newVolume)  {
//::Serial.print("new volume");  ::Serial.print("    ");
//::Serial.print(newVolume);  ::Serial.print("    ");

	if (noteInfo[channel].frequency != -1 && noteInfo[channel].volume != newVolume) {
		noteInfo[channel].volume = newVolume;
		noteInfo[channel].lastVolumeSent = newVolume;
		sendValue(channel, '^', newVolume);
	}
}

void ancNoteOff(int channel, int pitch) {
	if (pitch == -1) {
		pitch = noteInfo[channel].frequency;
	}

	if (pitch != -1) {
	    if (noteInfo[channel].mode == NOTE_PINMODE_MIDI) {
	      midiOut(noteInfo[channel].midi_channel, MIDI_NOTE_OFF, pitch, 0);
	    }

		sendMessage(channel, "!");
		noteInfo[channel].frequency = noteInfo[channel].period = -1;
	}
}

void ancUpdate() {
	if (g_iOutputMode & OUTPUT_LOCAL) {
		for(int i=0;i<MAX_CHANNELS;++i) {
			ancUpdateChannel(i);
		}
	}
}

void ancAssignModeToChannel(int channel, int cm) {
	int pin = noteInfo[channel].pinAssignment;
	switch(pin) {
		// Newer Arduinos only (i.e. non-ATmega8)
	case 3:
	case 5:
	case 6:
		// All Adruinos
	case 9:
	case 10:
	case 11:
		noteInfo[channel].mode = cm;
		break;
		// All other pins can _only_ be digital
	default:
		noteInfo[channel].mode = NOTE_PINMODE_DIGITAL;
		break;
	}
}

// TODO: V2. Can we time microseconds accurately enough, to let us return immediately without
// delay calls, and trigger write HIGH/LOW on time>necessary
void ancUpdateChannel(int channel) {
  	if (g_iOutputMode & OUTPUT_LOCAL) {

		if (noteInfo[channel].period != -1) {
			int vol = noteInfo[channel].volume;

			switch(noteInfo[channel].mode) {
				case NOTE_PINMODE_DIGITAL:
					vol = vol==0?0:HIGH;
					digitalWrite(noteInfo[channel].pinAssignment, vol); 
					delayMicroseconds(noteInfo[channel].period);
					digitalWrite(noteInfo[channel].pinAssignment, LOW); 
					delayMicroseconds(noteInfo[channel].period - 1);     // - 1 to make up for digitaWrite overhead
					break;
				case NOTE_PINMODE_ANALOG:
					vol = (vol > MAX_VOLUME) ? MAX_VOLUME : vol;
					//::Serial.println(vol);
					analogWrite(noteInfo[channel].pinAssignment, vol); 
					delayMicroseconds(noteInfo[channel].period);
					analogWrite(noteInfo[channel].pinAssignment, LOW); 
					delayMicroseconds(noteInfo[channel].period - 1);     // - 1 to make up for digitaWrite overhead
					break;
			}
		}
	}

}

void midiOut(int midi_channel, int message, int pitch, int velocity) {

  if (g_iOutputMode & OUTPUT_MIDI) {
    int key = 45 + anfGetPitchAsNote(pitch);
    int status_byte = message | midi_channel;

    Serial.write(status_byte);
    Serial.write(key);
    Serial.write(velocity);
   }
}

void sendValue(int channel, char precommand, int n) { 
	char buffer[8]; 
	int idx = 0;

	if (g_iOutputMode & OUTPUT_SERIAL) {
		buffer[0] = precommand;

		while(n > 0) {
			buffer[++idx] = 'J' + (n&0xf);
			n >>= 4;
		}  
		// make the last character lower case, so the reader knows when to finish
		buffer[idx] += 'a'-'A';
		buffer[++idx] = '\0';

		sendMessage(channel, buffer);
	}
}

void sendMessage(int channel, char *command) {
	if (g_iOutputMode & OUTPUT_SERIAL) {
		if (channel != lastChannelSent) {
			lastChannelSent = channel;
			Serial.print(hex[channel]);
		}
		Serial.print(command);
	}
}
