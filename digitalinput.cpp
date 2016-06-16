/*
* digitalinput.pde - Handlers for digital inputs. Current supports 
*                   toggle and push buttons.
*                   1. Toggle buttons change state on every 'down' press on
*                      the button. This means it takes two presses to return
*                      to its original state.
*                   2. Push buttons reflect the simple 'push-to-make' switches.
*                      The biggest benefit of using this abstraction is to
*                      simplify the state logic, as you can easily watch for
*                      the 'up' or 'down' motion without extra code.
*                    Buttons are intended to be pulled low when open, and shorted
*                    to high when pressed.
*                    Part of the 'Armstrong' Music System.
*
* Version 1.1
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

#define DIGITAL_TYPE_TOGGLE	0
#define DIGITAL_TYPE_PUSH	1


struct {
	bool type;
	bool state;
	bool info;
	bool stateChanged;
	//
	int pinAssignment;

} digitalDevice[MAX_DIGITAL_INPUTS];

bool adiUpdateDevice(int device);
bool updateToggle(int device);
bool updatePushButton(int device);

void adiInitialize() {
	for(int device=0;device<MAX_DIGITAL_INPUTS;++device) {
		digitalDevice[device].type = -1;
	}
}

void adiRelease() {
}

void adiUpdate() {
	for(int device=0;device<MAX_DIGITAL_INPUTS;++device) {
		digitalDevice[device].stateChanged = adiUpdateDevice(device);
	}
}


bool adiUpdateDevice(int device) {

	if (digitalDevice[device].pinAssignment != -1) {
		switch(digitalDevice[device].type) {
				case DIGITAL_TYPE_TOGGLE:
					return updateToggle(device);

				case DIGITAL_TYPE_PUSH:
					return updatePushButton(device);

		}
	}

	return false;
}

bool adiGetState(int device) {
	return digitalDevice[device].state;
}

bool adiDidStateChange(int device) {
	return digitalDevice[device].stateChanged;
}

bool adiIsPressed(int device) {
	return adiDidStateChange(device) && adiGetState(device);
}

bool adiIsReleased(int device) {
	return adiDidStateChange(device) && !adiGetState(device);
}

/*
** Toggle - Changes state on every rising edge. i.e. whenever the button is pressed
*/
void adiInitializeToggleButton(int device, int pin) {
	pinMode(pin, INPUT);

	digitalDevice[device].state = false;
	digitalDevice[device].info = false;

	digitalDevice[device].type = DIGITAL_TYPE_TOGGLE;
	digitalDevice[device].pinAssignment = pin;
}

bool updateToggle(int device) {

	if (digitalRead(digitalDevice[device].pinAssignment) != HIGH) {
		// button pressed
		if (!digitalDevice[device].info) {
			digitalDevice[device].state = !digitalDevice[device].state;
			digitalDevice[device].info = true;
			return true;
		}
	} else {
		// button not pressed
		if (digitalDevice[device].info) {
			digitalDevice[device].info = false;
		}
	}

	return false;
}


/*
** Push button - Basic button, with debounce
*/
void adiInitializePushButton(int device, int pin) {
	pinMode(pin, INPUT);

	digitalDevice[device].state = false;
	digitalDevice[device].info = false;		// means 'last state' in this instance

	digitalDevice[device].type = DIGITAL_TYPE_PUSH;
	digitalDevice[device].pinAssignment = pin;
}

bool updatePushButton(int device) {
	int val = digitalRead(digitalDevice[device].pinAssignment);
	int changed = 0;
  
	if (val == HIGH && digitalDevice[device].info == LOW) {
		digitalDevice[device].state = true;
		changed = true;
	} else if (val == LOW && digitalDevice[device].info == HIGH) {
		digitalDevice[device].state = false;
		changed = true;
	}

	digitalDevice[device].info = val;

	return changed;
}
