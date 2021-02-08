#pragma once

#include <sstream>
#include <string>

static std::vector<std::string> SplitString(const std::string& str, char delimiter)
{
	std::vector<std::string> list;
	std::stringstream buffer;

	for (char c : str) {
		if(c == delimiter) {
			if(buffer.tellp() != 0) {
				list.push_back(buffer.str());
				buffer.str("");
			}
		} else {
			buffer << c;
		}
	}

	if(buffer.tellp() != 0) {
		list.push_back(buffer.str());
	}

	return list;
}

static std::vector<std::string> SmartSplitAttributes(std::string& tagName)
{
	std::vector<std::string> attributes;

	bool isReadingEscaped = false;
	std::string currentAttrData;

	int firstSpace = -1;

	for (int i = 0; i < tagName.length(); ++i) {
		char currentChar = tagName[i];

		if(std::isspace(currentChar)) {
			firstSpace = i;
			break;
		}
	}

	if(firstSpace == std::string::npos) {
		return attributes;
	}

	for (int i = firstSpace + 1; i < tagName.length(); ++i) {
		char currentChar = tagName[i];

		switch (currentChar) {
			case 0x20: // space (SPC)
			case 0x09: // horizontal tab (TAB)
			case 0x0d: // carriage return (CR)
			case 0x0a: // newline (LF)
				if(!isReadingEscaped) {
					attributes.push_back(currentAttrData);
					currentAttrData = "";
				}
				currentAttrData += " ";
				break;
			case '"':
				isReadingEscaped = !isReadingEscaped;
				break;
			default:
				currentAttrData += currentChar;
				break;
		}


	}

	if(currentAttrData.length() > 0) {
		attributes.push_back(currentAttrData);
	}

	tagName = tagName.substr(0, firstSpace);

	return attributes;
}