/*
* analoginput.pde - Handlers for analog inputs. Current supports scaled inputs
*                   and one-shot triggers using piezo transducers.  
*                   1. Scaled inputs take the hardware input levels (0-1023 for
*                      Arduino) and re-adjust them to fit between a specified 
*                      range. Floor and ceiling limits are applied. You can 
*                      also limit the input range.
*                   2. Piezo triggers fluctuate, so this watches for when
*                      the input rises above the 'high' threshold. This
*                      indicates a 'hit'. It must then wait for the hardware
*                      to settle down and fall below the 'low' threshold
*                      before it can be re-triggered.
*                    Part of the 'Armstrong' Music System.
*
* Version 1.1
*
*  AUTHOR: Steven Goodwin (StevenGoodwin@gmail.com)
*          Copyright 2008-2014, Steven Goodwin
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

#define DEVICE_TYPE_SCALE	0
#define DEVICE_TYPE_PIEZO	1

struct {
	int type;
	int state;
	int info;
	int ancNoteOff;
	int stateChanged;
	//
	int pinAssignment;
	//
	int inRangeFrom;  
	int inRangeTo;  
	int outRangeFrom;  
	int outRangeTo;  
	//

} analogDevice[MAX_ANALOG_INPUTS];


void aaiInitialize() {
	for(int device=0;device<MAX_ANALOG_INPUTS;++device) {
		analogDevice[device].pinAssignment = -1;
		analogDevice[device].ancNoteOff = 0;
		analogDevice[device].stateChanged = 0;
	}
}

void aaiUpdate() {
	for(int device=0;device<MAX_ANALOG_INPUTS;++device) {
		if (analogDevice[device].pinAssignment != -1) {
			switch(analogDevice[device].type) {
				case DEVICE_TYPE_SCALE:
                                        aaiUpdateScalar(device);
					break;
				case DEVICE_TYPE_PIEZO:
					aaiUpdatePiezoTrigger(device);
					break;
			}
		}
	}
}

int aaiDidStateChange(int device) {
	return analogDevice[device].stateChanged;
}

int aaiGetState(int device) {
	return analogDevice[device].state;
}

int aaiIsNoteOff(int device) {
	return analogDevice[device].ancNoteOff;
}

void aaiSetInputScale(int device, int from, int to) {
	analogDevice[device].inRangeFrom = from;
	analogDevice[device].inRangeTo = to;
}

void aaiSetOutputScale(int device, int from, int to) {
	analogDevice[device].outRangeFrom = from;
	analogDevice[device].outRangeTo = to;
}

int rescale(int device, int value) {
	if (value < analogDevice[device].inRangeFrom) {
		value = analogDevice[device].inRangeFrom;
	} else if (value > analogDevice[device].inRangeTo) {
		value = analogDevice[device].inRangeTo;
	} 
	//
	long result = ((long)(value-analogDevice[device].inRangeFrom) * (long)(analogDevice[device].outRangeTo - analogDevice[device].outRangeFrom));
	result /= (long)(analogDevice[device].inRangeTo - analogDevice[device].inRangeFrom);
	return  result + analogDevice[device].outRangeFrom;
}

/*
** Scale - Re-map the input analogue value to something between given values
*/
void aaiInitializeScalar(int device, int pin, int minValue, int maxValue) {
	pinMode(pin, INPUT);

	analogDevice[device].state = 0;
	analogDevice[device].info = 0;

	analogDevice[device].inRangeFrom = 0;
	analogDevice[device].inRangeTo = 1024;
	analogDevice[device].outRangeFrom = minValue;
	analogDevice[device].outRangeTo = maxValue;

	analogDevice[device].type = DEVICE_TYPE_SCALE;
	analogDevice[device].pinAssignment = pin;
}

int aaiUpdateScalar(int device) {
	int val = analogRead(analogDevice[device].pinAssignment);  // read input value
	int oldState = analogDevice[device].state;
	int oldNoteOff = analogDevice[device].ancNoteOff;

	if (val < analogDevice[device].inRangeFrom) {
		analogDevice[device].ancNoteOff = 1;
	} else {
		analogDevice[device].ancNoteOff = 0;
	}

	analogDevice[device].state = rescale(device, val);

	analogDevice[device].stateChanged = (analogDevice[device].state != oldState || analogDevice[device].ancNoteOff != oldNoteOff);
}


/*
** Piezo - This doesn't trigger until a threshold has been exceeded, and doesn't
** switch back to 0, until it falls below a (potentially different) threshold.
** This is essentially an analog input, which produces a digital result.
*/
void aaiInitializePiezoTrigger(int device, int pin, int lowThreshold, int highThreshold) {
	pinMode(pin, INPUT);

	analogDevice[device].state = 0;
	analogDevice[device].info = 0;

	// BUGWARN: note re-use of variables
	analogDevice[device].inRangeFrom = lowThreshold;
	analogDevice[device].inRangeTo = highThreshold;

	analogDevice[device].type = DEVICE_TYPE_PIEZO;
	analogDevice[device].pinAssignment = pin;
}

int aaiUpdatePiezoTrigger(int device) {
	int val = analogRead(analogDevice[device].pinAssignment);

	if (val >= analogDevice[device].inRangeTo) {
		if (analogDevice[device].info == 0) {
			analogDevice[device].info = 1;
			analogDevice[device].state = 1;
			analogDevice[device].stateChanged = 1;
			return 1;
		}
	} else if (val < analogDevice[device].inRangeFrom) {
		if (analogDevice[device].info == 1) {
			analogDevice[device].info = 0;
			analogDevice[device].state = 0;
			analogDevice[device].stateChanged = 1;
			return 1;
		}
	}

	analogDevice[device].stateChanged = 0;

	return 0;
}
