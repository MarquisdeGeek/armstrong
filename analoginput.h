/*
* analoginput.hpp - Header file.  
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
#ifndef ARMSTRONG_ANALOGINPUT_H
#define ARMSTRONG_ANALOGINPUT_H

#define MAX_ANALOG_INPUTS	13

void aaiInitialize();
void aaiUpdate();

int aaiGetState(int device);
int aaiDidStateChange(int device);
int aaiIsNoteOff(int device);

void aaiSetInputScale(int device, int from, int to);
void aaiSetOutputScale(int device, int from, int to);

void aaiInitializeScalar(int device, int pin, int minValue, int maxValue);
int aaiUpdateScalar(int device);

void aaiInitializePiezoTrigger(int device, int pin, int lowThreshold, int highThreshold);
int aaiUpdatePiezoTrigger(int device);

#endif
