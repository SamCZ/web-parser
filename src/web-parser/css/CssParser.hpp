#pragma once

#include "Css.hpp"

#include <iostream>
#include <istream>
#include <map>
#include <functional>

namespace Css::CssParser
{

	struct Node
	{
		explicit Node(std::string path) : Path(std::move(path)), Style()
		{

		}

		std::string Path;
		Css::Style Style;
	};

	extern std::map<std::string, std::function<void(Node&, std::string&, std::string&)>> TokenParsers;
	extern void InitTokenParsers();
	static bool TokenParsersInitialized = false;

	static std::optional<Node> ParseNode(std::istream& stream)
	{
		if(!TokenParsersInitialized) {
			TokenParsersInitialized = true;
			InitTokenParsers();
		}

		char c = ' ';

		// Skip spaces
		while(std::isspace(stream.peek()))
		{
			stream.get(); // Skip 1 char

			if(stream.bad())
				return {};
		}

		// Node.Path
		std::string path;
		while(true)
		{
			c = stream.get();
			if(stream.bad() || stream.eof())
			{
				if(path.empty()) // Empty path
					return {};
				else if(path.length() == 1 && path[0] == ' ') // Empty path
					return {};
				else
					throw std::runtime_error("Data ended too soon");
			}

			// Comments
			if(c == '/' && stream.peek() == '*')
			{
				stream.get(); // Skip 1 char

				while(true)
				{
					c = stream.get();
					if(stream.bad() || stream.eof())
						throw std::runtime_error("Comment ended too soon");

					if(c == '*' && stream.peek() == '/')
					{
						stream.get(); // Skip 1 char
						break;
					}
				}
				continue;
			}

			// Convert all whitespace characters to simple spaces
			if(std::isspace(c))
				c = ' ';

			if(c == ' ')
			{
				if(path.empty()) // No space at start
					continue;
				if(path.back() == ' ') // Limit spaces to 1
					continue;
			}

			// Start of content
			if(c == '{')
				break;

			path += c;
		}
		if(path.empty()) {
			throw std::runtime_error("No CSS path");
			return {};
		}
		if(path.back() == ' ') // Remove trailing space
			path.resize(path.size() - 1);

		Node node(path);

		std::function<void(std::string&, std::string&)> addContent = [&node](std::string& key, std::string& value)
		{
			if(key == "margin")
			{
				auto margin = node.Style.Margin;

				if(!margin.has_value()) {
					FFourSize data = {};
					margin = data;
				}

				/*List<std::string> split = SplitString(value, ' ');

				switch (split.size()) {
					case 0:
						FUnitValue unitValue = {};

						break;
				}*/
			}
		};

		// Node content
		{
			std::string key;
			std::string value;

			bool readingKey = true;

			while(true)
			{
				c = stream.get();
				if(stream.bad())
					c = '}'; // Act as if the content ended

				// Comments
				if(c == '/' && stream.peek() == '*')
				{
					stream.get(); // Skip 1 char

					while(true)
					{
						c = stream.get();
						if(stream.bad() || stream.eof())
							throw std::runtime_error("Comment ended too soon");

						if(c == '*' && stream.peek() == '/')
						{
							stream.get(); // Skip 1 char
							break;
						}
					}
					continue;
				}

				// Convert all whitespace characters to simple spaces
				if(std::isspace(c))
					c = ' ';

				// Ent of content
				if(c == '}')
				{
					if(!readingKey)
					{
						// Value ended too early but we can still let it be

						// Remove trailing space
						if(!value.empty() && value.back() == ' ')
							value.resize(value.size() - 1);

						//addContent(key, value);
						if(TokenParsers.contains(key)) {
							TokenParsers[key](node, key, value);
						}
#ifdef DEBUG
						else {
							std::cout << "Parser not found for " << key << std::endl;
						}
#endif
						// Cleanup
						key = std::string();
						value = std::string();
					}
					else // readingKey
					{
						if(key.empty()) // Empty key
							break;
						else if(key.length() == 1 && key[0] == ' ') // Empty key
							break;
						else
							throw std::runtime_error("CSS content contains unfinished key");
					}
				}

				if(readingKey)
				{
					switch(c)
					{
						case ' ':
							continue; // Can be problem if inside the name but is OK before and after
						case ':': // End of key, start of value
							if(key.empty())
								throw std::runtime_error("Empty CSS key");
							readingKey = false;
							continue;
						default:
							if((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '-')
							{
								key += c;
								continue;
							}
							else
								throw std::runtime_error("Unexpected invalid character");
					}
				}
				else
				{
					if(c == ';') // End of value
					{
						readingKey = true;

						// Remove trailing space
						if(!value.empty() && value.back() == ' ')
							value.resize(value.size() - 1);

						//addContent(key, value);
						if(TokenParsers.contains(key)) {
							TokenParsers[key](node, key, value);
						}
#ifdef DEBUG
						else {
							std::cout << "Parser not found for " << key << std::endl;
						}
#endif

						// Cleanup
						key = std::string();
						value = std::string();

						continue;
					}
					else
					{
						if(c == ' ')
						{
							if(value.empty()) // No space at start
								continue;
							if(value.back() == ' ') // Limit spaces to 1
								continue;
						}

						value += c;
					}
				}
			}
		}

		return node;
	}
}
