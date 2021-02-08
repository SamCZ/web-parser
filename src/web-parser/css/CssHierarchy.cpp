#include "CssHierarchy.hpp"
#include "CssParser.hpp"
#include "CssClassPath.hpp"

#include <sstream>

namespace Css
{
	std::optional<Hierarchy> Hierarchy::Parse(const std::filesystem::path &path)
	{
		std::fstream file;
		file.open(path);

		if(file.is_open()) {
			return Parse(file);
		}

		return {};
	}

	std::optional<Hierarchy> Hierarchy::Parse(std::istream &stream)
	{
		std::optional<CssParser::Node> node;
		do {
			node = CssParser::ParseNode(stream);

			if(!node.has_value()) break;

			ClassPath classPath(node->Path);

			// TODO: Create hierarchy

			std::cout << node->Path << std::endl;
		} while(node.has_value());

		return Hierarchy();
	}

	std::optional<Hierarchy> Hierarchy::Parse(std::string &css)
	{
		std::stringstream ss(css);
		return Parse(ss);
	}
}