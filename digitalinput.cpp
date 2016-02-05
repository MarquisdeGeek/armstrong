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
	int type;
	int state;
	int info;
	int stateChanged;
	//
	int pinAssignment;

} digitalDevice[MAX_DIGITAL_INPUTS];

int updateToggle(int device);
int updatePushButton(int device) ;

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


int adiUpdateDevice(int device) {

	if (digitalDevice[device].pinAssignment != -1) {
		switch(digitalDevice[device].type) {
				case DIGITAL_TYPE_TOGGLE:
					return updateToggle(device);

				case DIGITAL_TYPE_PUSH:
					return updatePushButton(device);

		}
	}

	return 0;
}

int adiGetState(int device) {
	return digitalDevice[device].state;
}

int adiDidStateChange(int device) {
	return digitalDevice[device].stateChanged;
}

/*
** Toggle - Changes state on every rising edge. i.e. whenever the button is pressed
*/
void adiInitializeToggleButton(int device, int pin) {
	pinMode(pin, INPUT);

	digitalDevice[device].state = 0;
	digitalDevice[device].info = 0;

	digitalDevice[device].type = DIGITAL_TYPE_TOGGLE;
	digitalDevice[device].pinAssignment = pin;
}

int updateToggle(int device) {
        digitalDevice[device].stateChanged = 0;
	if (digitalRead(digitalDevice[device].pinAssignment) != HIGH) {
		// button pressed
		if (digitalDevice[device].info == 0) {
			digitalDevice[device].state ^= 1;
			digitalDevice[device].info = 1;
                        digitalDevice[device].stateChanged = 1;
			return 1;
		}
	} else {
		// button not pressed
		if (digitalDevice[device].info == 1) {
			digitalDevice[device].info = 0;
		}
	}

	return 0;
}


/*
** Push button - Basic button, with debounce
*/
void adiInitializePushButton(int device, int pin) {
	pinMode(pin, INPUT);

	digitalDevice[device].state = 0;
	digitalDevice[device].info = 0;

	digitalDevice[device].type = DIGITAL_TYPE_PUSH;
	digitalDevice[device].pinAssignment = pin;
}

int updatePushButton(int device) {
	int val = digitalRead(digitalDevice[device].pinAssignment);
	int changed = 0;
  
	if (val == HIGH && digitalDevice[device].info == LOW) {
		digitalDevice[device].state = 0;
		changed = 1;
	} else if (val == LOW && digitalDevice[device].info == HIGH) {
		digitalDevice[device].state = 1;
		changed = 1;
	}

	digitalDevice[device].info = val;
        digitalDevice[device].stateChanged = changed;

	return changed;
}
