/*
* digitalinput.h - Header file.  Handles digital input switches
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
#ifndef ARMSTRONG_DIGITAL_INPUT_H
#define ARMSTRONG_DIGITAL_INPUT_H

#define MAX_DIGITAL_INPUTS	13

#define PUSH_BUTTON_DEBOUNCE_TIME	200

void adiInitialize();
void adiUpdate();
int adiUpdateDevice(int device);
void adiRelease();

int adiGetState(int device);
int adiDidStateChange(int device);

void adiInitializeToggleButton(int device, int pin);
void adiInitializePushButton(int device, int pin);

#endif
