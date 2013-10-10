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

#include "PolySineWave.h"
#include "PolyCoreServices.h"
#include "PolyCore.h"
#include <cmath>
 #include <iostream>

namespace Polycode{

	SineWave::SineWave() : oscillationTime(1), amplitude(1), frequency(1), wavePosition(0){

	}

	void SineWave::setOscillationTime(Number msTime){
		this->oscillationTime = msTime;
	}
	void SineWave::setAmplitude(Number amplitude){
		this->amplitude = amplitude;
	}
	void SineWave::setFrequency(Number frequency){
		this->frequency = frequency;
	}

	void SineWave::setWaveTop(){
		// x = asin(1)/f

		wavePosition = asin(1)/frequency;
	}
	void SineWave::setWaveBottom(){

		// Just set position to wave top, and add half an oscillation
		setWaveTop();
		wavePosition += getWaveEnd() / 2;
	}
	void SineWave::setWaveStart(){
		wavePosition = 0;
	}
	void SineWave::setWavePosition(Number position, bool convertToRadians){

		if(convertToRadians)
			position *= TORADIANS;

		if(position <= getWaveEnd())
			wavePosition = position;
		else
			wavePosition = 0;
	}

	Number SineWave::getWaveValue(){

		Number tempIndex = calculateSineIndex();

		// Update wave position
		wavePosition += CoreServices::getInstance()->getCore()->getElapsed()*getWaveEnd()*oscillationTime;

		Number endOfWave = getWaveEnd();

		// Wave has reached the end
		if(wavePosition >= endOfWave){
			wavePosition = wavePosition - endOfWave;
		}

		return tempIndex;
	}
	Number SineWave::getWavePosition() const{
		return wavePosition;
	}

	Number SineWave::calculateSineIndex() const{
		return amplitude*sin(frequency*wavePosition);
	}
	Number SineWave::getWaveEnd() const{
		return ((2*PI)/frequency);
	}
};