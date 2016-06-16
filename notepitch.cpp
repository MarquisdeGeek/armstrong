/*
* notepitch.pde - Helper functions that understand frequencies.
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
#include <math.h> 
#include "armstrong.h"
#include "notepitch.h"

float fFreqlist[] = {
	2093.00f,  //c7
	2217.46f,
	2349.32f,
	2489.02f,
	2637.02f,

	2793.83f,
	2959.96f,
	3135.96f,
	3322.44f,

	3520.00f,
	3729.31f,
	3951.07f,
};

float fOctaveMult[] = {
	0.0078125,
	0.015625,
	0.03125,
	0.0625f,    
	0.125f,
	0.25f,
	0.5f,
	1.0f,  // 7
	2,
	4,
	8,
};

float anfGetNotePitch(int iNote) {
	int note = iNote % 12;
	int octave = iNote / 12;

	float freq = fFreqlist[note];
	float mult = fOctaveMult[octave];

	return freq * mult;
}


int anfGetPitchAsNote(float pitch) {
  int note = round(12.0*log(pitch/440.0)/log(2.0));
  return note;
}

float anfQuantizePitch(float iFrequency) {
	float lastFreq = fFreqlist[0]  * fOctaveMult[0];
	int octave, note;

	for(octave=0;octave<sizeof(fOctaveMult)/sizeof(fOctaveMult[0]);++octave) {
		for(note=0;note<12;++note) {
			float currentFreq = fFreqlist[note] * fOctaveMult[octave];

			if (iFrequency >= lastFreq && iFrequency <= currentFreq) {
				float diffLower = iFrequency - lastFreq;
				float diffUpper = currentFreq - iFrequency;

				if (diffLower < diffUpper) {
					return lastFreq;
				}
				return currentFreq;
			}
			lastFreq = currentFreq;
		}
	}

	return iFrequency;
}

int anfQuantizeNote(int iNote, int iKey) {
	// iKey: C=0, C#=1 etc
	int note = iNote % 12;

	note -= iKey;
	if (note < 0) {
		note += 12;
	}

	switch(note) {
		case	1:
		case	3:
		case	8:
		case	10:
			return iNote-1;

		case	6: // F# is more likely to go to 'G', for me, at least
			return iNote+1;
	}
	return iNote;
}
