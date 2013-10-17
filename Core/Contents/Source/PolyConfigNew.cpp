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
#include "PolyConfigNew.h"
#include "PolyLogger.h"
#include <fstream>

namespace Polycode {

	ConfigNew::ConfigNew() {
		rootEntry = make_smart(new ConfigEntryNew());
		rootEntry->parentEntry = NULL;
		rootEntry->name = "ROOT";
		rootEntry->strValue = "";
		rootEntry->numValue = 0;
		rootEntry->type = ConfigEntryNew::CONFIG_HEADER;

		indentChars = '\t';
	}

	ConfigNew::~ConfigNew() {
		
	}

	bool ConfigNew::loadConfig(const String& filePath) {
		Logger::log("Loading config: %s\n", filePath.c_str());
		
		std::ifstream reader(filePath.c_str(), std::ifstream::in);

		if(!reader.is_open()){
			Logger::log("Error loading config file...\n");
			Logger::log("Error: Could not open file %s\n", filePath.c_str());
			return false;
		}
		else{

			String line = "";
			ConfigEntryNew* parent = rootEntry.get();

			std::size_t lineNumber = 0;
			std::size_t createdHeaders = 0;
			std::size_t endedHeaders = 0;

			while(std::getline(reader, line.contents)){
				++lineNumber;

				std::size_t startClamp = line.find_first_of('[');
				std::size_t endClamp = line.find_last_of(']');
				std::size_t equalSign = line.find_first_of('=');
				std::size_t startOfLine = line.contents.find_first_not_of(" \t\r\n");

				if(startOfLine == std::string::npos)
					startOfLine = 0;

				// Comments and empty lines are stored to use when saving the file later on
				if(line[startOfLine] == '#' || line.contents.empty()){

					SmartPtr<ConfigEntryNew> entry(new ConfigEntryNew());
					entry->parentEntry = parent;
					entry->name = line;
					entry->strValue = "";
					entry->numValue = 0;
					entry->type = ConfigEntryNew::CONFIG_COMMENT;

					parent->entries.push_back(entry);
					continue;
				}

				// Discard comments partially on line
				for(int i = 0; i < line.size(); i++){
					if(line[i] == '#') {
						Logger::log("Warning trigged when loading config fille...\n");
						Logger::log("Warning (Line %i): Same line comments not supported.\n", lineNumber);
						line = line.substr(0, i);
						break;
					}
				}


				// Find start of header [name]
				if(startClamp != std::string::npos && endClamp != std::string::npos){

					SmartPtr<ConfigEntryNew> entry(new ConfigEntryNew());
					entry->parentEntry = parent;
					entry->name = line.substr(startClamp+1, endClamp - (startClamp+1));
					entry->strValue = "";
					entry->numValue = 0;
					entry->type = ConfigEntryNew::CONFIG_HEADER;

					parent->entries.push_back(entry);
					parent = entry.get();

					++createdHeaders;
				}
				// Find end of header /name]
				else if(line.find('/'+parent->name+']') != std::string::npos){
					parent = parent->parentEntry;
					++endedHeaders;
				}
				// Find config entry
				else if(equalSign != std::string::npos){

					// Make sure parent is a header and not a variable
					if(parent->type != ConfigEntryNew::CONFIG_HEADER){
						Logger::log("Error loading config fille...\n");
						Logger::log("Error (Line %i): Can't add subentries to non-header entry.\n", lineNumber);
						return false;
					}

					SmartPtr<ConfigEntryNew> entry(new ConfigEntryNew());
					entry->parentEntry = parent;
					entry->name = line.substr(startOfLine, equalSign - startOfLine);

					// Default values
					entry->strValue = "";
					entry->numValue = 0;

					std::size_t startOfString = line.find_first_of('"', equalSign+1);
					std::size_t endOfString = line.find_last_of('"');

					// Substringed value, except it's disregarding blankspaces/tabs/etc at end of line, i.e a number
					String strNumValue = line.substr(equalSign+1, (line.contents.find_last_not_of(" \t\r\n")+1)-(equalSign+1));

					entry->type = strNumValue.isNumber() ? ConfigEntryNew::CONFIG_NUMBER : ConfigEntryNew::CONFIG_STRING;

					// Check if the value is a number
					if(entry->type == ConfigEntryNew::CONFIG_NUMBER)
						entry->numValue = atof(strNumValue.c_str());
					// Check if quotations for a string was found
					else if(startOfString != std::string::npos && endOfString != std::string::npos)
						entry->strValue = line.substr(startOfString+1, endOfString-(startOfString+1));
					else{
						Logger::log("Error loading config fille...\n");
						Logger::log("Error (Line %i): Invalid value given.\n", lineNumber);
						return false;
					}

					parent->entries.push_back(entry);
				}
			}

			// Make sure all headers have been closed
			if(createdHeaders != endedHeaders){
				String wordEnding = (createdHeaders-endedHeaders) > 1 ? "s are" : " is";
				Logger::log("Error loading config fille...\n");
				Logger::log("Error: %i header%s missing closing tags.\n", createdHeaders-endedHeaders, wordEnding.c_str());
				return false;
			}

		}

		return true;
	}

