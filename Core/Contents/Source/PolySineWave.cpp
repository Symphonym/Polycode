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

namespace Polycode{

	SineWave::SineWave() : oscTimePassed(0), oscillationTime(1000), amplitude(1) {

	}

	void SineWave::setOscillationTime(int msTime){
		this->oscillationTime = msTime;
	}
	void SineWave::setAmplitude(Number amplitude){
		this->amplitude = amplitude;
	}

	void SineWave::setWaveTop(){
		// A = A*sin((oscPass/oscTotal)*(2*PI));
		// 1 = sin((oscPass/oscTotal)*(2*PI));
		// (sin-1(1)/(2*PI))*oscTotal = oscPass
		oscTimePassed = (asin(1)/(2*PI))*oscillationTime;
	}
	void SineWave::setWaveBottom(){
		// -A = A*sin((oscPass/oscTotal)*(2*PI));
		// -1 = sin((oscPass/oscTotal)*(2*PI));
		// (sin-1(-1)/(2*PI))*oscTotal = oscPass
		oscTimePassed = (asin(-1)/(2*PI))*oscillationTime;
	}
	void SineWave::setWaveStart(){
		oscTimePassed = 0;
	}
	void SineWave::setWavePosition(int msTime){
		if(msTime <= oscillationTime)
			oscTimePassed = msTime;
		else
			oscTimePassed = 0;
	}

	Number SineWave::getWaveValue(){

		oscTimePassed += CoreServices::getInstance()->getCore()->getElapsed()*1000.f;

		if(oscTimePassed >= oscillationTime){
			oscTimePassed = oscTimePassed - oscillationTime;
		}

		return calculateSineIndex();
	}

	Number SineWave::calculateSineIndex() const{
		return amplitude*sin((oscTimePassed/oscillationTime)*(2*PI));
	}
};