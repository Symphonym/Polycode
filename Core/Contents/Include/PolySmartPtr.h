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
			

			SmartPtr() : pointer(NULL) {
				referenceCount = new int(1);
				name = "No Name";
			}
			/**
			* Wraps pointer 'ptr', deleting it when the wrapper is destroyed
			* @param ptr Pointer to be wrapped
			* @param ptrName Optional name to be given to the pointer
			*/					
			SmartPtr(TType *ptr, String ptrName = "No Name") : pointer(ptr), name(ptrName) {
				referenceCount = new int(1);
			}
			/**
			* Shares ownership between two pointers
			* @param rhPtr Pointer to share ownership with
			*/		
			SmartPtr(const SmartPtr& rhPtr) : pointer(NULL), referenceCount(NULL){
				pointer = rhPtr.pointer;
				name = rhPtr.name;
				referenceCount = rhPtr.referenceCount;
				++*referenceCount;
			}
			~SmartPtr(){
				--*referenceCount;
				if(*referenceCount == 0)
				{
					delete pointer;
					delete referenceCount;
					pointer = NULL;
				}
			}

			SmartPtr& operator = (const SmartPtr& rhPtr) {

				if(pointer != rhPtr.pointer){
					--*referenceCount;

					if(*referenceCount == 0)
					{
						delete pointer;
						delete referenceCount;
						pointer = NULL;
					}

					pointer = rhPtr.pointer;
					name = rhPtr.name;
					referenceCount = rhPtr.referenceCount;
					++*referenceCount;
				}

				return *this;
			}
			SmartPtr& operator = (const std::pair<TType*, std::pair<int*, String> >& rhPtr) {

				if(pointer != rhPtr.first){
					--*referenceCount;

					if(*referenceCount == 0)
					{
						delete pointer;
						delete referenceCount;
						pointer = NULL;
					}

					pointer = rhPtr.first;
					name = rhPtr.second.second;
					referenceCount = rhPtr.second.first;
					++*referenceCount;
				}

				return *this;
			}

			TType* operator -> () {
				return pointer;
			}
			const TType* operator -> () const{
				return pointer;
			}
			TType& operator * () {
				return *pointer;
			}
			const TType& operator * () const{
				return *pointer;
			}

			/**
			* Returns the amount of pointers pointing to the underlying object
			*/	
			int getRefCount() const{
				return *referenceCount;
			}
			/**
			* Returns the name of the pointer, if given any
			*/	
			String getName() const{
				return name;
			}
			/**
			* Gets the underlying raw pointer
			* @return The underlying pointer
			*/	
			TType* get() const{
				return pointer;
			}

			/**
			* Gets a copy handle to use when there is polymorphism involved. The template type should be equal to the
			* type of the SmartPtr you're setting it equal to. Which will only work if THIS is derived from the other pointer.
			* @return Copyhandle consisting of all internal data
			*/
			template<typename TTargetCopyType> std::pair<TTargetCopyType*, std::pair<int*, String> > getCopyHandle(){
				return std::make_pair(pointer, std::make_pair(referenceCount, name));
			}

		private:

			TType *pointer;
			String name;

			int *referenceCount;
			
	};

	/**
	* Returns the specified pointer as a SmartPtr
	* @return A SmartPtr object with the specified pointer argument
	*/
	template<typename TPtrType> static SmartPtr<TPtrType> make_smart(TPtrType* ptr, String ptrName = "No Name"){
		return SmartPtr<TPtrType>(ptr, ptrName);
	}

}