	void ConfigNew::saveConfig(const String& filePath, const String& indentChars) {
		this->indentChars = indentChars;

		std::ofstream writer(filePath.c_str(), std::ofstream::trunc);
		saveConfigRecursive(writer, rootEntry.get(), 0);
	}

	void ConfigNew::saveConfigRecursive(std::ofstream& writer, ConfigEntryNew* entry, int indention){

		String indent = "";
		for(int i = 0; i < indention; i++)
			indent += indentChars;

		for(int i = 0; i < entry->entries.size(); i++){
			ConfigEntryNew* subEntry = entry->entries[i].get();

			if(subEntry->type == ConfigEntryNew::CONFIG_STRING)
				writer << indent.c_str() << subEntry->name.c_str() << "=\"" << subEntry->strValue.c_str() << "\"" << std::endl;
			else if(subEntry->type == ConfigEntryNew::CONFIG_NUMBER)
				writer << indent.c_str() << subEntry->name.c_str() << "=" << String::NumberToString(subEntry->numValue).c_str() << std::endl;
			else if(subEntry->type == ConfigEntryNew::CONFIG_HEADER)
				writer << indent.c_str() << "[" << subEntry->name.c_str() << "]" << std::endl;
			else if(subEntry->type == ConfigEntryNew::CONFIG_COMMENT)
				writer << subEntry->name.c_str() << std::endl;

			// Recurse into sub members and print those
			if(subEntry->entries.size() > 0)
				saveConfigRecursive(writer, entry->entries[i].get(), indention+1);

			// Close header
			if(subEntry->type == ConfigEntryNew::CONFIG_HEADER)
				writer << indent.c_str() << "/" << subEntry->name.c_str() << "]" << std::endl;

		}
	}

	const ConfigEntryNew* ConfigNew::getEntry(const String& entryPath) const{
		if(entryPath == "ROOT" || entryPath.contents.empty())
			return rootEntry.get();
		else
			return getEntryRecursive(entryPath, rootEntry.get());
	}
 	ConfigEntryNew* ConfigNew::getEntry(const String& entryPath){
 		return const_cast<ConfigEntryNew*>(static_cast<const ConfigNew*>(this)->getEntry(entryPath));	
 	}

	const ConfigEntryNew* ConfigNew::getEntryRecursive(const String& entryPath, const ConfigEntryNew* currentEntry) const {

		std::vector<String> paths = convertPath(entryPath).split('/');
		String desiredEntry = getPathSegment(entryPath, 0);
		String nextEntryPath = getPartialPath(entryPath, 1, -1);

		// We have to recurse deeper
		bool isMoreSubHeaders = entryPath.find('/') != std::string::npos;

		for(int i = 0; i < currentEntry->entries.size(); i++){

			const ConfigEntryNew* subEntry = currentEntry->entries[i].get();

			// Venture down another sub header
			if(subEntry->name == desiredEntry && isMoreSubHeaders) {
				return getEntryRecursive(nextEntryPath, subEntry);
			}
			// Found the right header or the right value/key config entry
			else if(subEntry->name == desiredEntry && !isMoreSubHeaders) {
				return subEntry;
			}
		}

		return NULL;
	}

	void ConfigNew::setString(const String& entryPath, const String& value) {
		ConfigEntryNew* entry = getEntry(entryPath);

		if(entry != NULL){
			entry->strValue = value;
		}
		else{
			// Get header of string variable
			entry = getEntry(getPartialPath(entryPath, 0, -2));

			SmartPtr<ConfigEntryNew> newHeader(new ConfigEntryNew());
			newHeader->parentEntry = entry;
			newHeader->name = entryPath.substr(entryPath.find_last_of('/')+1, entryPath.size());
			newHeader->strValue = value;
			newHeader->numValue = 0;
			newHeader->type = ConfigEntryNew::CONFIG_STRING;
			entry->entries.push_back(newHeader);
		}
	}

