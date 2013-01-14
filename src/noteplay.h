/*
* noteplay.h - Header file.  Stores the basic constants that describe note
*                    events and parameters.
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

#ifndef ARMSTRONG_NOTEPLAY_H
#define ARMSTRONG_NOTEPLAY_H

/*
This section has been copied from 'Steevs MIDI library'

http://freshmeat.net/projects/midilib/

*/

/*
** The MIDI spec only indicates middle C to be
** 60. It doesn't indicate which octave this is.
** Some may consider 4, if they label octaves 
** from -1, instead of 0. I have adopted an octave
** number here for tighter intergration.
*/
#define MIDI_NOTE_MIDDLE_C	MIDI_NOTE_C5

#define MIDI_NOTE_C			0
#define MIDI_NOTE_C_SHARP	1
#define MIDI_NOTE_C_FLAT	-11
#define MIDI_NOTE_D			2
#define MIDI_NOTE_D_SHARP	3
#define MIDI_NOTE_D_FLAT	1
#define MIDI_NOTE_E			4
#define MIDI_NOTE_E_SHARP	5
#define MIDI_NOTE_E_FLAT	3
#define MIDI_NOTE_F			5
#define MIDI_NOTE_F_SHARP	6
#define MIDI_NOTE_F_FLAT	5
#define MIDI_NOTE_G			7
#define MIDI_NOTE_G_SHARP	8
#define MIDI_NOTE_G_FLAT	6
#define MIDI_NOTE_A			9
#define MIDI_NOTE_A_SHARP	10
#define MIDI_NOTE_A_FLAT	8
#define MIDI_NOTE_B			11
#define MIDI_NOTE_B_SHARP	12
#define MIDI_NOTE_B_FLAT	10

#define MIDI_NOTE_C0		0 
#define MIDI_NOTE_C1		12
#define MIDI_NOTE_C2		24
#define MIDI_NOTE_C3		36
#define MIDI_NOTE_C4		48
#define MIDI_NOTE_C5		60
#define MIDI_NOTE_C6		72
#define MIDI_NOTE_C7		84
#define MIDI_NOTE_C8		96
#define MIDI_NOTE_C9		108
#define MIDI_NOTE_C10		120

#define MIDI_OCTAVE_0		MIDI_NOTE_C0
#define MIDI_OCTAVE_1		MIDI_NOTE_C1
#define MIDI_OCTAVE_2		MIDI_NOTE_C2
#define MIDI_OCTAVE_3		MIDI_NOTE_C3
#define MIDI_OCTAVE_4		MIDI_NOTE_C4
#define MIDI_OCTAVE_5		MIDI_NOTE_C5
#define MIDI_OCTAVE_6		MIDI_NOTE_C6
#define MIDI_OCTAVE_7		MIDI_NOTE_C7
#define MIDI_OCTAVE_8		MIDI_NOTE_C8
#define MIDI_OCTAVE_9		MIDI_NOTE_C9
#define MIDI_OCTAVE_10		MIDI_NOTE_C10


/*
** Note Duration (on PPQN=384)
*/
#define MIDI_NOTE_BREVE					1536
#define MIDI_NOTE_MINIM					 768
#define MIDI_NOTE_CROCHET				 384
#define MIDI_NOTE_QUAVER				 192
#define MIDI_NOTE_SEMIQUAVER			  96
#define MIDI_NOTE_SEMIDEMIQUAVER		  48

#define MIDI_NOTE_DOTTED_MINIM			 (768+384)
#define MIDI_NOTE_DOTTED_CROCHET		 (384+192)
#define MIDI_NOTE_DOTTED_QUAVER			 (192+96)
#define MIDI_NOTE_DOTTED_SEMIQUAVER		  (96+48)
#define MIDI_NOTE_DOTTED_SEMIDEMIQUAVER	  (48+24)

#define MIDI_NOTE_TRIPLE_CROCHET			256

//
// These are the Armstrong-specific methods
//
void anpSetTempo(int t);
int anpGetTempo();

void anpPlayNote(int iChannel, int iNote, int volume);
void anpStopNote(int iChannel);
	
void anpPlayNoteFor(int iChannel, int iNote, int iDuration, int volume);
void anpPlayRestFor(int iChannel, int iDuration);

#endif
