/*
* notepitch.h - Header file.  Helper functions that understand frequencies.
*                    To work with note IDs (e.g. C4, see noteplay.h)
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
#ifndef ARMSTRONG_NOTEPITCH_H
#define ARMSTRONG_NOTEPITCH_H

float anfGetNotePitch(int note);
float anfQuantizePitch(float iFrequency);
int   anfQuantizeNote(int iNote, int iKey);
int anfGetPitchAsNote(float pitch);

#endif
