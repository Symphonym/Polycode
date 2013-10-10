/*
 Copyright (C) 2011 by Ivan Safrin
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */

#pragma once
#include "PolyGlobals.h"
	
namespace Polycode {

	/**
	* A utility class for creating Sine waves with varying amplitudes, frequency and oscillation time
	*/
	class _PolyExport SineWave : public PolyBase {

		public:

			SineWave();

			/**
			* Sets how long it should take between each full oscillation
			* @param msTime Time between each oscillation, in milliseconds
			*/
			void setOscillationTime(Number msTime);
			/**
			* Sets the amplitude of the sine wave, height between lowest and highest points
			* @param amplitude Amplitude multiplier for the sine wave, is 1 by default.
			*/
			void setAmplitude(Number amplitude);
			/**
			* Sets the frequency of the sine wave, i.e distance between oscillations
			* @param frequency Frequency multiplier, is 1 by default.
			*/
			void setFrequency(Number frequency);

			/**
			* Sets the sine wave position to the lowest point of the wave, +amplitude
			*/
			void setWaveTop();
			/**
			* Sets the sine wave position to the lowest point of the wave, -amplitude
			*/
			void setWaveBottom();
			/**
			* Sets the sine wave position to the start of the wave
			*/
			void setWaveStart();
			/**
			* Sets the sine wave position to the specified value
			* @param position New position of the sine wave, in radians, must be lower than getWaveEnd()
			* @param convertToRadians Optional switch if position is given in degrees, converts 'position' to radians if true
			*/
			void setWavePosition(Number position, bool convertToRadians = false);

			/**
			* Updates the current value of the sine wave and returns the index of the wave.
			* @return The index of the sine wave at it's current position, which will be within the range -amplitude to +amplitude.
			*/		
			Number getWaveValue();
			/**
			* Gets the current position of the wave, i.e its position x-wise
			* @return The current x index of the sine wave
			*/	
			Number getWavePosition() const;
			/**
			* Gets the value of x when the wave ends
			* @return The x value of the sine wave at the end of the wave, in radians
			*/	
			Number getWaveEnd() const;

		private:

			Number calculateSineIndex() const;

			Number oscillationTime;
			Number amplitude;
			Number frequency;
			Number wavePosition;
			
	};

}
