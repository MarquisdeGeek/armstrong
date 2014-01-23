/*
* waveform.h - Header file.  Basic handling for arbitrary waveforms
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

#ifndef ARMSTRONG_WAVEFORM_H
#define ARMSTRONG_WAVEFORM_H

// System
void awfInitialize();
void awfRelease();

// Waveform generation
void awfFadeSample8(tBYTE *pWave, const int length, const int fadeAmount);
void awfImportSample8(tBYTE *pWave, const int length, const tBYTE *pData, const int lengthData);
void awfGenerateNoise8(tBYTE *pWave, const int length);
void awfGenerate8(tBYTE *pWave, const int length, const float *pHarmonics, const int numHarmonics);

// Playback
void awfPlaySample8(const tBYTE *pWave, const int length);
void awfStop();

void awfTrace8(const tBYTE *pWave, const int length);

#endif