	void ConfigNew::setNumber(const String& entryPath, Number value) {
		ConfigEntryNew* entry = getEntry(entryPath);

		// Set value of existing number
		if(entry != NULL){
			entry->numValue = value;
		}
		// Create new number and set its value
		else{

			String entryHeaderPath = entryPath.substr(0, entryPath.find_last_of('/'));
			entry = getEntry(entryHeaderPath);

			if(entry == NULL){
				Logger::log("Error modifying config file...\n");
				Logger::log("Error: Could not find ConfigEntry '%s' when setting number '%s'.\n",
					entryHeaderPath.c_str(),
					getPathSegment(entryPath, -1).c_str());
				throw EntryNotFoundException();
			}

			SmartPtr<ConfigEntryNew> newHeader(new ConfigEntryNew());
			newHeader->parentEntry = entry;
			newHeader->name = entryPath.substr(entryPath.find_last_of('/')+1, entryPath.size());
			newHeader->strValue = "";
			newHeader->numValue = value;
			newHeader->type = ConfigEntryNew::CONFIG_NUMBER;
			entry->entries.push_back(newHeader);
		}		
	}
	void ConfigNew::createHeader(const String& entryPath){
		createHeaderRecursive(convertPath(entryPath), rootEntry.get(), 0);		
	}
	void ConfigNew::createHeaderRecursive(const String& entryPath, ConfigEntryNew* entry, int subPath){

		std::vector<String> paths = entryPath.split('/');

		// Slowly move up the entryPath, creating headers if they don't exist
		String path = getPartialPath(entryPath, 0, subPath);
		ConfigEntryNew* subEntry = getEntry(path);

		// Retrieve the last path segment of the whole path
		String finalName = getPathSegment(entryPath, -1);

		// Create new header if it doesn't exist
		if(subEntry == NULL){
			SmartPtr<ConfigEntryNew> newHeader(new ConfigEntryNew());
			newHeader->parentEntry = entry;

			// Retrieve name of current header
			newHeader->name = getPathSegment(path, -1);
			newHeader->strValue = "";
			newHeader->numValue = 0;
			newHeader->type = ConfigEntryNew::CONFIG_HEADER;

			entry->entries.push_back(newHeader);

			// Check if header we create is the last one of the path
			if(finalName != newHeader->name)
				createHeaderRecursive(entryPath, newHeader.get(), subPath+1);
		}
		// Move into header if exists
		else{
			// Check if header we found is the last one of the path
			if(finalName != subEntry->name)
				createHeaderRecursive(entryPath, subEntry, subPath+1);
		}
	}

	String ConfigNew::convertPath(const String& entryPath) const {
		
		String newPath = entryPath;


		// Make sure path is clear so the processing in getEntry can be as simple
		// as possible.
		while(newPath.find("./") == 0)
			newPath = newPath.substr(2, newPath.size());

		while(newPath.find('/') == 0)
			newPath = newPath.substr(1, newPath.size());

		while(newPath.find_last_of('/') == newPath.size() - 1)
			newPath = newPath.substr(0, newPath.size() - 1);

		while(newPath.find("ROOT/") == 0)
			newPath = newPath.substr(5, newPath.size());

		return newPath;
	}


	Number ConfigNew::getNumber(const String& entryPath) const {
		String path = convertPath(entryPath);
		const ConfigEntryNew* entry = getEntry(path);
		if(entry == NULL){
			Logger::log("Error accessing config file...\n");
			Logger::log("Error: Could not find Number '%s' at the path %s.\n", getPathSegment(path, -1).c_str(), path.c_str());
			throw EntryNotFoundException();
		}
		else
			return entry->numValue;
	}

	const String& ConfigNew::getString(const String& entryPath) const {
		String path = convertPath(entryPath);
		const ConfigEntryNew* entry = getEntry(path);
		if(entry == NULL){
			Logger::log("Error accessing config file...\n");
			Logger::log("Error: Could not find String '%s' at the path %s.\n", getPathSegment(path, -1).c_str(), path.c_str());
			throw EntryNotFoundException();
		}
		else
			return entry->strValue;
	}

	String ConfigNew::getPathSegment(const String& entryPath, int index) const {

		std::vector<String> segments = entryPath.split('/');

		if(segments.size() <= 1)
			return entryPath;

		// -1 is the last segment, 1 is the first
		int legitIndex = index < 0 ? segments.size() + index : index;

		if(legitIndex < 0)
			legitIndex = 0;
		else if(legitIndex > segments.size()-1)
			legitIndex = segments.size()-1;

		return segments[legitIndex];
	}
	String ConfigNew::getPartialPath(const String& entryPath, int startIndex, int endIndex) const {

		String path = "";

		std::vector<String> segments = entryPath.split('/');

		int legitStartIndex = startIndex < 0 ? segments.size() + startIndex : startIndex;
		int legitEndIndex = endIndex < 0 ? segments.size() + endIndex : endIndex;

		// If we want to go back deeper than what is possible from the path, we return the ROOT header
		if(legitEndIndex < 0)
			return String("ROOT");

		// If the path consists of only one segment, we can't do anything with it other than return it back
		else if(segments.size() <= 1)
			return entryPath;

		// Create new path from path segments
		for(int i = legitStartIndex; i <= legitEndIndex && i < segments.size(); i++)
			path += segments[i] + '/';

		return convertPath(path);

	}

}
