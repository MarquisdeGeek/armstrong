#ifndef ARMSTRONG_H
#define ARMSTRONG_H

#include <WProgram.h>
#include <Arduino.h>

#include "notecontrol.h"
#include "noteplay.h"
#include "notepitch.h"
#include "musicplay.h"

#include "hardware.h"

#include "digitalinput.h"
#include "analoginput.h"

typedef unsigned char tBYTE;


void armInitialize();
void armUpdate();

#endif

