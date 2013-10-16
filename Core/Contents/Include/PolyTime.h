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
#include "PolyEventDispatcher.h"

namespace Polycode {
	
	/** 
	* A Stopwatch to measure time elapsed between two points in the program.
	*/ 
	class _PolyExport Time : public PolyBase {
		public:

			/** 
			* Creates a Time object from a value in microseconds
			* @param seconds The time in microseconds, default is 0.
			*/ 
			Time(int microseconds = 0);
			~Time();

			/** 
			* Sets the stored time from a value in seconds
			* @param seconds The time in seconds
			*/ 
			void setSeconds(Number seconds);
			/** 
			* Sets the stored time from a value in milliseconds
			* @param milliseconds The time in milliseconds
			*/ 
			void setMilliseconds(int milliseconds);
			/** 
			* Sets the stored time from a value in microseconds
			* @param microseconds The time in microseconds
			*/ 
			void setMicroseconds(int microseconds);

			inline bool operator < (const Time& rhTime){
				return this->microseconds < rhTime.microseconds;
			}
			inline bool operator > (const Time& rhTime){
				return this->microseconds > rhTime.microseconds;
			}
			inline bool operator <= (const Time& rhTime){
				return this->microseconds <= rhTime.microseconds;
			}
			inline bool operator >= (const Time& rhTime){
				return this->microseconds >= rhTime.microseconds;
			}
			inline bool operator == (const Time& rhTime){
				return this->microseconds == rhTime.microseconds;
			}
			inline bool operator != (const Time& rhTime){
				return this->microseconds != rhTime.microseconds;
			}

			/** 
			* Returns the stored time in seconds
			* @return The time in seconds
			*/ 
			Number asSeconds() const;
			/** 
			* Returns the stored time in milliseconds
			* @return The time in milliseconds
			*/ 
			int asMilliseconds() const;
			/** 
			* Returns the stored time in microseconds
			* @return The time in microseconds
			*/ 
			int asMicroseconds() const;

			static Time Seconds(Number seconds);
			static Time Milliseconds(int milliseconds);
			static Time Microseconds(int microseconds);

		private:

			int microseconds;

	};
}
