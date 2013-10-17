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

#include "PolyTime.h"

namespace Polycode{


	Time::Time(int microsec) : microseconds(microsec) {

	}

	Time::~Time() {
	}

	void Time::setSeconds(Number seconds){
		this->microseconds = seconds*1000000.0;
	}
	void Time::setMilliseconds(int milliseconds){
		this->microseconds = milliseconds*1000.0;
	}
	void Time::setMicroseconds(int microseconds){
		this->microseconds = microseconds;
	}

	Number Time::asSeconds() const{
		return this->microseconds/1000000.0;
	}
	int Time::asMilliseconds() const{
		return this->microseconds/1000.0;
	}
	int Time::asMicroseconds() const{
		return this->microseconds;
	}

	Time Time::Seconds(Number seconds){
		return Time(seconds*1000000.0);
	}
	Time Time::Milliseconds(int milliseconds){
		return Time(milliseconds*1000.0);
	}
	Time Time::Microseconds(int microseconds){
		return Time(microseconds);
	}
};