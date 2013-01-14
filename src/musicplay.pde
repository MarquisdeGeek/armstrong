/*
* musicplay.pde - Utility code to play complete pieces of music, as described
*                 by the Armstrong music format.
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

#include "noteplay.h"
#include "musicplay.h"

static int scaleOffset[] = { 9, 11, 0, 2, 4, 5, 7,  };  // starts at A

void ampPlayString(int iChannel, char *pString) {
	int ptr = 0;
	int max = strlen(pString);

	int octave = 2;
	int length = MIDI_NOTE_CROCHET;
	int volume = DEFAULT_VOLUME;

	int n, note,nc, v;

	while(ptr < max) {
		char c=pString[ptr++];

		if (ptr == max) {
			n = 0;
		} else {
			n = pString[ptr];
			if (n >= '0' && n <= '9') {
				v = n-'0';
				// continue parsing numbers
				do {
					nc = pString[++ptr];

					if (nc >= '0' && nc <= '9') {
						v *= 10;
						v += nc-'0';
					}
				} while((nc >= '0' && nc <= '9') && ptr < max);
			}
		}

		note = -1;

		if (c == 'O' || c == 'o') { octave = v; }
		else if (c == 'L' || c == 'l') { length = v * 12; }	// convert from our written format to the internal
		else if (c == 'V' || c == 'v') { volume = v; }	// convert from our written format to the internal
		else if (c == 'R' || c == 'r') { anpPlayRestFor(iChannel, length); }
		else if (c >= 'A' && c <= 'G') { note = c-'A'; }
		else if (c >= 'a' && c <= 'g') { note = c-'a'; }

		if (note != -1) {
			note = scaleOffset[note];
			if (n == '+') { ++note; ++ptr; }
			if (n == '-') { --note; ++ptr; }

			anpPlayNoteFor(iChannel, note + octave * 12, length, volume);
		}
	}
}
