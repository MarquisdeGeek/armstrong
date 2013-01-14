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
#include "notecontrol.h"
#include "notepitch.h"

// Do we have structs?
char hex[] ={'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};

int mode;
int lastChannelSent;

static void sendValue(int channel, char precommand, int n);
static void send(int channel, char *command);

struct {
	int freq;
	int volume;
	int mute;
	int mode;
	int pinAssignment;
	//
	int lastVolumeSent;
	int lastFrequencySent;
} noteInfo[MAX_CHANNELS];

void ancInitialize(int initialMode) {
	int i;
	for(i=0;i<MAX_CHANNELS;++i) {
		noteInfo[i].freq = -1;
		noteInfo[i].lastVolumeSent = noteInfo[i].lastFrequencySent = -1;
		noteInfo[i].mute = 0;
		noteInfo[i].volume = HIGH;
		noteInfo[i].mode = NOTE_PINMODE_DIGITAL;
		noteInfo[i].pinAssignment = -1;
	}
	lastChannelSent = -1;
	mode = initialMode;
}

void ancRelease() {
	int i;
	for(i=0;i<MAX_CHANNELS;++i) {
		ancNoteOff(i);
	}
}

void ancSetOutputMode(int m) {
	mode = m;
}

int ancGetOutputMode() {
	return mode;
}

void ancAssignChannelToPin(int channel, int pin) {
	noteInfo[channel].pinAssignment = pin;
	pinMode(pin, OUTPUT); 
}

int ancIsNoteOn(int channel) {
	if (noteInfo[channel].freq == -1) {
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
	if (period == noteInfo[channel].freq) {
		return;
	}

	if (noteInfo[channel].freq != -1) {
		ancNoteOff(channel);
	}

	noteInfo[channel].freq = period;
	noteInfo[channel].lastFrequencySent = period;
	noteInfo[channel].lastVolumeSent = volume;

	sendValue(channel, '#', pitch);
}

void ancNoteChangePitch(int channel, int newPitch) {
	if (noteInfo[channel].freq != -1) {
		int newPeriod = ahwGetTimingPeriod(newPitch);

		if (noteInfo[channel].freq != newPeriod) {  
			noteInfo[channel].freq = newPeriod;
			noteInfo[channel].lastFrequencySent = newPeriod;
			sendValue(channel, '$', newPitch);
		}
	}
} 

void ancNoteChangeVolume(int channel, int newVolume)  {
	if (noteInfo[channel].freq != -1 && noteInfo[channel].volume != newVolume) {
		noteInfo[channel].volume = newVolume;
		noteInfo[channel].lastVolumeSent = newVolume;
		sendValue(channel, '^', newVolume);
	}
}

void ancNoteOff(int channel) {
	if (noteInfo[channel].freq != -1) {
		send(channel, "!");
		noteInfo[channel].freq = -1;
	}
}

void ancUpdate() {
	if (mode & OUTPUT_LOCAL) {
		for(int i=0;i<MAX_CHANNELS;++i) {
			ancUpdateChannel(i);
		}
	}
}

void ancAssignModeToChannel(int channel, int cm) {
	switch(channel) {
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
  	if (mode & OUTPUT_LOCAL) {

		if (noteInfo[channel].freq != -1) {
			int vol = noteInfo[channel].volume;

			switch(noteInfo[channel].mode) {
				case NOTE_PINMODE_DIGITAL:
					vol = vol==0?0:HIGH;
					digitalWrite(noteInfo[channel].pinAssignment, vol); 
					delayMicroseconds(noteInfo[channel].freq);
					digitalWrite(noteInfo[channel].pinAssignment, LOW); 
					delayMicroseconds(noteInfo[channel].freq - 1);     // - 1 to make up for digitaWrite overhead
					break;
				case NOTE_PINMODE_ANALOG:
					vol = (vol > MAX_VOLUME) ? MAX_VOLUME : vol;
					analogWrite(noteInfo[channel].pinAssignment, vol); 
					delayMicroseconds(noteInfo[channel].freq);
					analogWrite(noteInfo[channel].pinAssignment, LOW); 
					delayMicroseconds(noteInfo[channel].freq - 1);     // - 1 to make up for digitaWrite overhead
					break;
			}
		}
	}
}

void sendValue(int channel, char precommand, int n) { 
	char buffer[8]; 
	int idx = 0;

	if (mode & OUTPUT_SERIAL) {
		buffer[0] = precommand;

		while(n > 0) {
			buffer[++idx] = 'J' + (n&0xf);
			n >>= 4;
		}  
		// make the last character lower case, so the reader knows when to finish
		buffer[idx] += 'a'-'A';
		buffer[++idx] = '\0';

		send(channel, buffer);
	}
}

void send(int channel, char *command) {
	if (mode & OUTPUT_SERIAL) {
		if (channel != lastChannelSent) {
			lastChannelSent = channel;
			Serial.print(hex[channel]);
		}
		Serial.print(command);
	}
}
