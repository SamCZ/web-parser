#pragma once

#include <fstream>
#include <filesystem>
#include <optional>

namespace Css
{
	class HierarchyNode
	{

	};

	class Hierarchy : public HierarchyNode
	{
	public:
		static std::optional<Hierarchy> Parse(const std::filesystem::path& path);
		static std::optional<Hierarchy> Parse(std::istream& stream);
		static std::optional<Hierarchy> Parse(std::string& css);
	};
}