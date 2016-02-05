/*
* noteplay.pde - Handles basic note playback, using the note control library.
*		         The oscillation of each note can be handled by either:
*                1. The ancUpdate methods (using anpPlayNote)
*                2. This code, internally (with anpPlayNoteFor)
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

int tempo = 120;
int crochetPeriod = 60000 / 120; 

void anpSetTempo(int t) {
	tempo = t;
	// at 120 bpm, there's 120 crochets per minute, or 1 per 500 ms
	// Or 1 quaver per 250 ms
	crochetPeriod = 60000 / tempo;
}

int anpGetTempo() {
	return tempo;
}

void anpPlayNote(int iChannel, int iNote, int volume) {
	ancNoteOn(iChannel, anfGetNotePitch(iNote), volume);
}

void anpStopNote(int iChannel) {
	ancNoteOff(iChannel);
}

void anpPlayNoteFor(int iChannel, int iNote, int iDuration, int volume) {
	int freq = anfGetNotePitch(iNote);
	ancNoteOn(iChannel, freq, volume);
	long period = ((long)iDuration * crochetPeriod) / 384;
	long  cycles = ((long)freq * (long)period) / 1000;    // calculate cycles
	for(int i=0;i<cycles;++i) {
		ancUpdateChannel(iChannel);
	}
	ancNoteOff(iChannel);
}

void anpPlayRestFor(int iChannel, int iDuration) {
	int period = (iDuration * crochetPeriod) / 384;
	delay(period);
}

