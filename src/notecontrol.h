/*
* notecontrol.h - Header file.  
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

#ifndef ARMSTRONG_NOTECONTROL_H
#define ARMSTRONG_NOTECONTROL_H

#define OUTPUT_LOCAL	1
#define OUTPUT_SERIAL	2
#define OUTPUT_MIDI	4

#define MAX_CHANNELS	16

#define MAX_VOLUME  255
#define DEFAULT_VOLUME  MAX_VOLUME

#define NOTE_PINMODE_DIGITAL   0
#define NOTE_PINMODE_ANALOG    1
#define NOTE_PINMODE_MIDI      2

void ancInitialize(int initialMode);
void ancUpdate();
void ancUpdateChannel(int channel);
void ancRelease();

void ancSetOutputMode(int m);
int ancGetOutputMode();
void ancAssignChannelToPin(int channel, int pin);
void ancAssignChannelToMIDI(int channel, int midi_channel);
void ancAssignModeToChannel(int channel, int cm);
int ancIsNoteOn(int channel);

void ancNoteOn(int channel, int pitch, int volume);
void ancNoteChangePitch(int channel, int newPitch);
void ancNoteChangeVolume(int channel, int newVolume);
void ancNoteOff(int channel);

#endif
