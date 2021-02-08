#include <web-parser/css/CssParser.hpp>
#include <fstream>
#include <iostream>

int main()
{
	std::fstream file;
	file.open("style.css");

	if(file.is_open()) {
		std::optional<Css::CssParser::Node> node;
		do {
			node = Css::CssParser::ParseNode(file);

			if(!node.has_value()) break;

			//std::cout << node->Path << std::endl;
		} while(node.has_value());
	}

	return 0;
}