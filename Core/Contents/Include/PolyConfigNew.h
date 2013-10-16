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
#include "PolySmartPtr.h"
#include <stdexcept>

namespace Polycode {

	struct ConfigEntryNew : public PolyBase {

		enum EntryTypes{
			CONFIG_NUMBER,
			CONFIG_STRING,
			CONFIG_HEADER,
			CONFIG_COMMENT
		};

		/**
		* Sub entries of the ConfigEntry
		*/
		std::vector<SmartPtr<ConfigEntryNew> > entries;
	    ConfigEntryNew* parentEntry;

	    /**
		* Header name or value key, depending on type
		*/
	    String name;

	    String strValue;
	    Number numValue;
	 
	    EntryTypes type;
	};
	
	/**
	* Saves and loads simple config files. Uses a custom file format which stores string or number variables in simple key=value pairs. Also supporting headers to isolate variables within a scope. 
	*/
	class _PolyExport ConfigNew : public PolyBase {
	public:
		/**
		* Default constructor.
		*/ 
		ConfigNew();
		~ConfigNew();		

		/**
		* Load config from a specified file into the specified namespace.
		* @param configNamespace Namespace of the config to load data into.
		* @param fileName Path to the file to load.
		* @return Returns true if the file was loaded succesfully.
		*/
		bool loadConfig(const String& filePath);
		/*

		/**
		* Save the currently loaded config data to an external file
		* @param filePath Where the file should be saved, including it's name at the end of the path
		* $param indentChars Character(s) that will be used when indenting the different headers in the file. Default is '\t'.
		*/		
		void saveConfig(const String& filePath, const String& indentChars = '\t');

		/**
		* Retrieves an entry from the already loaded config file with a path. I.e "Polycode/Window" will retrieve the Window entry within the Polycode header.
		* @param entryPath Path to the entry within the config file, each subheader is specified by adding a '/'.
		* @return The entry at the specified path, returns NULL if it was not found
		*/
		const ConfigEntryNew* getEntry(const String& entryPath) const;

		/**
		* Modifies or creates a number variable within the loaded config file, by specifying its location and new value
		* @param entryPath Path to the variable, including the name of the variable
		* @param value New string value for the variable
		*/		
		void setString(const String& entryPath, const String& value);
		
		/**
		* Modifies or creates a string variable within the loaded config file, by specifying its location and new value
		* @param entryPath Path to the variable, including the name of the variable
		* @param value New number value for the variable
		*/		
		void setNumber(const String& entryPath, Number value);
		
		/**
		* Creates a new header at within the config file at the specified path
		* @param entryPath Path to the variable, including the name of the new header
		*/	
		void createHeader(const String& entryPath);

		/**
		* Returns the numeric value of the config entry stored at the specified path
		* @param entryPath Path to the config entry
		*/				
		Number getNumber(const String& entryPath) const;
		
		/**
		* Returns the string value of the config entry stored at the specified path
		* @param entryPath Path to the config entry
		*/						
		const String& getString(const String& entryPath) const;
		
	private:

		struct EntryNotFoundException : public std::runtime_error{
			EntryNotFoundException() : std::runtime_error("Error: ConfigEntry not found, null exception imminent, shutting down!"){};
		};

		// Only allow the parser to gain direct access to entries
		ConfigEntryNew *getEntry(const String& entryPath);

		const ConfigEntryNew *getEntryRecursive(const String& entryPath, const ConfigEntryNew* currentEntry) const;
		void saveConfigRecursive(std::ofstream& writer, ConfigEntryNew* entry, int indention);
		void createHeaderRecursive(const String& entryPath, ConfigEntryNew* entry, int subPath);

		String convertPath(const String& entryPath) const;
		String getPathSegment(const String& entryPath, int index) const;
		String getPartialPath(const String& entryPath, int startIndex, int endIndex = -1) const;
		
		SmartPtr<ConfigEntryNew> rootEntry;
		String indentChars;
		
	};
}
