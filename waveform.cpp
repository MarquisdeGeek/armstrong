/*
* waveform.pde - Generate and playback arbitrary samples, using interuprts
*                    Part of the 'Armstrong' Music System.
*
* Version 1.3
*
*  AUTHOR: Steven Goodwin (StevenGoodwin@gmail.com)
*          Copyright 2014, Steven Goodwin
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
#include <avr/interrupt.h>
#include "armstrong.h"
#include "waveform.h"

const float PI2 = 6.28318530718f;

struct {
  const tBYTE *pData;
  int          lengthData;
  boolean      isPlaying;
  //
  int          volume;      // TODO
  
} waveData;


void awfInitialize() {

  /******** Set timer1 for 8-bit fast PWM output ********/
  pinMode(9, OUTPUT);       // Make timer's PWM pin an output
  TCCR1B  = (1 << CS10);    // Set prescaler to full 16MHz
  TCCR1A |= (1 << COM1A1);  // PWM pin to go low when TCNT1=OCR1A
  TCCR1A |= (1 << WGM10);   // Put timer into 8-bit fast PWM mode
  TCCR1B |= (1 << WGM12); 

  /******** Set up timer 2 to call ISR ********/
  TCCR2A = 0;               // We need no options in control register A
  TCCR2B = (1 << CS21);     // Set prescaller to divide by 8
  TIMSK2 = (1 << OCIE2A);   // Set timer to call ISR when TCNT2 = OCRA2
  OCR2A = 32;               // sets the frequency of the generated wave
  sei();                    // Enable interrupts to generate waveform!
  
  delay(5);
}

/******** Called every time TCNT2 = OCR2A *******/
ISR(TIMER2_COMPA_vect) {  // Called each time TCNT2 == OCR2A
  static byte index=0;    // Points to successive entries in the wavetable

//int g_WaveLength;
  if (waveData.isPlaying) {
    OCR1AL = waveData.pData[index++]; // Update the PWM output
  } else {
    OCR1AL = 0;
  }
  
  asm("NOP;NOP");         // Fine tuning
  TCNT2 = 6;              // Timing to compensate for time spent in ISR
}
//*/

void awfRelease() {
  cli();
}

void awfFadeSample8(tBYTE *pWave, const int length, const int fadeAmount) {
  // TODO: Make the fade a % of the peak
  const int zeroLine = 128;
  for (int i=0;i<length;++i) {
    if (pWave[i] > zeroLine) {
      pWave[i] -= fadeAmount;
      if (pWave[i] < zeroLine) {
          pWave[i] = zeroLine;
      }
    } else {
      pWave[i] += fadeAmount;
      if (pWave[i] > zeroLine) {
          pWave[i] = zeroLine;
      }
    }
  }
}

void awfImportSample8(tBYTE *pWave, const int length, const tBYTE *pData, const int lengthData) {
  for (int i=0;i<length;++i) {
    pWave[i] = pData[(i * length) / lengthData];
  }
}

void awfGenerateNoise8(tBYTE *pWave, const int length) {
  for (int i=0;i<length;++i) {
    pWave[i] = rand() * 255;
  }
}

void awfGenerate8(tBYTE *pWave, const int length, const float *pHarmonics, const int numHarmonics) {
  // TODO: Generate the wave in a separate buffer, and normalize
  for (int i=0;i<length;++i) {
    float val = 0;
    for(int h=0;h<numHarmonics;++h) {
      float v = (127 * sin(pHarmonics[h] * (h+1) * (PI2/length)*i));
      val += v;
    }
    //val /= numHarmonics;
    pWave[i] = int(val + 128);
  }
}


// TODO: Test this properly. I think it floods the serial port, causing corrupt output
void awfTrace8(const tBYTE *pWave, const int length) {
  int ysize = 20;
  int xsize = 80;

  for(int y=0;y<ysize;++y) {
    for(int x=0;x<xsize;++x) {
      int i = (x * length) / xsize;
      int h = (pWave[i] * ysize) / 255;  // wave format here is 0-255, this rescales to 0-(ysize-1)
      
      Serial.print(h == y ? "X" : " ");
    }
   Serial.println("");
  }
}

void awfPlaySample8(const tBYTE *pWave, const int length) {
  waveData.pData = pWave;
  waveData.lengthData = length;
  waveData.isPlaying = true;
}


void awfStop() {
  waveData.isPlaying = false;
}

