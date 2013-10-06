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
#include "PolyString.h"
	
namespace Polycode {
	
	/**
	* A wrapper for pointers, using reference counting
	*/
	template<typename TType> class _PolyExport SmartPtr : public PolyBase {
		public:
			

			SmartPtr() : data(NULL) {
				data = new SharedData();
				data->name = "No Name";
				data->pointer = NULL;
				data->referenceCount = 1;
			}
			/**
			* Wraps pointer 'ptr', deleting it when the wrapper is destroyed
			* @param ptr Pointer to be wrapped
			* @param ptrName Optional name to be given to the pointer
			*/					
			SmartPtr(TType *ptr, String ptrName = "No Name") : data(NULL) {
				data = new SharedData();
				data->name = ptrName;
				data->pointer = ptr;
				data->referenceCount = 1;
			}
			/**
			* Shares ownership between two pointers
			* @param rhPtr Pointer to share ownership with
			*/		
			SmartPtr(const SmartPtr& rhPtr) : data(NULL) {
				this->data = rhPtr.data;
				++data->referenceCount;
			}
			~SmartPtr(){
				--data->referenceCount;
				if(data->referenceCount == 0)
				{
					delete data->pointer;
					delete data;
				}
			}

			SmartPtr& operator = (const SmartPtr& rhPtr) {

				if(this != &rhPtr){
					--data->referenceCount;

					if(data->referenceCount == 0)
					{
						delete data->pointer;
						delete data;
					}

					data = rhPtr.data;
					++data->referenceCount;
				}

				return *this;
			}
			TType* operator -> () {
				return this->data->pointer;
			}
			TType& operator * () {
				return *this->data->pointer;
			}

			/**
			* Returns the amount of pointers pointing to the underlying object
			*/	
			int getRefCount() const{
				return data->referenceCount;
			}/**
			* Returns the name of the pointer, if given any
			*/	
			String getName() const{
				return data->name;
			}

		private:

			struct SharedData{
				TType *pointer;
				String name;

				int referenceCount;
			};

			SharedData *data;

			
	};
}
